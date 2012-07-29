#include <assert.h>

#include "mpi.hh"

using namespace DSN::MPI;

void BSend::operator()(const void* data, int count,
		       const ::MPI::Datatype& type,
		       int destination, int tag) const {
  assert(data != NULL);

  // calculate MPI buffer size for Bsend
  int bufSize =
    type.Pack_size(count, Job::getInstance().world()) + ::MPI::BSEND_OVERHEAD;
  char* buf = NULL;

  // allocate buffer memory for MPI
  try {
    buf = new char[bufSize];
  }

  catch (std::bad_alloc& ex) {
    // TODO(png): log4cpp based logging;
    std::cerr << __FILE__ << "(" << __LINE__ << "): Unable to allocate "
	      << bufSize << " bytes for transmission buffer: " << ex.what()
	      << std::endl;
    throw(ex); // FIXME(png): this sux!
  }

  /* note: this conversion to void* is required because MPI's Bsend()
   * expects a reference to a void* but delete[] on void pointers is
   * undefined and emits a warning in gcc -- png
   */
  void* pBuffer = static_cast<void*>(buf);

  // Transmit our data using the previously allocated buffer
  ::MPI::Attach_buffer(pBuffer, bufSize);
  try {
    Job::getInstance().world().Bsend(data, count, type, destination, tag);
  }
  catch (::MPI::Exception& ex) {
    // TODO(png): log4cpp logging!
    std::cerr << __FILE__ << "(" << __LINE__ << "): Caught MPI exception "
	      << "while trying to Bsend() data: "
	      << ex.Get_error_string() << " (error code: "
	      << ex.Get_error_code() << ")" << std::endl;

    // try to clean up buffer memory before rethrowing exception
    ::MPI::Detach_buffer(pBuffer);
    delete[] buf;

    throw(ex); // FIXME(png): this sux! 
  }
  ::MPI::Detach_buffer(pBuffer);


  delete[] buf;
}

