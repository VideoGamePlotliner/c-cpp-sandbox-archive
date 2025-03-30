#ifndef SANDBOX_MAN7_TEST
#define SANDBOX_MAN7_TEST

#include "man7_client.hpp"
#include "man7_server.hpp"
#include "cpp_waitpid.hpp"

class man7_test
{
private:
    // https://www.man7.org/linux/man-pages/man2/fork.2.html
    // https://www.man7.org/linux/man-pages/man2/wait.2.html
    static int man7_client_main_fork(int argc, const char *argv[])
    {
        int errnum = 0;

        errno = 0;
        const pid_t pid_client = fork();
        errnum = errno;

        if (pid_client < 0)
        {
            man7_connection::write_function_results(__func__, "fork (error)", pid_client, errnum);
            return 2000;
        }
        else if (pid_client == 0)
        {
            man7_connection::write_function_results(__func__, "fork (child process)", pid_client, errnum);

            errno = 0;
            int client_result = man7_client::man7_client_main(argc, argv);
            errnum = errno;
            man7_connection::write_function_results(__func__, "man7_client_main", client_result, errnum);

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

    // Adapted from the "Example output" section of https://www.man7.org/linux/man-pages/man7/unix.7.html
    static int man7_client_main_fork_call_1()
    {
        int argc = 3;
        const char *argv[3] = {"./client", "3", "4"};

        return man7_client_main_fork(argc, argv);
    }

    // Adapted from the "Example output" section of https://www.man7.org/linux/man-pages/man7/unix.7.html
    static int man7_client_main_fork_call_2()
    {
        int argc = 3;
        const char *argv[3] = {"./client", "11", "-5"};

        return man7_client_main_fork(argc, argv);
    }

    // Adapted from the "Example output" section of https://www.man7.org/linux/man-pages/man7/unix.7.html
    static int man7_client_main_fork_call_3()
    {
        int argc = 2;
        const char *argv[2] = {"./client", "DOWN"};

        return man7_client_main_fork(argc, argv);
    }

public:
    // Adapted from the "Example output" section of https://www.man7.org/linux/man-pages/man7/unix.7.html
    // https://www.man7.org/linux/man-pages/man2/fork.2.html
    // https://www.man7.org/linux/man-pages/man2/wait.2.html
    static void test_man7()
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

            errno = 0;
            int server_result = man7_server::man7_server_main();
            errnum = errno;
            man7_connection::write_function_results(__func__, "man7_server_main", server_result, errnum);

            exit(server_result);
        }
        else
        {
            man7_connection::write_function_results(__func__, "fork (parent process)", pid_server, errnum);

            errno = 0;
            int call_1_result = man7_client_main_fork_call_1();
            errnum = errno;

            man7_connection::write_function_results(__func__, "man7_client_main_fork_call_1", call_1_result, errnum);

            errno = 0;
            int call_2_result = man7_client_main_fork_call_2();
            errnum = errno;

            man7_connection::write_function_results(__func__, "man7_client_main_fork_call_2", call_2_result, errnum);

            errno = 0;
            int call_3_result = man7_client_main_fork_call_3();
            errnum = errno;

            man7_connection::write_function_results(__func__, "man7_client_main_fork_call_3", call_3_result, errnum);

            errno = 0;
            int waitpid_special_result = cpp_waitpid::waitpid_special(pid_server);
            errnum = errno;

            man7_connection::write_function_results(__func__, "waitpid_special", waitpid_special_result, errnum);
        }
    }
};

#endif // SANDBOX_MAN7_TEST
