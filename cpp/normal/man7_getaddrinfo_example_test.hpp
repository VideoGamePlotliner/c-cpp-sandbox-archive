#ifndef SANDBOX_MAN7_GETADDRINFO_EXAMPLE_TEST
#define SANDBOX_MAN7_GETADDRINFO_EXAMPLE_TEST

#include "man7_getaddrinfo_example_client.hpp"
#include "man7_getaddrinfo_example_server.hpp"
#include "cpp_waitpid.hpp"

class man7_getaddrinfo_example_test
{
private:
    // https://www.man7.org/linux/man-pages/man2/fork.2.html
    // https://www.man7.org/linux/man-pages/man2/wait.2.html
    // Based on man7_test::man7_client_main_fork()
    static int man7_getaddrinfo_example_client_main_fork(int argc, const char *argv[])
    {
        int errnum = 0;

        errno = 0;
        const pid_t pid_client = fork();
        errnum = errno;

        if (pid_client < 0)
        {
            man7_connection::write_function_results(__func__, "fork (error)", pid_client, errnum);
            return 2001;
        }
        else if (pid_client == 0)
        {
            man7_connection::write_function_results(__func__, "fork (child process)", pid_client, errnum);

            errno = 0;
            int client_result = man7_getaddrinfo_example_client::man7_getaddrinfo_example_client_main(argc, argv);
            errnum = errno;
            man7_connection::write_function_results(__func__, "man7_getaddrinfo_example_client_main", client_result, errnum);

            exit(client_result);
        }
        else
        {
            man7_connection::write_function_results(__func__, "fork (parent process)", pid_client, errnum);

            errno = 0;
            int waitpid_special_result = cpp_waitpid::waitpid_special(pid_client);
            errnum = errno;

            man7_connection::write_function_results(__func__, "waitpid_special", waitpid_special_result, errnum);

            return waitpid_special_result;
        }
    }

public:
    // https://www.man7.org/linux/man-pages/man2/fork.2.html
    // https://www.man7.org/linux/man-pages/man2/wait.2.html
    // Based on man7_test::test_man7()
    static void test_man7_getaddrinfo_example(const char *host)
    {
        int errnum = 0;

        errno = 0;
        const pid_t pid_server = fork();
        errnum = errno;

        if (pid_server < 0)
        {
            man7_connection::write_function_results(__func__, "fork (error)", pid_server, errnum);
        }
        else if (pid_server == 0)
        {
            man7_connection::write_function_results(__func__, "fork (child process)", pid_server, errnum);

            int argc_server = 2;
            const char *argv_server[2] = {
                "./getaddrinfo_example_server",

                // Refer to LOWER_BOUND_FOR_LOCAL_IP_PORT
                "4097"};

            errno = 0;
            int server_result = man7_getaddrinfo_example_server::man7_getaddrinfo_example_server_main(argc_server, argv_server);
            errnum = errno;
            man7_connection::write_function_results(__func__, "man7_getaddrinfo_example_server_main", server_result, errnum);

            exit(server_result);
        }
        else
        {
            man7_connection::write_function_results(__func__, "fork (parent process)", pid_server, errnum);

            man7_connection::write_str("Waiting to start man7_getaddrinfo_example_client_main_fork.");
            sleep(1);

            const char *argv_client[] = {
                "./getaddrinfo_example_client",
                host,

                // Refer to LOWER_BOUND_FOR_LOCAL_IP_PORT
                "4097",

                // What to send
                "pawn",
                "rook",
                "bishop",
                "knight"};

            // https://en.cppreference.com/w/c/language/sizeof
            // "Number of elements in any array a including VLA(since C99) may be determined with the expression sizeof a / sizeof a[0]."
            const int argc_client = sizeof(argv_client) / sizeof(argv_client[0]);

            errno = 0;
            int client_result = man7_getaddrinfo_example_client_main_fork(argc_client, argv_client);
            errnum = errno;

            man7_connection::write_function_results(__func__, "man7_getaddrinfo_example_client_main_fork", client_result, errnum);

            errno = 0;
            int waitpid_special_result = cpp_waitpid::waitpid_special(pid_server);
            errnum = errno;

            man7_connection::write_function_results(__func__, "waitpid_special", waitpid_special_result, errnum);
        }
    }
};

#endif // SANDBOX_MAN7_GETADDRINFO_EXAMPLE_TEST
