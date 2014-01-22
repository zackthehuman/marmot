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

#ifndef MARMOT_STACK_HPP
#define MARMOT_STACK_HPP

#include "marmot/Reference.hpp"
#include <squirrel.h>
#include <iostream>
#include <string>
#include <utility>

namespace marmot {

  namespace detail {
    // TODO
    template<typename T>
    inline T getNull(HSQUIRRELVM vm, std::true_type, int index = -1) {
      // if(lua_isnil(L, index) == 0)
      //    throw sol::sol_error("not nil");
      return nullptr;
    }

    template<typename T>
    inline T getNull(HSQUIRRELVM vm, std::false_type, int index = -1) {
        // T is a class type
        return T(vm, index);
    }

    template<typename T>
    inline T getHelper(HSQUIRRELVM vm, std::true_type, int index = -1) {
        return getNull<T>(vm, std::is_same<std::nullptr_t, T>(), index);
    }

    // template<typename T>
    // inline T getHelper(HSQUIRRELVM vm, std::false_type, int index = -1) {
    //     // T is a fundamental type
    //     return get_arithmetic<T>(vm, std::is_integral<T>{}, index);
    // }
  }

  namespace stack {
    template<typename T>
    inline T get(HSQUIRRELVM vm, int index = -1) {
      return detail::getHelper<T>(vm, std::is_class<T>{}, index);
    }

    template<>
    inline bool get(HSQUIRRELVM vm, int index) {
      SQBool result;
      sq_getbool(vm, index, &result);

      return result == true;
    }

    template<>
    inline float get(HSQUIRRELVM vm, int index) {
      SQFloat value;
      sq_getfloat(vm, index, &value);

      return value;
    }

    template<>
    inline int get(HSQUIRRELVM vm, int index) {
      SQInteger value;
      sq_getinteger(vm, index, &value);

      return value;
    }

    template<>
    inline std::nullptr_t get(HSQUIRRELVM vm, int index) {
      if(sq_gettype(vm, index) != OT_NULL) {
        throw MarmotError("Value not null.");
      }

      return nullptr;
    }

    template<>
    inline std::string get(HSQUIRRELVM vm, int index) {
      const SQChar* value = nullptr;

      if(SQ_SUCCEEDED(sq_getstring(vm, index, &value))) {
          // Yay!
      }

      return {value};
    }

    template<>
    inline Reference get(HSQUIRRELVM vm, int index) {
      return Reference(vm, index);
    }

    inline void push(HSQUIRRELVM vm, const Reference & value) {
      std::cout << "Pushing Reference &" << std::endl;
      value.push();
    }

    inline void push(HSQUIRRELVM vm, const float value) {
      std::cout << "Pushing const float" << std::endl;
      sq_pushfloat(vm, value);
    }

    inline void push(HSQUIRRELVM vm, const int value) {
      std::cout << "Pushing const int" << std::endl;
      sq_pushinteger(vm, value);
    }

    inline void push(HSQUIRRELVM vm, const bool && value) {
      std::cout << "Pushing const bool" << std::endl;
      sq_pushbool(vm, value);
    }

    inline void push(HSQUIRRELVM vm, const std::string & value) {
      std::cout << "Pushing const std::string &" << std::endl;
      sq_pushstring(vm, value.c_str(), -1);
    }

    inline void push(HSQUIRRELVM vm, const std::string && value) {
      std::cout << "Pushing const std::string &&" << std::endl;
      sq_pushstring(vm, value.c_str(), -1);
    }

    inline void push(HSQUIRRELVM vm, const SQChar * value) {
      std::cout << "Pushing const SQChar *" << std::endl;
      sq_pushstring(vm, value, -1);
    }

    inline void push(HSQUIRRELVM vm, std::nullptr_t value) {
      std::cout << "Pushing nullptr_t" << std::endl;
      sq_pushnull(vm);
    }
  }

} // marmot

#endif // MARMOT_STACK_HPP
