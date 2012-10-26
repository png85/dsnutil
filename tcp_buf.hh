#ifndef DSNUTIL_TCP_BUF_HH
#define DSNUTIL_TCP_BUF_HH 1

#include <iostream>
#include <iosfwd>
#include <deque>

#include <cstdio>

namespace DSN {
  namespace Net {
    typedef unsigned short port_type;

    class tcp_buf : public std::streambuf {
    public:
      typedef char char_type;
      typedef int int_type;
      typedef unsigned int pos_type;
      typedef int off_type;

      tcp_buf();
      ~tcp_buf();

      bool is_open() const;
      tcp_buf* connect(const char* host, port_type port);
      tcp_buf* close();

    protected:
      virtual int_type underflow();
      virtual int_type uflow();
      virtual int_type overflow(int_type c = EOF);
      virtual int_type pbackfail(int_type c = EOF);
      virtual std::streamsize xsgetn(char* s, std::streamsize n);
      virtual std::streamsize xsputn(const char* s, std::streamsize n);

    private:
      int m_fd;
      int_type m_cache;
      std::deque<unsigned char> m_buffer;
    };
  };
};

#endif
