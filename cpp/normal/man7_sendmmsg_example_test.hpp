#ifndef SANDBOX_MAN7_SENDMMSG_EXAMPLE_TEST
#define SANDBOX_MAN7_SENDMMSG_EXAMPLE_TEST

#include "man7_sendmmsg_example.hpp"

class man7_sendmmsg_example_test
{
public:
    static void test_man7_sendmmsg_example()
    {
        int errnum = 0;

        errno = 0;
        int result = man7_sendmmsg_example::man7_sendmmsg_example_main();
        errnum = errno;

        man7_connection::write_function_results(__func__, "man7_sendmmsg_example_main", result, errnum);
    }
};

#endif // SANDBOX_MAN7_SENDMMSG_EXAMPLE_TEST
