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

  // parse CSV data using iterator based interface
  for (DSN::CSV::Iterator i(file); i!=DSN::CSV::Iterator(); ++i) {
    DSN::CSV::Row row = (*i);
    std::cout << "Read row of size " << row.size() << " cells:" << std::endl;
    for (int j=0; j<row.size(); j++) {
      std::cout << "row[" << j << "] = '" << row[j] << "'" << std::endl;
    }
  }

  return EXIT_SUCCESS;
}
