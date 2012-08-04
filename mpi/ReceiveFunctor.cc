/** \file
 *
 * \brief Implementation of DSN::MPI::ReceiveFunctor
 *
 * This file implements the DSN::MPI::ReceiveFunctor class which is a general
 * functor to receive p2p communication data in a message passing environment.
 *
 * \author Peter Hille (png!das-system) <peter@das-system-networks.de>
 *
 * \version 1.0 Initial release
 */
#include "mpi.hh"

using namespace DSN::MPI;

void ReceiveFunctor::operator()(void* data, int count,
				const ::MPI::Datatype& type,
				int source, int tag,
				::MPI::Status& status) const {
  try {
    Job::getInstance().world().Recv(data, count,
				    type, source, tag, status);
  }
    
  catch (::MPI::Exception& ex) {
    // TODO(png): log4cpp logging
    std::cerr << __FILE__ << "(" << __LINE__ << "): Caught MPI exception "
	      << "while trying to recv() data: "
	      << ex.Get_error_string() << " (error code: "
	      << ex.Get_error_code() << ")" << std::endl;
  }
}
