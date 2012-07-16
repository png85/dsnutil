#include <iostream>
#include <cstdlib>

#include "netutil.h"

int main(int argc, char** argv) {
  std::cout << "libdsnutil/netutil test" << std::endl
	    << "(c) 2012 #das-system Networks" << std::endl;

  if (argc != 2) {
    std::cout << "Syntax: " << argv[0] << " [1|2]" << std::endl;
    return EXIT_FAILURE;
  }

  int test_mode = atoi(argv[1]);
  switch(test_mode) {
  case 1:
    std::cout << "netutil test #1: get_in_addr():" << std::endl;
    std::cout << "[X] TODO! --png" << std::endl;
    return EXIT_SUCCESS;
    break;
  case 2:
    std::cout << "netutil test #2: socketUDP():" << std::endl;
    {
      struct addrinfo ai;
      int sockfd;
      if ((sockfd = socketUDP("das-system-networks.de", 31337, &ai)) == -1) {
	std::cout << "[X] Error creating UDP socket to "
		  << "das-system-networks.de:31337" << std::endl;
	return EXIT_FAILURE;
      }

      std::cout << "[i] created socket to connect to "
		<< "das-system-networks.de:31337/udp";

      close(sockfd);
    }
    return EXIT_SUCCESS;
    break;
  default:
    std::cout << "Invalid test_mode specified!" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Test succeeded :-)" << std::endl;

  return EXIT_SUCCESS;
}
