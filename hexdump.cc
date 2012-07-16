/// $Id$
///
/// (c) 2011 DAS-SYSTEM Networks
///
/// \file
/// \brief Hexdump utility function
///
/// This file contains a function to hex-dump a given memory area.
///
/// \author Peter Hille <peter@das-system-networks.de>
///

#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "hexdump.hh"

/// \brief Print hex dump of a memory area.
///
/// This pretty-prints a hex dump of the given memory area and size to an
/// output stream. The output of this is roughly similar to 
/// 'hexdump -C file' on UNIX systems.
///
/// \param mem Pointer to memory area to be dumped
/// \param length Number of bytes to dump from mem
/// \param out output stream to which the hexdump gets written
void hexdump(const void* mem, size_t length, std::ostream& out) {
  char line[80];
  const char* src = static_cast<const char*>(mem);

  for(unsigned int i=0; i<length; i+=16, src+=16) {
    char* t = line;
    // each line begins with the offset...
    t+= sprintf(t, "%04x: ", i);

    // ... then we print the data in hex byte-wise...
    for(int j=0; j<16; j++) {
      if(i+j < length)
	t+= sprintf(t, "%02X", src[j] & 0xff);
      else
	t += sprintf(t, "  ");

      // print a tabulator after the first group of 8 bytes
      if (j == 7)
	t += sprintf(t, "\t");
      else
	t += sprintf(t, " ");
    }

    // ... and finally we display all printable characters
    t+= sprintf(t, "|");
    for(int j=0; j<16; j++) {
      if(i+j < length) {
	if (isprint((unsigned char)src[j]))
	  t += sprintf(t, "%c", src[j]);
	else
	  t += sprintf(t, ".");
      } else {
	t+= sprintf(t, " ");
      }
    }

    t += sprintf(t, "|\n");
    out << line;
  }
}
