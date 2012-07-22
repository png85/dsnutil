#include <cstdlib>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/BasicLayout.hh>
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

  std::cout << "Testing shift operator to stdout: " << mpi << std::endl;

  log4cpp::Category& log = log4cpp::Category::getInstance("test-mpi");
  try {
    log4cpp::Appender* a = new log4cpp::OstreamAppender("test-mpi/stdout",
							&std::cout);
    a->setLayout(new log4cpp::BasicLayout());
    log.addAppender(a);
  }

  catch (std::bad_alloc& ex) {
    std::cerr << "Unable to allocate memory for main()'s log4cpp::Category: "
	      << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  log.setPriority(log4cpp::Priority::DEBUG);

  log.infoStream() << "Testing shift operator to log4cpp::CategoryStream: "
		   << mpi;

  mpi.copyLogSettings(log);
  log.debugStream() << "Successfully copied logging settings to MPI instance.";

  log4cpp::Category::shutdown();

  return EXIT_SUCCESS;
}
