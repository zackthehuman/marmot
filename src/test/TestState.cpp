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
#include "marmot/Table.hpp"
#include <catch/catch.hpp>

//
// Tests for marmot::State
//

TEST_CASE( "State can be instantiated without error", "[marmot::State]" ) {
  REQUIRE_NOTHROW(marmot::State());
}

TEST_CASE( "State creates a non-null Squirrel VM", "[marmot::State]" ) {
  marmot::State sq;
  
  REQUIRE(sq.getVM() != nullptr);
}

TEST_CASE( "Reading and writing variables to root table", "[marmot::State]" ) {
  marmot::State sq;

  const int expectedRead = 304;
  const int actualRead = sq["_versionnumber_"].get<int>();
  
  REQUIRE(expectedRead == actualRead);

  sq["a"] = 1234;
  const auto actualWrite1 = sq["a"].get<int>();

  REQUIRE(1234 == actualWrite1);

  sq["b"] = 12.34f;
  const auto actualWrite2 = sq["b"].get<float>();

  REQUIRE(12.34f == actualWrite2);

  sq["c"] = true;
  const auto actualWrite3 = sq["c"].get<bool>();

  REQUIRE(true == actualWrite3);

  sq["d"] = false;
  const auto actualWrite4 = sq["d"].get<bool>();

  REQUIRE(false == actualWrite4);

  sq["e"] = "marmot";
  const auto actualWrite5 = sq["e"].get<std::string>();

  REQUIRE("marmot" == actualWrite5);
}

TEST_CASE( "State can return the root table", "[marmot::State]" ) {
  marmot::State sq;
  
  REQUIRE_NOTHROW(sq.getRootTable());
}

TEST_CASE( "State proxies reads and writes to root table", "[marmot::State]" ) {
  marmot::State sq;
  marmot::Table root = sq.getRootTable();

  sq["marmot"] = 1337;
  
  REQUIRE(sq["marmot"].get<int>() == root["marmot"].get<int>());
}

TEST_CASE( "State can return the constants table", "[marmot::State]" ) {
  marmot::State sq;
  marmot::Table table;
  
  REQUIRE_NOTHROW(sq.getConstTable());
  REQUIRE(typeid(table) == typeid(sq.getConstTable()));
}

TEST_CASE( "State can return the registry table", "[marmot::State]" ) {
  marmot::State sq;
  marmot::Table table;

  REQUIRE_NOTHROW(sq.getRegistryTable());
  REQUIRE(typeid(table) == typeid(sq.getRegistryTable()));
}

TEST_CASE( "State can create new tables", "[marmot::State]" ) {
  marmot::State sq;
  marmot::Table table;
  
  REQUIRE_NOTHROW(sq.createTable());
  REQUIRE(typeid(table) == typeid(sq.createTable()));
}

TEST_CASE( "State can execute script from a string", "[marmot::State]" ) {
  marmot::State sq;
  
  REQUIRE_NOTHROW(sq.runString("a <- 5;"));
  REQUIRE(sq["a"].get<int>() == 5);

  REQUIRE_NOTHROW(sq.runString("b <- 3.14;"));
  REQUIRE(sq["b"].get<float>() == 3.14f);

  REQUIRE_NOTHROW(sq.runString("c <- true;"));
  REQUIRE(sq["c"].get<bool>() == true);

  REQUIRE_NOTHROW(sq.runString("d <- \"marmot\";"));
  REQUIRE(sq["d"].get<std::string>() == "marmot");

  REQUIRE_NOTHROW(sq.runString("e <- \"marmot\\0with embedded null\";"));
  REQUIRE(std::string(sq["e"].get<const SQChar*>()) == "marmot\0with embedded null");

  REQUIRE_NOTHROW(sq.runString("f <- \"marmot\\0with embedded null\";"));
  REQUIRE(sq["f"].get<std::string>() == "marmot\0with embedded null");
}

TEST_CASE( "State executing script from a string doesn't change the stack by default", "[marmot::State]" ) {
  marmot::State sq;
  const std::string expected = "top";

  marmot::stack::push(sq.getVM(), expected);
  
  REQUIRE_NOTHROW(sq.runString("a <- 5; return 41;"));

  std::string actual = marmot::stack::get<std::string>(sq.getVM(), -1);
  REQUIRE(actual == expected);
}

TEST_CASE( "State executing script from a string does change the stack if specified", "[marmot::State]" ) {
  marmot::State sq;
  const float expected = 41;

  marmot::stack::push(sq.getVM(), expected);
  
  REQUIRE_NOTHROW(sq.runString("a <- 5; return 41;", true));

  auto actual = marmot::stack::get<int>(sq.getVM(), -1);
  REQUIRE(actual == expected);
}

TEST_CASE( "State executing script from a string throws errors as exceptions", "[marmot::State]" ) {
  marmot::State sq;

  REQUIRE_THROWS(sq.runString("marmot++ invalid code"));
}

TEST_CASE( "State propogates Squirrel exceptions as C++ exceptions", "[marmot::State]" ) {
  marmot::State sq;

  REQUIRE_THROWS(sq.runString("throw \"This is really bad.\""));
}
