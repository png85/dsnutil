#include <memory>

int main()
{
    try {
        std::unique_ptr<int> foo(new int);
    }

    catch (std::bad_alloc& ex) {
        return 0; // this is OK; the test just has to compile! --png
    }

    return 0;
}
