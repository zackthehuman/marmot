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

#ifndef MARMOT_TABLE_HPP
#define MARMOT_TABLE_HPP

#include "marmot/Reference.hpp"
#include "marmot/Proxy.hpp"
#include "marmot/Stack.hpp"
#include <squirrel.h>
#include <string>
#include <vector>

namespace marmot {

  // Forward declarations
  template<typename Table, typename Key>
  class Proxy;

  class Table : public Reference {
  public:
    Table() noexcept
      : Reference()
    {

    }

    Table(HSQUIRRELVM vm, int index = -1)
      : Reference(vm, index)
    {
      // TODO: Perform type assertion
    }

    virtual ~Table() { 

    }

    int getSize() const {
      push();
      auto size = sq_getsize(getState(), -1);
      sq_pop(getState(), 1);

      return size;
    }

    std::vector<std::string> getSlots() const {
      std::vector<std::string> slots;

      push();                   // Push the table
      sq_pushnull(getState());  // Push a null iterator

      while(SQ_SUCCEEDED(sq_next(getState(), -2))) {
          // Here -1 is the value and -2 is the key
          slots.push_back(stack::get<std::string>(getState(), -2));
          sq_pop(getState(), 2); // Pops key and value before the next iteration
      }

      sq_pop(getState(), 2); // Pops the null iterator and the table

      return slots;
    }

    template<typename T, typename U>
    Table& set(T&& key, U&& value) {
      push();
      stack::push(getState(), std::forward<T>(key));
      stack::push(getState(), std::forward<U>(value));
      sq_newslot(getState(), -3, SQFalse);
      sq_pop(getState(), 1);
      return *this;
    }

    template<typename T, typename U>
    T get(U&& key) {
      push();
      stack::push(getState(), std::forward<U>(key));
      sq_get(getState(), -2);
      auto value = stack::get<T>(getState(), -1);
      sq_pop(getState(), 2); // Pops the slot value and table
      return value;
    }

    template<typename T>
    Proxy<Table, T> operator[](T&& key) {
      return Proxy<Table, T>(*this, std::forward<T>(key));
    }

    template<typename T>
    Proxy<const Table, T> operator[](T&& key) const {
      return Proxy<const Table, T>(*this, std::forward<T>(key));
    }
  };

} // marmot

#endif // MARMOT_TABLE_HPP
