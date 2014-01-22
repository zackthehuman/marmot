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

#include "marmot/State.hpp"
#include "marmot/Reference.hpp"
#include "marmot/Table.hpp"
#include <catch/catch.hpp>

//
// Tests for marmot::Reference
//

TEST_CASE( "Reference can be default-constructed", "[marmot::Reference]" ) {
  REQUIRE_NOTHROW(marmot::Reference());

  auto ref = marmot::Reference();
  REQUIRE(ref.getState() == nullptr);
  REQUIRE_THROWS(ref.push());
  REQUIRE_THROWS(ref.getTypeString());
  REQUIRE(ref.getReferenceCount() == 0);
}

TEST_CASE( "References are equal if they refer to the same object on the same vm", "[marmot::Reference]" ) {
  marmot::State sq;
  auto table = sq.createTable();
  table.push();

  // Both references refer to the same table on the top of the stack
  auto refA = marmot::Reference(sq.getVM(), -1);
  auto refB = marmot::Reference(sq.getVM(), -1);

  REQUIRE(refA == refB);
}

TEST_CASE( "References are not equal if they refer to different objects on the same vm", "[marmot::Reference]" ) {
  marmot::State sq;
  auto tableA = sq.createTable();
  auto tableB = sq.createTable();
  tableA.push();
  tableB.push();

  // Both references refer to the same table on the top of the stack
  auto refA = marmot::Reference(sq.getVM(), -1);
  auto refB = marmot::Reference(sq.getVM(), -2);

  REQUIRE(refA != refB);
}

TEST_CASE( "References are not equal if they refer to different objects on different vms", "[marmot::Reference]" ) {
  marmot::State sqA;
  marmot::State sqB;
  auto tableA = sqA.createTable();
  auto tableB = sqB.createTable();
  tableA.push();
  tableB.push();

  // Both references refer to the same table on the top of the stack
  auto refA = marmot::Reference(sqA.getVM(), -1);
  auto refB = marmot::Reference(sqB.getVM(), -1);

  REQUIRE(refA != refB);
}