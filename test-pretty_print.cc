#include <vector>
#include <map>
#include <array>
#include <list>

#include <sstream>

#include "config.h"

#ifdef LOG4CPP_FOUND
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/BasicLayout.hh>
#endif

#include "pretty_print.hh"

int main() {
    std::vector<int> v0;
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::array<double, 3> a0{ {0.1, 0.2, 0.3} };
    std::map<std::string, int> m;
    m["foo"] = 23;
    m["bar"] = 42;
    std::list<int> l{5, 4, 3, 2, 1};
    std::list<std::string> l2{ "foo", "bar", "bla", "baz" };

    std::cout << "Testing pretty printing on std::cout:" << std::endl;
    std::cout << "Empty vector: " << v0 << std::endl
              << "Vector      : " << v1 << std::endl
              << "Array       : " << a0 << std::endl
              << "Map         : " << m << std::endl
              << "list<int>   : " << l << std::endl
              << "list<string>: " << l2 << std::endl;

    std::stringstream ss;
    ss << "Empty vector: " << v0 << std::endl
       << "Vector      : " << v1 << std::endl
       << "Array       : " << a0 << std::endl
       << "Map         : " << m << std::endl
       << "list<int>   : " << l << std::endl
       << "list<string>: " << l2 << std::endl;
    std::cout << std::endl << "Testing pretty printing into std::stringstream:" << std::endl << ss.str();

#ifdef LOG4CPP_FOUND
    std::cout << std::endl << "Testing pretty printing on log4cpp streams:" << std::endl;
    log4cpp::Category& log = log4cpp::Category::getInstance("test-pretty_print");
    log.setPriority(log4cpp::Priority::DEBUG);

    try {
        log4cpp::Appender* appender = new log4cpp::OstreamAppender("test-pretty_print", &std::cout);
        appender->setLayout(new log4cpp::BasicLayout());
        log.addAppender(appender);
    }
    catch (std::bad_alloc& ex) {
        std::cerr << "Failed to allocate memory for log4cpp objects: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }


    log << log4cpp::Priority::DEBUG << "Empty vector: " << v0;
    log << log4cpp::Priority::DEBUG << "Vector      : " << v1;
    log << log4cpp::Priority::DEBUG << "Array       : " << a0;
    log << log4cpp::Priority::DEBUG << "Map         : " << m;
    log << log4cpp::Priority::DEBUG << "list<int>   : " << l;
    log << log4cpp::Priority::DEBUG << "list<string>: " << l2;

    log4cpp::Category::shutdown();
#endif

    return 0;
}
