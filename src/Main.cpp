#include "Main.hpp"
#include "marmot/VM.hpp"
#include "marmot/State.hpp"
#include "marmot/Reference.hpp"

#include <iostream>

int main(int argc, char** argv) {

  std::cout << "Reference count at beginning: " << marmot::detail::referenceCount << std::endl;
  
  {
    marmot::State sq;
    std::cout << "Reference count in scope: " << marmot::detail::referenceCount << std::endl;

    auto root = sq.getRootTable();
    
    std::cout << "Root table has " << root.getSize() << " slot(s)." << std::endl;

    root.set("someKey", 1337);

    std::cout << "Root table has " << root.getSize() << " slot(s)." << std::endl;

    auto slots = root.getSlots();
    for(int i = 0, len = slots.size(); i < len; ++i) {
      std::cout << slots.at(i) << std::endl;
    }

    auto val = root.get<int>("someKey");
    std::cout << "someKey = " << val << std::endl;
  }

  std::cout << "Reference count at end: " << marmot::detail::referenceCount << std::endl;
 
  return 0;
};