#ifndef SANDBOX_CPP_PROTOENT_TEST
#define SANDBOX_CPP_PROTOENT_TEST

#include "cpp_protoent.hpp"
#include <iostream>

class cpp_protoent_test
{
public:
    static void test_cpp_protoent()
    {
        cpp_protoent::operator_os(std::cout << '\n') << '\n';
    }
};

#endif // SANDBOX_CPP_PROTOENT_TEST
