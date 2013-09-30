#include <iostream>
#include <fstream>
#include <cstdlib>

#include "csv.hh"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "This test needs a CSV file as its first argument!"
	      << std::endl;
    return EXIT_FAILURE;
  }

  // open input file
  std::cout << "-- Using test data from '" << argv[1] << "'" << std::endl;
  std::ifstream file(argv[1]);
  if (!file.good()) {
    std::cerr << "Unable to load CSV file from '" << argv[1] << "'!";
    return EXIT_FAILURE;
  }

  // parse each row and dump its contents on stdout
  DSN::CSV::Row row;
  while (file >> row) {
    std::cout << "Read row of size " << row.size() << " cells:" << std::endl;
    for (size_t i=0; i<row.size(); i++) {
      std::cout << "row[" << i << "] = '" << row[i] << "'" << std::endl;
    }
  }

  return EXIT_SUCCESS;
}
