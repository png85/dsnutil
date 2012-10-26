#ifndef DSNUTIL_TCP_STREAM_HH
#define DSNUTIL_TCP_STREAM_HH 1

#include <iostream>

namespace DSN {
  namespace Net {
    typedef unsigned short port_type;
    
    class tcp_stream : public std::iostream {
    public:
      tcp_stream();
      tcp_stream(const char* host, port_type port);
      tcp_stream& connect(const char* host, port_type port);
      void close();

    };
  };
};

#endif // !DSNUTIL_TCP_STREAM_HH
