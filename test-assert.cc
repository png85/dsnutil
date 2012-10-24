#include <iostream>

#include <cstdlib>

#include "assert.hh"

int main(int argc, char** argv) {
  std::cout << "assert.hpp throwing assert() test" << std::endl;
  try {
    assert(1 == 0);
  }

  catch (const std::exception& ex) {
    std::cout << "Got exception: " << ex.what() << std::endl;
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
