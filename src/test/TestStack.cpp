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
#include <catch/catch.hpp>

//
// Tests for marmot::stack
//

TEST_CASE( "Pushing and reading values works correctly", "[marmot::stack]" ) {
  marmot::State sq;

  REQUIRE(sq_gettop(sq.getVM()) == 0);

  marmot::stack::push(sq.getVM(), 1);
  REQUIRE(sq_gettop(sq.getVM()) == 1);
  REQUIRE(sq_gettype(sq.getVM(), -1) == OT_INTEGER);
  REQUIRE(marmot::stack::get<int>(sq.getVM(), -1) == 1);

  marmot::stack::push(sq.getVM(), 2.0f);
  REQUIRE(sq_gettop(sq.getVM()) == 2);
  REQUIRE(sq_gettype(sq.getVM(), -1) == OT_FLOAT);
  REQUIRE(marmot::stack::get<float>(sq.getVM(), -1) == 2.0f);

  // Pushing std::string
  marmot::stack::push(sq.getVM(), std::string{"first test string"});
  REQUIRE(sq_gettop(sq.getVM()) == 3);
  REQUIRE(sq_gettype(sq.getVM(), -1) == OT_STRING);
  REQUIRE(marmot::stack::get<std::string>(sq.getVM(), -1) == "first test string");

  // Pushing const SQChar*
  marmot::stack::push(sq.getVM(), "second test string");
  REQUIRE(sq_gettop(sq.getVM()) == 4);
  REQUIRE(sq_gettype(sq.getVM(), -1) == OT_STRING);
  REQUIRE(marmot::stack::get<std::string>(sq.getVM(), -1) == "second test string");

  marmot::stack::push(sq.getVM(), true);
  REQUIRE(sq_gettop(sq.getVM()) == 5);
  REQUIRE(sq_gettype(sq.getVM(), -1) == OT_BOOL);
  REQUIRE(marmot::stack::get<bool>(sq.getVM(), -1) == true);

  marmot::stack::push(sq.getVM(), false);
  REQUIRE(sq_gettop(sq.getVM()) == 6);
  REQUIRE(sq_gettype(sq.getVM(), -1) == OT_BOOL);
  REQUIRE(marmot::stack::get<bool>(sq.getVM(), -1) == false);

  marmot::stack::push(sq.getVM(), nullptr);
  REQUIRE(sq_gettop(sq.getVM()) == 7);
  REQUIRE(sq_gettype(sq.getVM(), -1) == OT_NULL);
  REQUIRE(marmot::stack::get<std::nullptr_t>(sq.getVM(), -1) == nullptr);
}

TEST_CASE( "Pushing with no arguments pushes nothing", "[marmot::stack]" ) {
  marmot::State sq;

  REQUIRE(sq_gettop(sq.getVM()) == 0);
  marmot::stack::push(sq.getVM());
  REQUIRE(sq_gettop(sq.getVM()) == 0);
}

TEST_CASE( "Pushing multiple arguments pushes all of them", "[marmot::stack]" ) {
  marmot::State sq;

  REQUIRE(sq_gettop(sq.getVM()) == 0);
  marmot::stack::push(sq.getVM(), "this", 15, true, nullptr, 5.0f);
  REQUIRE(sq_gettop(sq.getVM()) == 5);
  REQUIRE(sq_gettype(sq.getVM(), 1) == OT_STRING);
  REQUIRE(sq_gettype(sq.getVM(), 2) == OT_INTEGER);
  REQUIRE(sq_gettype(sq.getVM(), 3) == OT_BOOL);
  REQUIRE(sq_gettype(sq.getVM(), 4) == OT_NULL);
  REQUIRE(sq_gettype(sq.getVM(), 5) == OT_FLOAT);
}

TEST_CASE( "Reading values with mismatched types throws exceptions", "[marmot::stack]" ) {
  marmot::State sq;

  marmot::stack::push(sq.getVM(), "not null");
  REQUIRE_THROWS(marmot::stack::get<std::nullptr_t>(sq.getVM(), -1));
}

TEST_CASE( "Values can be pushed and popped", "[marmot::stack]" ) {
  marmot::State sq;

  marmot::stack::push(sq.getVM(), 1);
  marmot::stack::push(sq.getVM(), 2.0f);
  marmot::stack::push(sq.getVM(), "foo bar");
  marmot::stack::push(sq.getVM(), true);
  marmot::stack::push(sq.getVM(), nullptr);

  REQUIRE(sq_gettop(sq.getVM()) == 5);

  std::nullptr_t nullVal = marmot::stack::pop<std::nullptr_t>(sq.getVM());
  bool boolVal = marmot::stack::pop<bool>(sq.getVM());
  std::string stringVal = marmot::stack::pop<std::string>(sq.getVM());
  float floatVal = marmot::stack::pop<float>(sq.getVM());
  int intVal = marmot::stack::pop<int>(sq.getVM());

  REQUIRE(sq_gettop(sq.getVM()) == 0);
  REQUIRE(nullptr == nullVal);
  REQUIRE(true == boolVal);
  REQUIRE("foo bar" == stringVal);
  REQUIRE(2.0f == floatVal);
  REQUIRE(1 == intVal);
}

