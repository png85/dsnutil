#include <iostream>
#include <cstdlib>
#include <mpi.h>

#include "config.h"
#include "mpi.hh"

#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/BasicLayout.hh>

/// \brief Default constructor
DSN::MPI::MPI() :
  log(log4cpp::Category::getInstance("DSN::MPI")),
  m_processorName("unknown"), m_rank(-1), m_poolSize(-1) {
  // initialize bare minimum logger to stdout
  log.setPriority(log4cpp::Priority::DEBUG);
  try {
    log4cpp::Appender* logAppender =
      new log4cpp::OstreamAppender("DSN::MPI/stdout", &std::cout);
    logAppender->setLayout(new log4cpp::BasicLayout());
    log.addAppender(logAppender);
  }

  catch (std::bad_alloc& ex) {
    std::cerr << __FILE__ << "(" << __LINE__ << "): Unable to allocate "
	      << "memory for log4cpp objects: " << ex.what();
    exit(EXIT_FAILURE);
  }
}


/// \brief Destructor
///
/// Assures that message passing gets properly shut down on object destruction.
DSN::MPI::~MPI() {
  if (::MPI::Is_initialized()) {
    ::MPI::Finalize();
  }
}


/// \brief Initialize with command line arguments
///
/// Re-initializes message passings with the command line arguments
/// supplies to the program's main() function.
///
/// \param argc Reference to \a argc from main()
/// \param argv Reference to \a argv from main()
///
/// \return true if the initialization was successful, otherwise false
bool DSN::MPI::initialize(int& argc, char**& argv) {
  if (::MPI::Is_initialized()) {
    // if MPI is already initialized we need to finalize and then
    // re-initialize
    try {
      ::MPI::Finalize();
    }

    catch (::MPI::Exception& ex) {
      log.errorStream() << "MPI::Finalize() failed with error code "
			<< ex.Get_error_code() << ": "
			<< ex.Get_error_string();
      return false;
    }
  }

  try {
    // initialize MPI
    ::MPI::Init(argc, argv);
    if (!::MPI::Is_initialized()) {
      log.errorStream() << "MPI::Init() failed!";
      return false;
    }

    ::MPI::COMM_WORLD.Set_errhandler(::MPI::ERRORS_THROW_EXCEPTIONS);
  }

  catch (::MPI::Exception& ex) {
    log.errorStream() << "MPI::Init() failed with error code "
		      << ex.Get_error_code() << ": "
		      << ex.Get_error_string();
    return false;
  }

  log.debugStream() << __FILE__ << "(" << __LINE__ << "): MPI::Init completed";

  try {
    m_poolSize = ::MPI::COMM_WORLD.Get_size();
  }

  catch (::MPI::Exception& ex) {
    log.errorStream() << "Failed to get MPI pool size for this job: "
		      << ex.Get_error_string() << " (error code #"
		      << ex.Get_error_code() << ")";
    return false;
  }

  log.debugStream() << __FILE__ << "(" << __LINE__ << "): Pool size is "
		    << m_poolSize;

  try {
    m_rank = ::MPI::COMM_WORLD.Get_rank();
  }

  catch (::MPI::Exception& ex) {
    log.errorStream() << "Failed to get MPI rank of this instance: "
		      << ex.Get_error_string() << " (error code #"
		      << ex.Get_error_code() << ")";
    return false;
  }

  log.debugStream() << __FILE__ << "(" << __LINE__ << "): Rank is " << m_rank;

  try {
    char buf[MPI_MAX_PROCESSOR_NAME];
    int s = MPI_MAX_PROCESSOR_NAME;
    ::MPI::Get_processor_name(buf, s);
    m_processorName = buf;
  }

  catch (::MPI::Exception& ex) {
    log.errorStream() << "Failed to get MPI processor name for this instance: "
		      << ex.Get_error_string() << " (error code #"
		      << ex.Get_error_code() << ")";
    return false;
  }

  log.debugStream() << __FILE__ << "(" << __LINE__ << "): Processor name is '"
		    << m_processorName << "'";

  return true;
}


/// \brief Check if message passing initialized successfully
bool DSN::MPI::isInitialized() {
  try {
    return ::MPI::Is_initialized();
  }

  catch (::MPI::Exception& ex) {
    log.errorStream() << "Failed to check wether MPI is initialized: "
		      << ex.Get_error_string() << " (error code #"
		      << ex.Get_error_code() << ");";
      return false;
  }
}


/// \brief Get processor name
///
/// Returns the processor name (usually hostname) of the message passing
/// node that runs this instance.
std::string DSN::MPI::processorName() {
  return m_processorName;
}


/// \brief Get message passing rank
int DSN::MPI::rank() {
  return m_rank;
}


/// \brief Get message passing pool size
int DSN::MPI::poolSize() {
  return m_poolSize;
}


/// \brief Copy logging settings
///
/// This copies all appenders and the logging priority from the given
/// log4cpp::Category instance but leaves object ownership as it is.
void DSN::MPI::copyLogSettings(log4cpp::Category& l) {
  // copy all log appenders but leave ownership with the source
  // category
  log4cpp::AppenderSet logAppenders = l.getAllAppenders();
  log.removeAllAppenders();
  for (log4cpp::AppenderSet::iterator i = logAppenders.begin();
       i != logAppenders.end(); ++i) {
    log4cpp::Appender& appender = *(*i);
    log.addAppender(appender);
  }

  // copy logging priority
  log.setPriority(l.getPriority());
}
