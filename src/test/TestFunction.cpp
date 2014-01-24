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

#include "marmot/Stack.hpp"
#include "marmot/State.hpp"
#include "marmot/Function.hpp"
#include <catch/catch.hpp>

//
// Tests for marmot::stack
//

TEST_CASE( "Function can be default-constructed", "[marmot::Function]" ) {
  REQUIRE_NOTHROW(marmot::Function());

  marmot::Function fn;
  REQUIRE_THROWS(fn.call<void>());
}

TEST_CASE( "Function must be constructed with a vm, environment, and closure object", "[marmot::Function]" ) {
  marmot::State sq;
  marmot::Table root = sq.getRootTable();

  root.push();
  marmot::stack::push(sq.getVM(), "print");
  sq_get(sq.getVM(), -2);

  REQUIRE_NOTHROW(marmot::Function(sq.getVM(), -2, -1));
}

TEST_CASE( "Functions can be called and return values", "[marmot::Function]" ) {
  marmot::State sq;
  marmot::Table root = sq.getRootTable();

  root.push();
  marmot::stack::push(sq.getVM(), "print");
  sq_get(sq.getVM(), -2);

  marmot::Function printFn{sq.getVM(), -2, -1};

  sq_pop(sq.getVM(), 2); // Pops the closure and the root table

  REQUIRE_THROWS(printFn.call<void>()); // ::print() Expects one argument
  REQUIRE_NOTHROW(printFn.call<void>("valid argument"));

  sq.runString("function add(a, b) { return a + b; }");

  root.push();
  marmot::stack::push(sq.getVM(), "add");
  sq_get(sq.getVM(), -2);

  marmot::Function addFn{sq.getVM(), -2, -1};

  REQUIRE_THROWS(addFn.call<void>()); // ::add() Expects two arguments
  REQUIRE_THROWS(addFn.call<int>(2)); // ::add() Expects two arguments
  REQUIRE_THROWS(addFn.call<int>(2, 3, 4)); // ::add() Expects two arguments
  REQUIRE_NOTHROW(addFn.call<int>(2, 3));
  REQUIRE(addFn.call<int>(2, 3) == 5);
  REQUIRE(addFn.call<float>(5.0f, 10.0f) == 15.0f);
  REQUIRE(addFn.call<std::string>("it ", "worked") == "it worked");
}

TEST_CASE( "Functions are called with the correct environment", "[marmot::Function]" ) {
  marmot::State sq;
  marmot::Table root = sq.getRootTable();

  sq.runString("name <- \"Fox Moulder\"; skully <- { name = \"Skully\" }; function printName() { ::print(this.name); return this.name; }");

  root.push();
  marmot::stack::push(sq.getVM(), "printName");
  sq_get(sq.getVM(), -2);

  marmot::Function printRootFn{sq.getVM(), -2, -1};

  marmot::Reference skully = sq["skully"].get<marmot::Reference>();
  skully.push();

  marmot::Function printSkullyFn{sq.getVM(), -1, -2};

  sq_pop(sq.getVM(), 3); // Pops the skully table, closure, and the root table

  REQUIRE(printRootFn.call<std::string>() == "Fox Moulder");
  REQUIRE(printSkullyFn.call<std::string>() == "Skully");
}

