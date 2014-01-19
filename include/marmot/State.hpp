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

#ifndef SQUIRTLE_STATE_HPP
#define SQUIRTLE_STATE_HPP

#include "marmot/Error.hpp"
#include "marmot/Reference.hpp"
#include <squirrel.h>
#include <iostream>
#include <memory>
#include <string>

namespace marmot {
  namespace detail {
    void print(HSQUIRRELVM vm, const SQChar *s, ...) {
      va_list vl; 
      va_start(vl, s);
      vprintf(s, vl);
      va_end(vl);
      printf("\n");
    }

    void error(HSQUIRRELVM vm, const SQChar *s, ...) {
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
    Reference root;

  public:
    State(const unsigned int stackSize = 1024)
      : vm(sq_open(stackSize), sq_close)
      , root()
    {
      sq_setprintfunc(vm.get(), detail::print, detail::error);

      std::cout << "Stack size at start: " << sq_gettop(vm.get()) << std::endl;

      sq_pushroottable(vm.get());
      sq_pushstring(vm.get(), "print", -1);
      sq_get(vm.get(), -2);         //get the function from the root table
      sq_pushroottable(vm.get());   //’this’ (function environment object)
      sq_pushstring(vm.get(), "Hello from marmot::State.", -1);
      sq_call(vm.get(), 2, SQFalse, SQFalse);
      sq_pop(vm.get(), 2);          //pops the roottable and the function

      sq_pushroottable(vm.get());
      root = Reference(vm.get(), -1);
      std::cout << "My type: " << root.getTypeString() << std::endl;
      sq_pop(vm.get(), 1);

      std::cout << "Stack size at finish: " << sq_gettop(vm.get()) << std::endl;
    }

    ~State() {
      std::cout << "~State() " << sq_gettop(vm.get()) << std::endl; 
    }
  };
} // marmot

#endif // SQUIRTLE_STATE_HPP
