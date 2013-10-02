#include <vector>
#include <map>
#include <array>
#include <list>

#include "pretty_print.hh"

int main() {
    std::vector<int> v0;

    dsn::pretty_print::p(v0);

    std::array<double, 3> a0{ {0.1, 0.2, 0.3} };
    dsn::pretty_print::p(a0);

    std::vector<int> v1{1, 2, 3, 4, 5};
    dsn::pretty_print::p(v1);

    std::map<std::string, int> m;
    m["foo"] = 23;
    m["bar"] = 42;
    dsn::pretty_print::p(m);

    std::cout << "---" << std::endl;
    std::cout << m << std::endl
              << v1 << std::endl
              << a0 << std::endl
              << v0 << std::endl;

    std::list<int> l{5, 4, 3, 2, 1};
    std::cout << l << std::endl;

    std::list<std::string> l2{ "foo", "bar", "bla", "baz" };
    std::cout << l2 << std::endl;

    return 0;
}
