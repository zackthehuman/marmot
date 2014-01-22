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

#ifndef MARMOT_REFERENCE_HPP
#define MARMOT_REFERENCE_HPP

#include "marmot/Error.hpp"
#include <squirrel.h>
#include <memory>
#include <string>

namespace marmot {

  class Reference {
  private:
    HSQUIRRELVM vm = nullptr; // Non-owning pointer
    HSQOBJECT obj;

    bool release() {
      return SQ_SUCCEEDED(sq_release(vm, &obj));
    }

  public:
    Reference() noexcept
      : vm(nullptr)
    {
      sq_resetobject(&obj);
    }

    Reference(HSQUIRRELVM vm, int index)
      : vm(vm)
    {
      sq_resetobject(&obj);            // Initialize the handle
      sq_getstackobj(vm, index, &obj); // Retrieve an object handle from index
      sq_addref(vm, &obj);             // Adds a reference to the object
    }

    Reference(Reference&& other) noexcept {
      vm = other.vm;
      obj = other.obj;
      sq_addref(vm, &obj);

      other.vm = nullptr;
    }

    Reference(const Reference& other) noexcept {
      vm = other.vm;
      obj = other.obj;
      sq_addref(vm, &obj);
    }

    Reference& operator=(Reference&& other) noexcept {
      release();

      vm = other.vm;
      obj = other.obj;
      sq_addref(vm, &obj);

      other.release();
      other.vm = nullptr;

      return *this;
    }

    Reference& operator=(const Reference& other) noexcept {
      if(this != &other) {
        release();

        vm = other.vm;
        obj = other.obj;
        sq_addref(vm, &obj);
      } 

      return *this;
    }

    bool operator==(const Reference& other) noexcept {
      if(getState() && (getState() == other.getState())) {
        push();
        other.push();

        SQInteger cmp = sq_cmp(getState());
        sq_pop(getState(), 2);

        return 0 == cmp;
      }

      return false;
    }

    bool operator!=(const Reference& other) noexcept {
      return !(*this == other);
    }

    virtual ~Reference() {
      if(vm) {
        release();
      } 
    }

    void push() const {
      if(vm) {
        sq_pushobject(vm, obj);        
      } else {
        throw MarmotError("Reference has no HSQUIRRELVM.");
      }
    }

    unsigned int getReferenceCount() {
      if(vm) {
        return sq_getrefcount(vm, &obj);
      }

      return 0;
    }

    HSQUIRRELVM getState() const noexcept {
      return vm;
    }

    std::string getTypeString() {
      push();

      std::string typeString;

      switch(sq_gettype(vm, -1)) {
        case OT_NULL:
          typeString = {"null"};
          break;
        case OT_INTEGER:
          typeString = {"integer"};
          break;
        case OT_FLOAT:
          typeString = {"float"};
          break;
        case OT_STRING:
          typeString = {"string"};
          break;
        case OT_TABLE:
          typeString = {"table"};
          break;
        case OT_ARRAY:
          typeString = {"array"};
          break;
        case OT_USERDATA:
          typeString = {"userdata"};
          break;
        case OT_CLOSURE:
          typeString = {"closure (function)"};
          break;
        case OT_NATIVECLOSURE:
          typeString = {"native closure (C function)"};
          break;
        case OT_GENERATOR:
          typeString = {"generator"};
          break;
        case OT_USERPOINTER:
          typeString = {"userpointer"};
          break;
        case OT_CLASS:
          typeString = {"class"};
          break;
        case OT_INSTANCE:
          typeString = {"instance"};
          break;
        case OT_WEAKREF:
          typeString = {"weak reference"};
          break;
        default:
          throw MarmotError("invalid object type");
      }

      sq_pop(vm, 1);

      return typeString;
    }
  };

} // marmot

#endif // MARMOT_REFERENCE_HPP
