#include <mpi.h>

#include "config.h"
#include "mpi.hh"

int main(int argc, char** argv) {
  DSN::MPI& mpi = DSN::MPI::getInstance();

  if (!mpi.initialize(argc, argv)) {
    std::cerr << "Unable to initialize MPI!" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "MPI initialized successfully as job #"
	    << mpi.rank() << "/" << mpi.poolSize() << " on "
	    << mpi.processorName() << std::endl;

  log4cpp::Category::shutdown();

  return EXIT_SUCCESS;
}
