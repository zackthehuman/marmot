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

#ifndef MARMOT_PROXY_HPP
#define MARMOT_PROXY_HPP

#include "marmot/Reference.hpp"
#include "marmot/Table.hpp"
#include "marmot/Stack.hpp"
#include <squirrel.h>
#include <string>

namespace marmot {

  template<typename Table, typename Key>
  class Proxy {
  private:
    Table & table;
    Key & key;

  public:
    template<typename T>
    Proxy(Table& table, T&& key)
      : table(table)
      , key(std::forward<T>(key))
    {

    }

    template<typename T>
    T get() const {
      return table.template get<T>(key);
    }

    template<typename T>
    Proxy& set(T&& item) {
      table.set(key, std::forward<T>(item));
      return *this;
    }

    template<typename U>
    void operator=(U&& other) {
      table.set(key, std::forward<U>(other));
    }
  };
} // marmot

#endif // MARMOT_PROXY_HPP
