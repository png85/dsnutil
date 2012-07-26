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
