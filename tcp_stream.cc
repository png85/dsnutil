#include "tcp_stream.hh"
#include "tcp_buf.hh"

using namespace DSN::Net;

tcp_stream::tcp_stream() : std::iostream(new tcp_buf()) {
}


tcp_stream::tcp_stream(const char* host, port_type port) : std::iostream(new tcp_buf()) {
  static_cast<tcp_buf*>(rdbuf())->connect(host, port);
}


tcp_stream& tcp_stream::connect(const char* host, port_type port) {
  static_cast<tcp_buf*>(rdbuf())->connect(host, port);
  return *this;
}


void tcp_stream::close() {
  static_cast<tcp_buf*>(rdbuf())->close();
}
