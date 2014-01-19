/*
 * Templated call function for class marmot::VM
 */

template <typename... Ret, typename... Args>
typename _Popper<sizeof...(Ret), Ret...>::type call(const std::string &fun, const Args&... args) {
  // Push function to stack
  pushRootTable(); // Stack size: +1 (global functions are part of the root table)
  push(fun);       // Stack size: +2
  get(-2);         // Stack size: +2 (top value is replaced with value from the table)
  pushRootTable(); // Stack size: +3 (the "this" argument of the function call)

  // Since we push the "this" table before the arguments, we need to add 1.
  const int argCount = (sizeof...(Args)) + 1;
  const SQBool hasReturnValue = (sizeof...(Ret)) > 0 ? SQTrue : SQFalse;

  // Push all arguments to the stack using our variadic push().
  push(args...);   // Stack size: +3 + sizeof...(Args)

  // Call the function.
  if(SQ_FAILED(sq_call(instance, argCount, hasReturnValue, SQFalse))) {
    // We could throw an exception here -- need to determine how to
    // handle error cases.
  }
  // Stack size after function call: +2 (the arguments and "this" value were popped)

  // Now we need to remove the things we pushed to call the function
  // without removing any return values there may be. So we count down from
  // the top of the stack, skipping the number or return values.
  const int removeIndex = -(static_cast<int>(sizeof...(Ret)) + 1);
  remove(removeIndex); // remove function name
  remove(removeIndex); // remove root table

  // Return all the results and remove them from the stack
  return pop<Ret...>();
}