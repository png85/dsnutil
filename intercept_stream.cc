/** \file
 *
 * \brief Stream interceptor class
 */
#include <cstdlib>
#include <sstream>

#include "intercept_stream.hh"

using namespace DSN;

intercept_stream::intercept_stream(std::ostream& stream,
				   log4cpp::Priority::Value priority,
				   log4cpp::Category& log,
				   const char* streamName) :
  m_orgstream(stream), m_streamName("unnamed"),
  m_log(log4cpp::Category::getInstance("DSN::intercept_stream")),
  m_priority(priority)
{
  // swap the old buffer in iostream with this buffer
  m_orgbuf = m_orgstream.rdbuf(this);

  try {
    // create a new ostream that we set the old buffer in
    m_newstream.reset(new std::ostream(m_orgbuf));
  }

  catch (std::bad_alloc& ex) {
    log << log4cpp::Priority::FATAL << __FILE__ << "(" << __LINE__ << "): "
	<< "Unable to allocate memory for stream interceptor: "
	<< ex.what();
  }

  if (streamName) {
    m_streamName = streamName;
  }

  // copy appenders and priority from parent log4cpp::Category instance
  log4cpp::AppenderSet appenders = log.getAllAppenders();
  for (log4cpp::AppenderSet::iterator i=appenders.begin(); i!=appenders.end(); ++i) {
    log4cpp::Appender& appender = *(*i);
    m_log.addAppender(appender);
  }
  m_log.setPriority(log.getPriority());
}

intercept_stream::~intercept_stream() {
  // restore old buffer
  m_orgstream.rdbuf(m_orgbuf);

  // release memory for new stream
  m_newstream.reset();

  // release log4cpp appenders
  m_log.removeAllAppenders();
}


std::streamsize intercept_stream::xsputn(const char* msg,
					 std::streamsize count) {
  // output to new stream with old buffer
  m_newstream->write(msg, count);

  m_log << m_priority << "<" << m_streamName << "> " << msg;

  return count;
}

int intercept_stream::sync() {
  return 0;
}

std::streambuf::int_type intercept_stream::overflow(std::streambuf::int_type c) {
  if (!std::streambuf::traits_type::eq_int_type(c, std::streambuf::traits_type::eof())) {
    std::streambuf::char_type const t =
      std::streambuf::traits_type::to_char_type(c);
    this->xsputn(&t, 1);
  }

  return !traits_type::eof();
}
