#include <iostream>
#include <string>

#include "map.hh"

int main(void) {
    std::map<std::string, float> stocks = {
        std::pair<std::string, float>("YHOO", 5.0f),
        std::pair<std::string, float>("MSFT", 7.5f),
        std::pair<std::string, float>("GOOG", 9.9f),
        std::pair<std::string, float>("DSN ", 13.37f),
        std::pair<std::string, float>("IBM ", 6.0f),
        std::pair<std::string, float>("ACME", 1.5)
    };

    std::function<void (std::pair<std::string, float>)> map_dump = [&](std::pair<std::string, float> p) {
        std::cout << "stocks['" << p.first << "'] = " << p.second << std::endl;
    };

    std::function<void (std::string)> vector_dump = [&](std::string k) {
        std::cout << "stocks['" << k << "'] = " << stocks[k] << std::endl;
    };

    std::cout << "Unsorted map at start:" << std::endl;
    std::for_each(stocks.begin(), stocks.end(), map_dump);
    std::cout << std::endl;


    // extract map keys into existing vector
    std::cout << "Extracting map keys(1):" << std::endl;
    std::vector<std::string> k1;
    DSN::map_keys(stocks, k1);
    std::for_each(k1.begin(), k1.end(), [&](std::string s) { std::cout << s << std::endl; });


    // extract map keys into new vector
    std::cout << "Extracting map keys(2):" << std::endl;
    std::vector<std::string> k2 = DSN::map_keys(stocks);
    std::for_each(k2.begin(), k2.end(), [&](std::string s) { std::cout << s << std::endl; });
    std::cout << std::endl;


    // sort map keys into existing vector
    std::cout << "Sorting keys in ascending order(1):" << std::endl;
    std::vector<std::string> k3;
    DSN::map_sort(stocks, k3);
    std::for_each(k3.begin(), k3.end(), vector_dump);


    // sort map keys into new vector
    std::cout << "Sorting keys in ascending order(2):" << std::endl;
    std::vector<std::string> k4 = DSN::map_sort(stocks);
    std::for_each(k4.begin(), k4.end(), vector_dump);


    std::function<bool (std::string, std::string)> desc_sort = [&](std::string a, std::string b) {
        return (stocks[a] > stocks[b]);
    };

    // sort map keys into existing vector w/ custom comparator
    std::cout << "Sorting keys in descending order(1):" << std::endl;
    std::vector<std::string> k5;
    DSN::map_sort(stocks, k5, desc_sort);
    std::for_each(k5.begin(), k5.end(), vector_dump);


    // sort map keys into new vector w/ custom comparator
    std::cout << "Sorting keys in descending order(2):" << std::endl;
    std::vector<std::string> k6 = DSN::map_sort(stocks, desc_sort);
    std::for_each(k6.begin(), k6.end(), vector_dump);


    return 0;
}
