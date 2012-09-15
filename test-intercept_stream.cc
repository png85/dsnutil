#include <cstdlib>

#include <log4cpp/OstreamAppender.hh>

#include "intercept_stream.hh"

int main(int argc, char** argv) {
  log4cpp::Category& log(log4cpp::Category::getInstance("test-intercept_stream"));
  log.setPriority(log4cpp::Priority::DEBUG);
  try {
    log4cpp::Appender* a = new log4cpp::OstreamAppender("test-intercept_stream/stdout", &std::cerr);
    a->setLayout(new log4cpp::BasicLayout());
    log.addAppender(a);
  }

  catch (std::bad_alloc& ex) {
    std::cerr << "Unable to initialize log4cpp: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  log << log4cpp::Priority::DEBUG << "log4cpp target initialized!";

  std::cout << "this will go to normal stdout" << std::endl;

  log << log4cpp::Priority::INFO << "Installing stream interceptor for std::cout";
  std::auto_ptr<DSN::intercept_stream> interceptor(new DSN::intercept_stream(std::cout,
									     log4cpp::Priority::DEBUG, log,
									     "cout"));
  std::cout << "this goes to intercepted cout" << std::endl;
  interceptor.reset();

  log << log4cpp::Priority::INFO << "Removed stream interceptor for std::cout";
  std::cout << "this goes to normal stdout again" << std::endl;

  log4cpp::Category::shutdown();
  return EXIT_SUCCESS;
}
