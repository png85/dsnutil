/**
 * $Id$
 *
 * (c) 2011 DAS-SYSTEM Networks
 *
 * \file
 * \brief Prototypes for network utility code.
 *
 * \author Peter Hille <peter@das-system-networks.de>
 */
#ifndef NETUTIL_H
#define NETUTIL_H 1

#include <netdb.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

  void* get_in_addr(struct sockaddr* sa);

  int socketUDP(const char* targetHost, int targetPort, struct addrinfo* pAddrInfo);

#ifdef __cplusplus
};
#endif

#endif /* !NETUTIL_H */
