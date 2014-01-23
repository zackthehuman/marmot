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

#include "marmot/Table.hpp"
#include "marmot/State.hpp"
#include <catch/catch.hpp>
#include <algorithm>
#include <string>
#include <vector>

//
// Tests for marmot::Table
//

TEST_CASE( "Tables are default-constructable", "[marmot::Table]" ) {
  REQUIRE_NOTHROW(marmot::Table());

  auto ref = marmot::Table();
  REQUIRE(ref.getState() == nullptr);
  REQUIRE_THROWS(ref.push());
  REQUIRE_THROWS(ref.getTypeString());
  REQUIRE(ref.getReferenceCount() == 0);
}

TEST_CASE( "Tables can be constructed with the value from the top of the stack by default", "[marmot::Table]" ) {
  marmot::State sq;

  sq_newtable(sq.getVM());

  auto ref = marmot::Table(sq.getVM()); // Uses index -1
  REQUIRE(ref.getState() != nullptr);
  REQUIRE_NOTHROW(ref.push());
  REQUIRE_NOTHROW(ref.getTypeString());
  REQUIRE(ref.getTypeString() == "table");
  REQUIRE(ref.getReferenceCount() == 1);
}

TEST_CASE( "Tables can be constructed with the value from the stack if index is specified", "[marmot::Table]" ) {
  marmot::State sq;

  sq_newtable(sq.getVM());
  sq_pushroottable(sq.getVM());

  auto ref = marmot::Table(sq.getVM(), -2); // Not the root table
  REQUIRE(ref.getState() != nullptr);
  REQUIRE_NOTHROW(ref.push());
  REQUIRE_NOTHROW(ref.getTypeString());
  REQUIRE(ref.getTypeString() == "table");
  REQUIRE(ref.getReferenceCount() == 1);
}

TEST_CASE( "Tables' size can be read", "[marmot::Table]" ) {
  marmot::State sq;
  auto table = sq.createTable();

  REQUIRE(table.getSize() == 0);

  table["a"] = 1;
  REQUIRE(table.getSize() == 1);
  table["b"] = 56;
  REQUIRE(table.getSize() == 2);
}

TEST_CASE( "Reading and writing variables to a table works", "[marmot::Table]" ) {
  marmot::State sq;
  marmot::Table table = sq.createTable();

  table["a"] = 1234;
  const auto actualWrite1 = table["a"].get<int>();

  REQUIRE(1234 == actualWrite1);

  table["b"] = 12.34f;
  const auto actualWrite2 = table["b"].get<float>();

  REQUIRE(12.34f == actualWrite2);

  table["c"] = true;
  const auto actualWrite3 = table["c"].get<bool>();

  REQUIRE(true == actualWrite3);

  table["d"] = false;
  const auto actualWrite4 = table["d"].get<bool>();

  REQUIRE(false == actualWrite4);

  table["e"] = "marmot";
  const auto actualWrite5 = table["e"].get<std::string>();

  REQUIRE("marmot" == actualWrite5);
}

TEST_CASE( "Reading and writing variables to a table via get() and set() works", "[marmot::Table]" ) {
  marmot::State sq;
  marmot::Table table = sq.createTable();

  table.set("a", 1234);
  const auto actualWrite1 = table.get<int>("a");

  REQUIRE(1234 == actualWrite1);

  table.set("b", 12.34f);
  const auto actualWrite2 = table.get<float>("b");

  REQUIRE(12.34f == actualWrite2);

  table.set("c", true);
  const auto actualWrite3 = table.get<bool>("c");

  REQUIRE(true == actualWrite3);

  table.set("d", false);
  const auto actualWrite4 = table.get<bool>("d");

  REQUIRE(false == actualWrite4);

  table.set("e", "marmot");
  const auto actualWrite5 = table.get<std::string>("e");

  REQUIRE("marmot" == actualWrite5);
}

TEST_CASE( "Tables' slots can be listed", "[marmot::Table]" ) {
  marmot::State sq;
  auto table = sq.createTable();

  auto emptySlots = table.getSlots();
  REQUIRE(emptySlots.size() == 0);

  table["a"] = 1;
  table["b"] = 56;
  table["c"] = "marmot";

  auto containsSlot = [&](const marmot::Table& table, const std::string& slot) {
    const auto & slots = table.getSlots();
    return std::find(std::begin(slots), std::end(slots), slot) != std::end(slots);
  };

  auto threeSlots = table.getSlots();
  REQUIRE(threeSlots.size() == 3);
  REQUIRE(containsSlot(table, "a"));
  REQUIRE(containsSlot(table, "b"));
  REQUIRE(containsSlot(table, "c"));
}
