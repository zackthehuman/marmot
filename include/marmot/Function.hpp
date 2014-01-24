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

#ifndef MARMOT_FUNCTION_HPP
#define MARMOT_FUNCTION_HPP

#include "marmot/Reference.hpp"
#include "marmot/Proxy.hpp"
#include "marmot/Stack.hpp"
#include <squirrel.h>
#include <string>
#include <vector>

namespace marmot {

  class Function {
  private:
    HSQUIRRELVM vm = nullptr; // Non-owning pointer
    HSQOBJECT fnObj;
    HSQOBJECT environment;

    bool release() {
      return SQ_SUCCEEDED(sq_release(vm, &fnObj)) && SQ_SUCCEEDED(sq_release(vm, &environment));
    }

  public:
    Function() noexcept
      : vm(nullptr)
    {
      sq_resetobject(&fnObj);
      sq_resetobject(&environment);
    }

    Function(HSQUIRRELVM vm, int envIndex, int fnIndex)
      : vm(vm)
    {
      sq_resetobject(&fnObj);              // Initialize the handle
      sq_getstackobj(vm, fnIndex, &fnObj); // Retrieve the function object handle from index
      sq_addref(vm, &fnObj);               // Adds a reference to the function object
      sq_resetobject(&environment);              // Initialize the handle
      sq_getstackobj(vm, envIndex, &environment); // Retrieve the function object handle from index
      sq_addref(vm, &environment);               // Adds a reference to the function object
    }

    virtual ~Function() { 

    }

    template <typename Ret, typename... Args>
    Ret call(const Args&... args) {
      if(!vm) {
        throw MarmotError("Cannot call function without a VM.");
      }

      if(sq_isnull(environment) || sq_isnull(fnObj)) {
        throw MarmotError("Cannot call function without an environment and stack object.");
      }

      const SQBool hasReturnValue = std::is_void<Ret>::value ? SQFalse : SQTrue;

      sq_pushobject(vm, fnObj);
      sq_pushobject(vm, environment);
      stack::push(vm, args...);
      
      if(SQ_FAILED(sq_call(vm, sizeof...(args) + 1, hasReturnValue, SQTrue))) {
        sq_getlasterror(vm);
        sq_tostring(vm, -1);
        std::string error = stack::get<std::string>(vm, -1);
        sq_pop(vm, 1); // Pop the error
        std::cout << "Error calling the func! " << error << std::endl;        
      }

      // Remove the closure from the stack before popping the return value
      if(hasReturnValue == SQTrue) {
        // The return value is at the top, the closure at -2
        sq_remove(vm, -2);
      } else {
        // The closure is at the top of the stack
        sq_pop(vm, 1);
      }

      return stack::pop<Ret>(vm);
    }
  };

} // marmot

#endif // MARMOT_FUNCTION_HPP
  