/** \file
 *
 * \brief Implementation of DSN::MPI::SSend
 *
 * This file implements the DSN::MPI::SSend functor for point-to-point
 * communication in a message passing environment.
 *
 * \author Peter Hille (png!das-system)
 *
 * \version 1.0 Initial release
 */
#include "mpi.hh"

using namespace DSN::MPI;

void SSend::operator()(const void* data, int count,
		       const ::MPI::Datatype& type,
		       int destination, int tag) const {
  try {
    Job::getInstance().world().Ssend(data, count, type, destination, tag);
  }

  catch (::MPI::Exception& ex) {
    // TODO(png): log4cpp logging
    std::cerr << __FILE__ << "(" << __LINE__ << "): Caught MPI exception "
	      << "while trying to Ssend() data: "
	      << ex.Get_error_string() << " (error code: "
	      << ex.Get_error_code() << ")" << std::endl;
    throw(ex); // FIXME(png): this sux!
  }
}
