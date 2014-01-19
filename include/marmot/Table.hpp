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
#include <squirrel.h>
#include <string>
#include <vector>

namespace marmot {

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

    std::vector<std::string> getSlots() const {
      std::vector<std::string> slots;

      push();                   // Push the table
      sq_pushnull(getState());  // Push a null iterator

      while(SQ_SUCCEEDED(sq_next(getState(), -2))) {
          // Here -1 is the value and -2 is the key
          const SQChar * key;
          sq_getstring(getState(), -2, &key);
          slots.push_back({key});
          sq_pop(getState(), 2); // Pops key and value before the next iteration
      }

      sq_pop(getState(), 2); // Pops the null iterator and the table

      return slots;
    }
  };

} // marmot

#endif // MARMOT_TABLE_HPP
