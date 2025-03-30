#ifndef SANDBOX_CPP_MAN7_GETADDRINFO_EXAMPLE_CLIENT
#define SANDBOX_CPP_MAN7_GETADDRINFO_EXAMPLE_CLIENT

// Adapted from "Client program" section of https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html

#include "man7_getaddrinfo_example.hpp"
#include "cpp_sockets.hpp"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <string>

class man7_getaddrinfo_example_client
{
public:
    static int man7_getaddrinfo_example_client_main(int argc, char const *argv[])
    {
        int errnum = 0;

        int sfd;
        int getaddrinfo_result;
        char buf[BUF_SIZE_getaddrinfo];
        size_t size;
        ssize_t nread, nwrite;
        addrinfo hints;
        addrinfo *result, *rp;

        if (argc < 3)
        {
            write_usage(argv[0]);

            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            errno = EINVAL;

            return 1;
        }

        if (strcmp(argv[1], "127.0.0.1") && strcmp(argv[1], "::1"))
        {
            // i.e., argv[1] does not equal "127.0.0.1" and does not equal "::1"

            // The "DESCRIPTION" section of https://www.man7.org/linux/man-pages/man7/ipv6.7.html
            // says that "::1" is IPv6's loopback address.

            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            errno = EADDRNOTAVAIL;

            return 2;
        }

        try
        {
            // https://en.cppreference.com/w/cpp/string/basic_string/stol
            const long long port_number = std::stoll(argv[2]);

            // By the way, according to https://www.man7.org/linux/man-pages/man7/ipv6.7.html,
            // "IPv4 and IPv6 share the local port space."

            if (port_number < ((long long)LOWER_BOUND_FOR_LOCAL_IP_PORT) || port_number > ((long long)65535))
            {
                // Refer to 'errno = EACCES;' in cpp_sockets::f_bind_ip()
                errno = EACCES;
                return 3;
            }
        }
        catch (...)
        {
            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            errno = EINVAL;

            return 4;
        }

        /* Obtain address(es) matching host/port. */

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
        hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
        hints.ai_flags = 0;
        hints.ai_protocol = 0; /* Any protocol */

        errno = 0;
        getaddrinfo_result = getaddrinfo(argv[1], argv[2], &hints, &result);
        errnum = errno;
        man7_getaddrinfo_example::write_getinfo_results(__func__, "getaddrinfo", getaddrinfo_result, errnum);

        if (getaddrinfo_result)
        {
            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            // "RETURN VALUE" section of https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
            switch (getaddrinfo_result)
            {
            case EAI_ADDRFAMILY:
                errno = EADDRNOTAVAIL;
                return 5;
            case EAI_AGAIN:
                errno = EAGAIN;
                return 6;
            case EAI_BADFLAGS:
                errno = EINVAL;
                return 7;
            case EAI_FAIL:
                errno = ENOTRECOVERABLE;
                return 8;
            case EAI_FAMILY:
                errno = EAFNOSUPPORT;
                return 9;
            case EAI_MEMORY:
                errno = ENOMEM;
                return 10;
            case EAI_NODATA:
                errno = EADDRNOTAVAIL;
                return 11;
            case EAI_NONAME:
                errno = EINVAL;
                return 12;
            case EAI_SERVICE:
                errno = EOPNOTSUPP;
                return 13;
            case EAI_SOCKTYPE:
                errno = ESOCKTNOSUPPORT;
                return 14;
            case EAI_SYSTEM:
                return 15;
            default:
                errno = EIO;
                return 16;
            }
        }

        /* getaddrinfo() returns a list of address structures.
           Try each address until we successfully connect(2).
           If socket(2) (or connect(2)) fails, we (close the socket
           and) try the next address. */

        for (rp = result; rp; rp = rp->ai_next)
        {
            errno = 0;
            sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            errnum = errno;
            man7_connection::write_function_results(__func__, "socket", sfd, errnum);

            if (sfd == -1)
            {
                continue;
            }

            errno = 0;
            const int connect_result = connect(sfd, rp->ai_addr, rp->ai_addrlen); // https://www.man7.org/linux/man-pages/man2/connect.2.html
            errnum = errno;
            man7_connection::write_function_results(__func__, "connect", connect_result, errnum);

            if (!connect_result)
            {
                break; /* Success */
            }

            man7_connection::close_without_changing_errno(__func__, sfd);
        }

        freeaddrinfo(result); /* No longer needed */

        if (!rp)
        {
            /* No address succeeded */
            man7_connection::write_str("Could not connect");
            man7_connection::close_without_changing_errno(__func__, sfd);
            return 17;
        }

        /* Send remaining command-line arguments as separate
           datagrams, and read responses from server. */

        for (int j = 3; j < argc; j++)
        {
            size = strlen(argv[j]) + 1;
            /* +1 for terminating null byte */

            if (size > BUF_SIZE_getaddrinfo)
            {
                write_ignoring_long_message(j);
                continue;
            }

            errno = 0;
            nwrite = write(sfd, argv[j], size);
            errnum = errno;

            if (nwrite != ((ssize_t)size))
            {
                man7_connection::write_function_results(__func__, "write -- partial/failed write", nwrite, errnum);
                man7_connection::close_without_changing_errno(__func__, sfd);
                return 18;
            }
            else
            {
                man7_connection::write_function_results(__func__, "write -- successful write", nwrite, errnum);
            }

            errno = 0;

            // "EXAMPLES" section of https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
            // ... The programs are an echo server and client for UDP datagrams.
            //
            // "ERRORS" section of https://www.man7.org/linux/man-pages/man7/udp.7.html:
            // ECONNREFUSED
            // No receiver was associated with the destination address. This
            // might be caused by a previous packet sent over the socket.
            //
            nread = read(sfd, buf, BUF_SIZE_getaddrinfo); // TODO If argv[1] is "::1", then this line sets errno to ECONNREFUSED.

            errnum = errno;
            man7_connection::write_function_results(__func__, "read", nread, errnum);
            buf[sizeof(buf) - 1] = 0;

            if (nread == -1)
            {
                man7_connection::close_without_changing_errno(__func__, sfd);
                return 19;
            }

            write_num_bytes_received(nread, buf);
        }

        man7_connection::close_without_changing_errno(__func__, sfd);
        return 0;
    }

private:
    // Output the string atomically, and don't change errno.
    static void write_ignoring_long_message(int index)
    {
        const int errnum = errno;
        std::string s("Ignoring long message in argument ");
        s += std::to_string(index);
        man7_connection::write_str(s);
        errno = errnum;
    }

    // Output the string atomically, and don't change errno.
    static void write_num_bytes_received(ssize_t nread, const std::string &buf)
    {
        const int errnum = errno;
        std::string s("Received ");
        s += std::to_string(nread);
        s += ((nread == 1) ? " byte: " : " bytes: ");
        s += buf;
        man7_connection::write_str(s);
        errno = errnum;
    }

    // Output the string atomically, and don't change errno.
    static void write_usage(const std::string &argv_0)
    {
        const int errnum = errno;
        std::string s("Usage: ");
        s += argv_0;
        s += " host port msg...";
        man7_connection::write_str(s);
        errno = errnum;
    }
};

#endif // SANDBOX_CPP_MAN7_GETADDRINFO_EXAMPLE_CLIENT
