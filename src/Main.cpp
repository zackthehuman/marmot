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

#include "Main.hpp"
#include "marmot/VM.hpp"
#include "marmot/State.hpp"
#include "marmot/Reference.hpp"
#include "marmot/Function.hpp"

#include <iostream>

int main(int argc, char** argv) {
  
  {
    marmot::State sq;

    auto root = sq.getRootTable();
    
    std::cout << "Root table has " << root.getSize() << " slot(s)." << std::endl;

    root.set("someKey", 1337);
    sq["anotherKey"] = "anotherValue";

    std::cout << "Root table has " << root.getSize() << " slot(s)." << std::endl;

    auto slots = root.getSlots();
    for(int i = 0, len = slots.size(); i < len; ++i) {
      std::cout << slots.at(i) << std::endl;
    }

    auto val = root.get<int>("someKey");
    std::cout << "someKey = " << val << std::endl;

    auto val2 = sq["anotherKey"].get<std::string>();
    std::cout << "anotherKey = " << val2 << std::endl;

    // auto newTable = sq.createTable();
    // newTable.set("agent", "Moulder");
    //sq["thirdKey"] = newTable;

    // auto actual = root.get<marmot::Table>("thirdKey");
    //std::cout << "agent = " << actual.get<std::string>("agent") << std::endl;

    // auto print = root.get<marmot::Reference>("print");
    sq.getRootTable().push();
    marmot::stack::push(sq.getVM(), "print");
    sq_get(sq.getVM(), -2);

    marmot::Function printFn(sq.getVM(), -2, -1);

    // Empty the stack
    sq_pop(sq.getVM(), 2);

    std::cout << "Stack size: " << sq_gettop(sq.getVM()) << std::endl;

    // marmot::Reference refBefore(sq.getVM(), -1);
    // std::cout << "Stack top type: " << refBefore.getTypeString() << std::endl;

    printFn.call<void>("this is printing from a proxied call");

    // marmot::Reference refAfter(sq.getVM(), -1);
    // std::cout << "Stack top type: " << refAfter.getTypeString() << std::endl;

    // bool shouldBeTrue = printFn.call<bool>(1, true);
    // bool shouldBeFalse = printFn.call<bool>(1, false);

    // std::cout << "Should be true: " << shouldBeTrue << ", should be false: " << shouldBeFalse << std::endl;

    std::cout << "Stack size: " << sq_gettop(sq.getVM()) << std::endl;

    sq.runString("function add(a, b) { return a + b; }");

    sq.getRootTable().push();
    marmot::stack::push(sq.getVM(), "add");
    sq_get(sq.getVM(), -2);

    marmot::Function addFn(sq.getVM(), -2, -1);

    // Empty the stack
    sq_pop(sq.getVM(), 2);

    int result = addFn.call<int>(2, 3);
    std::cout << "Stack size: " << sq_gettop(sq.getVM()) << std::endl;
    std::cout << "Result: " << result << std::endl;

    try {
      sq.runString("throw \"The system is down.\"");
    } catch(marmot::MarmotError & error) {
      std::cout << "Error!!! " << error.what() << std::endl;
    }
  }
 
  return 0;
};