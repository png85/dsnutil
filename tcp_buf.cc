#include "tcp_buf.hh"

#include <algorithm>

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>

using namespace DSN::Net;


tcp_buf::tcp_buf() : m_fd(-1), m_cache(EOF) {
}


tcp_buf::~tcp_buf() {
  close();
}


bool tcp_buf::is_open() const {
  return (m_fd != -1);
}


tcp_buf* tcp_buf::connect(const char* host, port_type port) {
  sockaddr_in sa_in = {};
  sa_in.sin_family = AF_INET;
  sa_in.sin_port = htons(port);

  if (const hostent* entry = gethostbyname(host)) {
    memcpy(&sa_in.sin_addr.s_addr, entry->h_addr, entry->h_length);
  } else if ((sa_in.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE) {
    return 0;
  }

  m_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (m_fd == -1)
    return 0;

  if (::connect(m_fd,
		reinterpret_cast<const sockaddr*>(&sa_in), sizeof(sa_in)) == -1) {
    close();
    return 0;
  }

  return this;
}


tcp_buf* tcp_buf::close() {
  if (m_fd == -1 || ::close(m_fd) == -1)
    return 0;

  m_fd = -1;

  return this;
}


tcp_buf::int_type tcp_buf::underflow() {
  if (m_buffer.empty()) {
    char buf[4096];
    int len = recv(m_fd, buf, sizeof(buf), 0);
    m_buffer.insert(m_buffer.end(), buf, buf + len);
  }

  return (!m_buffer.empty()) ? m_buffer.front() : EOF;
}


tcp_buf::int_type tcp_buf::uflow() {
  int_type c = underflow();
  m_cache = c;
  return (!m_buffer.empty()) ? m_buffer.front() : EOF;
}


tcp_buf::int_type tcp_buf::overflow(int i) {
  if (i == EOF)
    return EOF;

  unsigned char c = static_cast<unsigned char>(i);

  return (send(m_fd, &c, 1, 0) == 1) ? i : EOF;
}


std::streamsize tcp_buf::xsgetn(char* s, std::streamsize n) {
  unsigned sz = m_buffer.size();
  std::streamsize len = std::min(sz, unsigned(n));
  if (len) {
    copy(m_buffer.begin(), m_buffer.begin() + len, s);
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + len);
  }

  while (len < n) {
    std::streamsize delta = recv(m_fd, s + len, n - len, 0);
    if (delta < 1)
      break;
    len += delta;
  }

  return len;
}


std::streamsize tcp_buf::xsputn(const char* s, std::streamsize n) {
  return send(m_fd, s, n, 0);
}


tcp_buf::int_type tcp_buf::pbackfail(int_type c) {
  if (c == EOF) std::swap(m_cache, c);
  if (c != EOF) m_buffer.push_front(c);

  return c;
}

