#include "tcp_stream.hh"
#include <iostream>
#include <string>
#include <cstdlib>

int main() {
  const char* hostname = "www.kernel.org";
  const DSN::Net::port_type port = 80;
  const char* page = "/";

  std::cout << "Creating tcp_stream to " << hostname << ":" << port << "..." << std::endl;
  DSN::Net::tcp_stream ios(hostname, port);
  if (!ios.is_open()) {
    std::cout << "Failed to create tcp_stream..." << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Sending HTTP GET request..." << std::endl;
  ios << "GET " << page << " HTTP/1.0\n\n" << std::flush;

  std::cout << "Trying to read response..." << std::endl;
  std::string s;
  while(std::getline(ios, s))
    std::cout << s << std::endl;

  return EXIT_SUCCESS;
}
