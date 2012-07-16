/**
 * $Id$
 *
 * (c) 2011 DAS-SYSTEM Networks
 *
 * \file
 * \brief Networking utility functions
 *
 * \author Peter Hille <peter@das-system-networks.de>
 */

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/** \brief Get in_addr part of a sockaddr
 *
 * This helper function gets the ->in_addr or ->sin6_addr part of a 
 * struct sockaddr depending on the address family.
 */
void* get_in_addr(struct sockaddr* sa) {
  if (sa->sa_family == AF_INET)
    return &(((struct sockaddr_in*)sa)->sin_addr);

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/** \brief Create UDP socket
 *
 * Helper function to create an UDP socket to the given target host and port.
 *
 * \param targetHost Hostname or IP address of the target host
 * \param targetPort Target port number
 * \param pAddrInfo The struct addrinfo belonging to the created socket gets stored here
 *
 * \returns Socket descriptor of the newly created socket or -1 on errors.
 */
int socketUDP(const char* targetHost, int targetPort, struct addrinfo* pAddrInfo) {
  struct addrinfo hints;
  struct addrinfo* servinfo;
  struct addrinfo* p;
  int sockfd = -1;
  int rv;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  char portBuf[32];
  snprintf(portBuf, 32, "%d", targetPort);

  if ((rv = getaddrinfo(targetHost, portBuf, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return -1;
  }

  for (p=servinfo; p!=NULL; p=p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("socket");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "failed to bind socket!\n");
    freeaddrinfo(servinfo);
    return -1;
  }

  memcpy(pAddrInfo, p, sizeof(struct addrinfo));

  freeaddrinfo(servinfo);
  return sockfd;
}

