# Marmot

Marmot is a C++ wrapper for [Squirrel](http://www.squirrel-lang.org). It has been
built and tested with Squirrel 3.04. Marmot aims to be easy to use and easy to
add into existing projects.

## A Small Example

```cpp
#include <marmot/Stack.hpp>
#include <cassert>

int main() {
  marmot::State sq;
  
  sq["example"] = 1337;
  int value = sq["example"].get<int>();
  assert(value == 1337);

  return 0;
}
```

## Features

- Supports retrieval and setting of multiple types including std::string.
- Simple API that completely away the C stack API.
- operator[]-style manipulation of tables is provided.
- Support for tables.

## Supported Compilers

Marmot makes use of C++11 features. GCC 4.7 and Clang 3.3 or higher should be
able to compile without problems.

## License

Marmot is distributed with an MIT License. You can see LICENSE.txt for more info.

## Examples & Tests

See the `test` directory for examples of how to use the Marmot API.

## Kudos

This project was inspired by other C++ wrappers like [Sol](https://github.com/Rapptz/sol/)
and [Selene](https://github.com/jeremyong/Selene).
