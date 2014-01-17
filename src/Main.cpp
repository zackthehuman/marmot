#include "Main.hpp"
#include "squirtle/VM.hpp"

#include <iostream>

int main(int argc, char** argv) {
  std::cout << "Squirtle test program." << std::endl;

  squirtle::VM machine;
  std::cout << "Stack size: " << machine.getStackTop() << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;


  machine.call("print", "called me and printed!");

  std::cout << "Stack size: " << machine.getStackTop() << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;

  machine.push(11, 22, 33, 44, 55);
  std::cout << "Stack size: " << machine.getStackTop() << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;

  int output1 = 0;

  output1 = machine.call<int>("add", 2, 3);

  std::cout << "Output1: " << output1 << std::endl;
  std::cout << "Stack size: " << machine.getStackTop() << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;

  machine.call("noArgs");

  std::cout << "Stack size: " << machine.getStackTop() << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;
  std::cout << "Stack top:    " << machine.read<int>(-1) << std::endl;
  std::cout << "Stack bottom: " << machine.read<int>(2) << std::endl;

  int output2 = machine.call<int>("noArgsWithReturnValue");
  std::cout << "Output2: " << output2 << std::endl;

  return 0;
};