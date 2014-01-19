#ifndef MARMOT_VM_INCLUDED
#define MARMOT_VM_INCLUDED

#include <cstdarg>
#include <iostream>
#include <memory>   
#include <string>
#include <tuple>

#include <squirrel.h>
#include <sqstdio.h>

namespace marmot {  

  namespace detail {
    void squirrelPrintFunction(HSQUIRRELVM vm, const SQChar *s, ...) {
      va_list vl; 
      va_start(vl, s);
      vprintf(s, vl);
      va_end(vl);
      printf("\n");
    }

    SQInteger print_foo_bar(HSQUIRRELVM vm) {
      std::cout << "print_foo_bar, " << sq_gettop(vm) << std::endl;
      return 0;
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

      sq_pushroottable(instance);
      sq_pushstring(instance,"print_foo_bar",-1);
      sq_newclosure(instance,detail::print_foo_bar,0); //create a new function
      // sq_pushinteger(instance, 41);
      sq_newslot(instance,-3,SQFalse);
      sq_pop(instance,1); //pops the root table

      pushRootTable();
      sqstd_register_iolib(instance);
      sqstd_dofile(instance, _SC("marmot.nut"), SQFalse, SQTrue);
      pop(1);
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

    #include "marmot/VM_pop.inl"
    #include "marmot/VM_call.inl"
  };

  #include "marmot/VM_read.inl"

} // marmot

#endif // MARMOT_VM_INCLUDED