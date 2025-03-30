#ifndef SANDBOX_MAN7_INET_PTON_EXAMPLE_TEST
#define SANDBOX_MAN7_INET_PTON_EXAMPLE_TEST

#include "man7_inet_pton_example.hpp"

class man7_inet_pton_example_test
{
public:
    // https://www.man7.org/linux/man-pages/man3/inet_pton.3.html
    static void test_man7_inet_pton_example()
    {
        int errnum = 0;

        {
            errno = 0;
            int result = man7_inet_pton_example::man7_inet_pton_example_main(AF_INET, "1.2.3.4");
            errnum = errno;

            man7_connection::write_function_results(__func__, "man7_inet_pton_example_main (ipv4)", result, errnum);
        }
        {
            errno = 0;
            int result = man7_inet_pton_example::man7_inet_pton_example_main(AF_INET6, "1023:4567:89AB:CDEF:2046:8ACE:1357:9BDF");
            errnum = errno;

            man7_connection::write_function_results(__func__, "man7_inet_pton_example_main (ipv6)", result, errnum);
        }
    }
};

#endif // SANDBOX_MAN7_INET_PTON_EXAMPLE_TEST
