/** \file
 *
 * \brief Stream interceptor class
 */
#ifndef INTERCEPT_STREAM_HH
#define INTERCEPT_STREAM_HH 1

#include <iostream>
#include <memory>

#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>

namespace DSN {
  /** \brief Interceptor for std::ostream instances
   *
   * This class can be used to intercept all writes to an std::ostream instance
   * and re-route them through log4cpp.
   *
   * \version 1.0
   *
   * \author Peter Hille (png!das-system) <peter@das-system-networks.de>
   */
  class intercept_stream : public std::streambuf {
  public:
    /** \brief Default constructor
     *
     * \param stream std::ostream instance to intercept
     * \param priority Priority for all messages to this stream
     * \param log log4cpp::Category to copy logging settings
     * \param streamName Name of the intercepted stream in human readable form
     */
    intercept_stream(std::ostream& stream, log4cpp::Priority::Value priority,
		     log4cpp::Category& log, const char* streamName=0);
    ~intercept_stream();

  protected:
    virtual std::streamsize xsputn(const char* msg, std::streamsize count);
    virtual std::streambuf::int_type overflow(std::streambuf::int_type c);
    virtual int sync();

  private:
    /// \brief Buffer of intercepted stream
    std::streambuf* m_orgbuf;

    /// \brief Reference to original stream object
    std::ostream& m_orgstream;

    /// \brief Newly allocated output stream
    std::auto_ptr<std::ostream> m_newstream;

    /// \brief Stream name for log4cpp output
    std::string m_streamName;

    log4cpp::Category& m_log;
    log4cpp::Priority::Value m_priority;
  };
};

#endif // !INTERCEPT_STREAM_HH
