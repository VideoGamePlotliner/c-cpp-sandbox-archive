#include "man7_getaddrinfo_example_test.hpp"
#include "cpp_ip_loopback.hpp"

int main()
{
    man7_getaddrinfo_example_test::test_man7_getaddrinfo_example(IPv4_LOOPBACK_STRING_LITERAL);
    man7_getaddrinfo_example_test::test_man7_getaddrinfo_example(IPv6_LOOPBACK_STRING_LITERAL);
    return 0;
}
