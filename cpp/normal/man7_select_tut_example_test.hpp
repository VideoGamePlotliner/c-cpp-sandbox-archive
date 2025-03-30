#ifndef SANDBOX_MAN7_SELECT_TUT_EXAMPLE_TEST
#define SANDBOX_MAN7_SELECT_TUT_EXAMPLE_TEST

#include "man7_select_tut_example.hpp"

class man7_select_tut_example_test
{
public:
    static void test_man7_select_tut_example(const char *host)
    {
        int errnum = 0;

        const char *argv[] = {"./select_tut_example",

                              // Refer to LOWER_BOUND_FOR_LOCAL_IP_PORT
                              "4097",

                              // Refer to LOWER_BOUND_FOR_LOCAL_IP_PORT
                              "4097",

                              host};

        // https://en.cppreference.com/w/c/language/sizeof
        // "Number of elements in any array a including VLA(since C99) may be determined with the expression sizeof a / sizeof a[0]."
        const int argc = sizeof(argv) / sizeof(argv[0]);

        man7_select_tut_example example;

        errno = 0;
        const int result = example.man7_select_tut_example_main(argc, argv);
        errnum = errno;

        man7_connection::write_function_results(__func__, "man7_select_tut_example_main", result, errnum);
    }
};

#endif // SANDBOX_MAN7_SELECT_TUT_EXAMPLE_TEST
