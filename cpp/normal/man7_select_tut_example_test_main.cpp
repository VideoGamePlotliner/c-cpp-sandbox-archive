#include "man7_select_tut_example_test.hpp"
#include "cpp_ip_loopback.hpp"

int main()
{
    man7_select_tut_example_test::test_man7_select_tut_example(IPv4_LOOPBACK_STRING_LITERAL);
    man7_select_tut_example_test::test_man7_select_tut_example(IPv6_LOOPBACK_STRING_LITERAL);
    return 0;
}
