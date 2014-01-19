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
#include <iostream>
#include <memory>
#include <string>

namespace marmot {
  namespace detail {

  } // detail

  class Reference {
  private:
    HSQUIRRELVM vm = nullptr; // Non-owning pointer
    HSQOBJECT obj;

    bool release() {
      // std::cout << "RELEASING!" << std::endl;
      releaseCount--;
      return SQ_SUCCEEDED(sq_release(vm, &obj));
    }

  public:
    static int releaseCount;

    Reference() noexcept
      : vm(nullptr)
    {
      sq_resetobject(&obj);
    }

    Reference(HSQUIRRELVM vm, int index)
      : vm(vm)
    {
      // std::cout << "Called \"Reference(HSQUIRRELVM vm, int index)\"" << std::endl;
      sq_resetobject(&obj);            // Initialize the handle
      sq_getstackobj(vm, index, &obj); // Retrieve an object handle from index
      sq_addref(vm, &obj);             // Adds a reference to the object
      // std::cout << "RefCount: " << sq_getrefcount(vm, &obj) << std::endl;
      releaseCount++;
    }

    Reference(Reference&& other) noexcept {
      // std::cout << "Called \"Reference(Reference&& other)\"" << std::endl;
      vm = other.vm;
      obj = other.obj;
      sq_addref(vm, &obj);
      // std::cout << "RefCount: " << sq_getrefcount(vm, &obj) << std::endl;
      releaseCount++;

      other.vm = nullptr;
    }

    Reference(const Reference& other) noexcept {
      // std::cout << "Called \"Reference(const Reference& other)\"" << std::endl;
      vm = other.vm;
      obj = other.obj;
      sq_addref(vm, &obj);
      // std::cout << "RefCount: " << sq_getrefcount(vm, &obj) << std::endl;
      releaseCount++;
    }

    Reference& operator=(Reference&& other) noexcept {
      // std::cout << "Called \"Reference& operator=(Reference&& other)\"" << std::endl;
      release();

      vm = other.vm;
      obj = other.obj;
      sq_addref(vm, &obj);
      // std::cout << "RefCount: " << sq_getrefcount(vm, &obj) << std::endl;
      releaseCount++;

      other.release();
      other.vm = nullptr;

      return *this;
    }

    Reference& operator=(const Reference& other) noexcept {
      // std::cout << "Called \"Reference& operator=(const Reference& other)\"" << std::endl;
      release();

      vm = other.vm;
      obj = other.obj;
      sq_addref(vm, &obj);
      // std::cout << "RefCount: " << sq_getrefcount(vm, &obj) << std::endl;
      releaseCount++;

      return *this;
    }

    virtual ~Reference() {
      // std::cout << "~Reference()" << std::endl;

      if(vm) {
        if(release()) {
          // std::cout << "RELEASING ALL REFERENCES!" << std::endl;
        }
        // std::cout << "RefCount: " << sq_getrefcount(vm, &obj) << std::endl;
      } else {
        // std::cout << "Can't release since my VM is null!" << std::endl;
      }
    }

    void push() {
      sq_pushobject(vm, obj);
    }

    unsigned int getReferenceCount() {
      return sq_getrefcount(vm, &obj);
    }

    std::string getTypeString() {
      push();
      std::string typeString;
      switch(sq_gettype(vm, -1)) {
        case OT_NULL:
          typeString = std::string("null");
          break;
        case OT_INTEGER:
          typeString = std::string("integer");
          break;
        case OT_FLOAT:
          typeString = std::string("float");
          break;
        case OT_STRING:
          typeString = std::string("string");
          break;
        case OT_TABLE:
          typeString = std::string("table");
          break;
        case OT_ARRAY:
          typeString = std::string("array");
          break;
        case OT_USERDATA:
          typeString = std::string("userdata");
          break;
        case OT_CLOSURE:
          typeString = std::string("closure(function)");
          break;
        case OT_NATIVECLOSURE:
          typeString = std::string("native closure(C function)");
          break;
        case OT_GENERATOR:
          typeString = std::string("generator");
          break;
        case OT_USERPOINTER:
          typeString = std::string("userpointer");
          break;
        case OT_CLASS:
          typeString = std::string("class");
          break;
        case OT_INSTANCE:
          typeString = std::string("instance");
          break;
        case OT_WEAKREF:
          typeString = std::string("weak reference");
          break;
        default:
          throw MarmotError("invalid object type");
      }
      sq_pop(vm, 1);

      return typeString;
    }

  };

  int Reference::releaseCount = 0;
} // marmot

#endif // MARMOT_REFERENCE_HPP
