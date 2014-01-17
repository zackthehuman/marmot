#ifndef SQUIRTLE_VM_INCLUDED
#define SQUIRTLE_VM_INCLUDED

#include <cstdarg>
#include <iostream>
#include <memory>   
#include <string>
#include <tuple>

#include <squirrel.h>
#include <sqstdio.h>

namespace squirtle {  

  namespace detail {
    void squirrelPrintFunction(HSQUIRRELVM vm, const SQChar *s, ...) {
      va_list vl; 
      va_start(vl, s);
      vprintf(s, vl);
      va_end(vl);
      printf("\n");
    }
  }

  class VM {
  private:
    HSQUIRRELVM instance;

  public:
    VM()
      : instance(nullptr)
    {
      instance = sq_open(1024);

      sq_setprintfunc(instance, detail::squirrelPrintFunction, detail::squirrelPrintFunction);
      pushRootTable();
      sqstd_register_iolib(instance);
      sqstd_dofile(instance, _SC("squirtle.nut"), SQFalse, SQTrue);
    }

    ~VM() {
      sq_close(instance);
    }

    void pushRootTable() {
      sq_pushroottable(instance);
    }

    void push() {
      // This allows us to call functions with no arguments.
    }

    void push(const SQChar * value) {
      sq_pushstring(instance, value, -1);
    }

    void push(const std::string & value) {
      sq_pushstring(instance, value.c_str(), -1);
    }

    void push(const std::string && value) {
      sq_pushstring(instance, value.c_str(), -1);
    }

    void push(const float value) {
      sq_pushfloat(instance, value);
    }

    void push(const int value) {
      sq_pushinteger(instance, value);
    }

    void push(const bool value) {
      sq_pushbool(instance, value);
    }

    void push(std::nullptr_t value) {
      sq_pushnull(instance);
    }

    /**
     * Pops n elements from the stack.
     * @param numElements - the number of elements to pop.
     */
    void pop(int numElements) {
      sq_pop(instance, numElements);
    }

    /**
     * Pops one element from the stack.
     */
    void popTop() {
      sq_poptop(instance);
    }

    /**
     * Removes an element from the stack at `index`.
     * @param index - the stack index of the element to remove
     */
    void remove(int index) {
      sq_remove(instance, index);
    }
    
    void get(int index) {
      sq_get(instance, index);
    }

    int getStackTop() const {
      return sq_gettop(instance);
    }
    
    template <typename T, typename... Ts>
    void push(const T value, const Ts... values) {
      push(value);
      push(values...);
    }

    template<typename T> T read(int index) const;

    #include "squirtle/VM_pop.inl"
    #include "squirtle/VM_call.inl"
  };

  #include "squirtle/VM_read.inl"

} // squirtle

#endif // SQUIRTLE_VM_INCLUDED