// Adapted from the "File client.c" section of https://www.man7.org/linux/man-pages/man7/unix.7.html

#ifndef SANDBOX_CPP_MAN7_CLIENT
#define SANDBOX_CPP_MAN7_CLIENT

#include "man7_connection.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

// https://www.man7.org/linux/man-pages/man2/wait.2.html
#include <sys/wait.h>

class man7_client
{
private:
    // https://www.man7.org/linux/man-pages/man2/write.2.html
    // https://www.man7.org/linux/man-pages/man3/write.3p.html
    // Output the string atomically, and don't change errno.
    static void write_client_buffer(const std::string &buffer)
    {
        const int errnum = errno;
        std::string s("Result = ");
        s += buffer;
        man7_connection::write_str(s);
        errno = errnum;
    }

    // Output the string atomically, and don't change errno.
    static void write_argv_str(const std::string &name_of_calling_function, int argc, const char *argv[], int i, ssize_t w, int errnum)
    {
        const int errnum_2 = errno;
        std::string s("write (argc is ");
        s += std::to_string(argc);
        s += " and argv[";
        s += std::to_string(i);
        s += "] is \"";
        s += argv[i];
        s += "\")";
        man7_connection::write_function_results(name_of_calling_function, s, (int)w, errnum);
        errno = errnum_2;
    }

public:
    static int man7_client_main(int argc, const char *argv[], int num_tries_left = 5)
    {
        if (num_tries_left <= 0)
        {
            man7_connection::write_num_tries_left("Whoops. Can't connect.", num_tries_left);

            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            // "ERRORS" section of https://www.man7.org/linux/man-pages/man2/connect.2.html
            errno = ECONNREFUSED;

            return 5;
        }

        int ret;
        int data_socket;
        ssize_t r, w;
        sockaddr_un addr;
        char buffer[BUFFER_SIZE];

        int errnum = 0;

        /* Create local socket. */

        errno = 0;
        data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
        errnum = errno;
        man7_connection::write_function_results(__func__, "socket", data_socket, errnum);

        if (data_socket == -1)
        {
            return 1;
        }

        /*
         * For portability clear the whole structure, since some
         * implementations have additional (nonstandard) fields in
         * the structure.
         */

        memset(&addr, 0, sizeof(addr));

        /* Connect socket to socket address. */

        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
        addr.sun_path[sizeof(addr.sun_path) - 1] = 0;

        errno = 0;
        ret = connect(data_socket, (const sockaddr *)&addr, sizeof(addr));
        errnum = errno;
        man7_connection::write_function_results(__func__, "connect", ret, errnum);

        if (ret == -1)
        {
            // According to https://www.man7.org/linux/man-pages/man2/connect.2.html,
            // "If `connect()` fails, consider the state of the socket as unspecified.
            // Portable applications should close the socket and create a new one for
            // reconnecting."
            man7_connection::close_without_changing_errno(__func__, data_socket);

            num_tries_left--;

            if (num_tries_left > 0)
            {
                man7_connection::write_num_tries_left("Whoops. Failed to connect. Waiting.", num_tries_left);

                sleep(1);

                man7_connection::write_num_tries_left("Let's try reconnecting.", num_tries_left);

                return man7_client_main(argc, argv, num_tries_left);
            }
            else
            {
                man7_connection::write_num_tries_left("Whoops. Can't reconnect.", num_tries_left);

                // https://www.man7.org/linux/man-pages/man3/errno.3.html
                // https://www.man7.org/linux/man-pages/man3/errno.3p.html
                // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
                // "Errors" section of https://www.man7.org/linux/man-pages/man2/connect.2.html
                errno = ECONNREFUSED;

                return 2;
            }
        }

        /* Send arguments. */

        for (int i = 1; i < argc; ++i)
        {
            errno = 0;
            w = write(data_socket, argv[i], strlen(argv[i]) + 1);
            errnum = errno;
            write_argv_str(__func__, argc, argv, i, w, errnum);

            if (w == -1)
            {
                man7_connection::close_without_changing_errno(__func__, data_socket);
                break;
            }
        }

        /* Request result. */

        strncpy(buffer, "END", sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;

        errno = 0;
        w = write(data_socket, buffer, strlen(buffer) + 1);
        errnum = errno;
        man7_connection::write_function_results(__func__, "write (\"END\")", w, errnum);

        if (w == -1)
        {
            man7_connection::close_without_changing_errno(__func__, data_socket);
            return 3;
        }

        /* Receive result. */

        errno = 0;
        r = read(data_socket, buffer, sizeof(buffer));
        errnum = errno;
        man7_connection::write_function_results(__func__, "read", r, errnum);

        if (r == -1)
        {
            man7_connection::close_without_changing_errno(__func__, data_socket);
            return 4;
        }

        /* Ensure buffer is 0-terminated. */

        buffer[sizeof(buffer) - 1] = 0;

        write_client_buffer(buffer);

        /* Close socket. */

        man7_connection::close_without_changing_errno(__func__, data_socket);

        return 0;
    }
};

#endif // SANDBOX_CPP_MAN7_CLIENT
