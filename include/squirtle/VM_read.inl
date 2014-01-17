/*
 * Templated read functions for class squirtle::VM
 */

template<>
std::string VM::read(int index) const {
  const SQChar* value = nullptr;

  if(SQ_SUCCEEDED(sq_getstring(instance, index, &value))) {
      // Yay!
  }

  return std::string(value);
}

template<>
float VM::read(int index) const {
  SQFloat value;
  sq_getfloat(instance, index, &value);

  return value;
}

template<>
int VM::read(int index) const {
  SQInteger value;
  sq_getinteger(instance, index, &value);

  return value;
}

template<>
bool VM::read(int index) const {
  SQBool value;
  sq_getbool(instance, index, &value);

  return value;
}

template<>
std::nullptr_t VM::read(int index) const {
  std::nullptr_t value = nullptr;
  SQObjectType type = sq_gettype(instance, index);
  
  if(type == OT_NULL) {

  }

  return value;
}
