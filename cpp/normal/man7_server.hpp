// Adapted from the "File server.c" section of https://www.man7.org/linux/man-pages/man7/unix.7.html

#ifndef SANDBOX_CPP_MAN7_SERVER
#define SANDBOX_CPP_MAN7_SERVER

#include "man7_connection.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>
#include <stdexcept>

class man7_server
{
private:
    // https://www.man7.org/linux/man-pages/man3/unlink.3p.html
    // Don't change errno.
    static void unlink_without_changing_errno(const char *path)
    {
        const int errnum = errno;
        unlink(path);
        errno = errnum;
    }

public:
    static int man7_server_main(void)
    {
        int down_flag = 0;
        int ret;
        int connection_socket;
        int data_socket;
        int result;
        ssize_t r, w;
        sockaddr_un name;
        char buffer[BUFFER_SIZE];

        int errnum = 0;

        /* Create local socket. */

        errno = 0;
        connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
        errnum = errno;
        man7_connection::write_function_results(__func__, "socket", connection_socket, errnum);

        if (connection_socket == -1)
        {
            // ACCORDING TO THE "Notes" SECTION OF
            // https://www.man7.org/linux/man-pages/man7/unix.7.html
            // USE unlink() AFTER bind(), NOT BEFORE IT.
            return 1;
        }

        /*
         * For portability clear the whole structure, since some
         * implementations have additional (nonstandard) fields in
         * the structure.
         */

        memset(&name, 0, sizeof(name));

        /* Bind socket to socket name. */

        name.sun_family = AF_UNIX;
        strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
        name.sun_path[sizeof(name.sun_path) - 1] = 0;

        errno = 0;
        ret = bind(connection_socket, (const sockaddr *)&name, sizeof(name));
        errnum = errno;
        man7_connection::write_function_results(__func__, "bind", ret, errnum);

        if (ret == -1)
        {
            man7_connection::close_without_changing_errno(__func__, connection_socket);
            unlink_without_changing_errno(SOCKET_NAME);
            return 2;
        }

        /*
         * Prepare for accepting connections. The backlog size is set
         * to 20. So while one request is being processed other requests
         * can be waiting.
         */

        errno = 0;
        ret = listen(connection_socket, 20);
        errnum = errno;
        man7_connection::write_function_results(__func__, "listen", ret, errnum);

        if (ret == -1)
        {
            man7_connection::close_without_changing_errno(__func__, connection_socket);
            unlink_without_changing_errno(SOCKET_NAME);
            return 3;
        }

        /* This is the main loop for handling connections. */

        for (;;)
        {

            /* Wait for incoming connection. */

            errno = 0;
            data_socket = accept(connection_socket, NULL, NULL);
            errnum = errno;
            man7_connection::write_function_results(__func__, "accept", data_socket, errnum);

            if (data_socket == -1)
            {
                man7_connection::close_without_changing_errno(__func__, connection_socket);
                unlink_without_changing_errno(SOCKET_NAME);
                return 4;
            }

            result = 0;
            for (;;)
            {

                /* Wait for next data packet. */

                errno = 0;
                r = read(data_socket, buffer, sizeof(buffer));
                errnum = errno;
                man7_connection::write_function_results(__func__, "read", r, errnum);

                if (r == -1)
                {
                    man7_connection::close_without_changing_errno(__func__, data_socket);
                    man7_connection::close_without_changing_errno(__func__, connection_socket);
                    unlink_without_changing_errno(SOCKET_NAME);
                    return 5;
                }

                /* Ensure buffer is 0-terminated. */

                buffer[sizeof(buffer) - 1] = 0;

                /* Handle commands. */

                if (!strncmp(buffer, "DOWN", sizeof(buffer)))
                {
                    down_flag = 1;
                    continue;
                }

                if (!strncmp(buffer, "END", sizeof(buffer)))
                {
                    break;
                }

                if (down_flag)
                {
                    continue;
                }

                // https://www.man7.org/linux/man-pages/man3/atoi.3.html
                // https://www.man7.org/linux/man-pages/man3/atoi.3p.html
                // https://www.man7.org/linux/man-pages/man3/strtol.3.html
                // https://www.man7.org/linux/man-pages/man3/strtol.3p.html
                // https://en.cppreference.com/w/cpp/string/basic_string/stol
                // https://en.cppreference.com/w/cpp/language/catch
                // https://en.cppreference.com/w/cpp/error/out_of_range
                // https://en.cppreference.com/w/cpp/error/invalid_argument
                int int_value = 0;
                errno = 0;
                try
                {
                    int_value = std::stoi(buffer);
                }
                catch (const std::out_of_range &)
                {
                    if (!errno)
                    {
                        errno = ERANGE;
                    }
                }
                catch (...)
                {
                    if (!errno)
                    {
                        errno = EINVAL;
                    }
                }
                errnum = errno;
                man7_connection::write_function_results(__func__, "std::stoi", int_value, errnum);

                /* Add received summand. */

                result += int_value;
            }

            /* Send result. */

            snprintf(buffer, sizeof(buffer), "%d", result);
            buffer[sizeof(buffer) - 1] = 0;

            errno = 0;
            w = write(data_socket, buffer, sizeof(buffer));
            errnum = errno;
            man7_connection::write_function_results(__func__, "write", w, errnum);

            if (w == -1)
            {
                man7_connection::close_without_changing_errno(__func__, data_socket);
                man7_connection::close_without_changing_errno(__func__, connection_socket);
                unlink_without_changing_errno(SOCKET_NAME);
                return 6;
            }

            /* Close socket. */

            man7_connection::close_without_changing_errno(__func__, data_socket);

            /* Quit on DOWN command. */

            if (down_flag)
            {
                break;
            }
        }

        man7_connection::close_without_changing_errno(__func__, connection_socket);

        /* Unlink the socket. */

        unlink_without_changing_errno(SOCKET_NAME);

        return 0;
    }
};

#endif // SANDBOX_CPP_MAN7_SERVER
