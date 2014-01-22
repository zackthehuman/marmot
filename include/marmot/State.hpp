// The MIT License (MIT)

// Copyright (c) 2014 Zachary Mulgrew, ZackTheHuman

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef MARMOT_STATE_HPP
#define MARMOT_STATE_HPP

#include "marmot/Error.hpp"
#include "marmot/Reference.hpp"
#include "marmot/Proxy.hpp"
#include "marmot/Table.hpp"
#include "marmot/Stack.hpp"
#include <squirrel.h>
#include <iostream>
#include <memory>
#include <string>

namespace marmot {
  namespace detail {
    inline void print(HSQUIRRELVM vm, const SQChar *s, ...) {
      va_list vl; 
      va_start(vl, s);
      vprintf(s, vl);
      va_end(vl);
      printf("\n");
    }

    inline void error(HSQUIRRELVM vm, const SQChar *s, ...) {
      printf("Squirrel error! ");
      va_list vl; 
      va_start(vl, s);
      vprintf(s, vl);
      va_end(vl);
      printf("\n");
    }
  } // detail

  class State {
  private:
    std::unique_ptr<SQVM, void(*)(SQVM*)> vm;
    Table root;
    Table constants;
    Table registry;

    /**
     * Returns a Table object to the root table.
     * @return a Table object to the root table.
     */
    Table && _getRootTable() {
      sq_pushroottable(vm.get());
      auto result = Table(vm.get(), -1);
      sq_pop(vm.get(), 1);

      return std::move(result);
    }

    /**
     * Returns a Table object to the constants table.
     * @return a Table object to the constants table.
     */
    Table && _getConstTable() {
      sq_pushconsttable(vm.get());
      auto result = Table(vm.get(), -1);
      sq_pop(vm.get(), 1);

      return std::move(result);
    }

    /**
     * Returns a Table object to the registry table.
     * @return a Table object to the registry table.
     */
    Table && _getRegistryTable() {
      sq_pushregistrytable(vm.get());
      auto result = Table(vm.get(), -1);
      sq_pop(vm.get(), 1);

      return std::move(result);
    }

  public:
    State(const unsigned int stackSize = 1024)
      : vm(sq_open(stackSize), sq_close)
      , root(_getRootTable())
      , constants(_getConstTable())
      , registry(_getRegistryTable())
    {
      sq_setprintfunc(vm.get(), detail::print, detail::error);
    }

    ~State() {
    }

    /**
     * Gets a handle (pointer) to the owned Squirrel VM.
     * @return Squirrel VM handle
     */
    const HSQUIRRELVM getVM() const {
      return vm.get();
    }

    /**
     * Compiles and executes a string of Squirrel code.
     * @param script [description]
     */
    void runString(const std::string & script, bool pushReturnValue = false) {
      SQRESULT compileResult = sq_compilebuffer(
        getVM(),
        script.c_str(),
        static_cast<SQInteger>(script.size() * sizeof(SQChar)), _SC(""),
        true
      );

      if(SQ_FAILED(compileResult)) {
        sq_getlasterror(getVM());
        std::string error = stack::get<std::string>(getVM(), -1);
        sq_pop(getVM(), 1); // Pop the error
        throw MarmotError(error);
      } else {
        // The closure is on the stack at position -1
        sq_pushroottable(getVM());
        SQRESULT callResult = sq_call(getVM(), 1, pushReturnValue, true);
         // Remove the closure from the stack, reatining the return value if specified
        sq_remove(getVM(), pushReturnValue ? -2 : -1);

        if(SQ_FAILED(callResult)) {
          sq_getlasterror(getVM());
          sq_tostring(getVM(), -1); // Convert the error object to a string
          std::string error = stack::get<std::string>(getVM(), -1);
          sq_pop(getVM(), 1); // Pop the error
          throw MarmotError(error);
        }
      }
    }

    /**
     * Gets the root table.
     * @return a wrapper object for the root table
     */
    Table & getRootTable() {
      return root;
    }

    /**
     * Gets the constants table.
     * @return a wrapper object for the constants table
     */
    Table & getConstTable() {
      return constants;
    }

    /**
     * Gets the registry table.
     * @return a wrapper object for the registry table
     */
    Table & getRegistryTable() {
      return registry;
    }

    /**
     * Creates a new table and returns a wrapper object for it.
     * @return a wrapper object for a native Squirrel table
     */
    Table createTable() {
      root.push();
      sq_newtable(getVM());

      Table result(getVM(), -1);
      sq_pop(getVM(), 2); // Pops the new table and the root table

      return result;
    }

    /**
     * Proxies getting and setting values through the state to the root table.
     * This can be used to get or set global variables.
     *
     * @return a proxy object to the value found by Key in the root table
     */
    template<typename T>
    Proxy<Table, T> operator[](T&& key) {
      return root[std::forward<T>(key)];
    }

    /**
     * Proxies getting and setting values through the state to the root table.
     * This can be used to get global variables.
     *
     * @return a proxy object to the value found by Key in the root table (const)
     */
    template<typename T>
    Proxy<const Table, T> operator[](T&& key) const {
      return root[std::forward<T>(key)];
    }
  };
} // marmot

#endif // MARMOT_STATE_HPP
