#ifndef SANDBOX_CPP_MAN7_GETADDRINFO_EXAMPLE_SERVER
#define SANDBOX_CPP_MAN7_GETADDRINFO_EXAMPLE_SERVER

// /usr/include/features.h
// /usr/include/netdb.h
// https://www.man7.org/linux/man-pages/man7/feature_test_macros.7.html
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // _GNU_SOURCE

// Adapted from "Server program" section of https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html

#include "man7_connection.hpp"
#include "cpp_sockets.hpp"
#include "man7_getaddrinfo_example_client.hpp"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <string>
#include <sys/time.h>

#ifndef BUF_SIZE_getaddrinfo
#define BUF_SIZE_getaddrinfo 500
#endif // BUF_SIZE_getaddrinfo

class man7_getaddrinfo_example_server
{
private:
    static const int MAX_NUM_TRIES = 4;

public:
    static int man7_getaddrinfo_example_server_main(int argc, char const *argv[])
    {
        int errnum = 0;

        int sfd;
        int getinfo_result;
        char buf[BUF_SIZE_getaddrinfo];
        ssize_t nread;
        socklen_t peer_addrlen;
        addrinfo hints;
        addrinfo *result, *rp;
        sockaddr_storage peer_addr;

        if (argc != 2)
        {
            // Based on part of man7_getaddrinfo_example_client::man7_getaddrinfo_example_client_main()
            write_usage(argv[0]);

            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            errno = (argc < 2) ? EINVAL : E2BIG;

            return 1;
        }

        try
        {
            // https://en.cppreference.com/w/cpp/string/basic_string/stol
            const long long port_number = std::stoll(argv[1]);

            // By the way, according to https://www.man7.org/linux/man-pages/man7/ipv6.7.html,
            // "IPv4 and IPv6 share the local port space."

            if (port_number < ((long long)LOWER_BOUND_FOR_LOCAL_IP_PORT) || port_number > ((long long)65535))
            {
                // Refer to 'errno = EACCES;' in cpp_sockets::f_bind_ip()
                errno = EACCES;
                return 2;
            }
        }
        catch (...)
        {
            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            errno = EINVAL;

            return 3;
        }

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
        hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
        hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
        hints.ai_protocol = 0;          /* Any protocol */
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;

        errno = 0;
        getinfo_result = getaddrinfo(NULL, argv[1], &hints, &result);
        errnum = errno;
        man7_getaddrinfo_example_client::write_getinfo_results(__func__, "getaddrinfo", getinfo_result, errnum);

        if (getinfo_result)
        {
            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            // "RETURN VALUE" section of https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
            switch (getinfo_result)
            {
            case EAI_ADDRFAMILY:
                errno = EADDRNOTAVAIL;
                return 4;
            case EAI_AGAIN:
                errno = EAGAIN;
                return 5;
            case EAI_BADFLAGS:
                errno = EINVAL;
                return 6;
            case EAI_FAIL:
                errno = ENOTRECOVERABLE;
                return 7;
            case EAI_FAMILY:
                errno = EAFNOSUPPORT;
                return 8;
            case EAI_MEMORY:
                errno = ENOMEM;
                return 9;
            case EAI_NODATA:
                errno = EADDRNOTAVAIL;
                return 10;
            case EAI_NONAME:
                errno = EINVAL;
                return 11;
            case EAI_SERVICE:
                errno = EOPNOTSUPP;
                return 12;
            case EAI_SOCKTYPE:
                errno = ESOCKTNOSUPPORT;
                return 13;
            case EAI_SYSTEM:
                return 14;
            default:
                errno = EIO;
                return 15;
            }
        }

        /* getaddrinfo() returns a list of address structures.
           Try each address until we successfully bind(2).
           If socket(2) (or bind(2)) fails, we (close the socket
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
            const int bind_result = bind(sfd, rp->ai_addr, rp->ai_addrlen); // https://www.man7.org/linux/man-pages/man2/bind.2.html
            errnum = errno;
            man7_connection::write_function_results(__func__, "bind", bind_result, errnum);

            if (!bind_result)
            {
                break; /* Success */
            }

            man7_connection::close_without_changing_errno(__func__, sfd);
        }

        freeaddrinfo(result); /* No longer needed */

        if (!rp)
        {
            /* No address succeeded */
            man7_connection::write_str("Could not bind");
            man7_connection::close_without_changing_errno(__func__, sfd);
            return 16;
        }

        {
            // https://www.man7.org/linux/man-pages/man3/timeval.3type.html
            // https://www.man7.org/linux/man-pages/man3/time_t.3type.html

            timeval optval;
            memset(&optval, 0, sizeof(optval));
            optval.tv_sec = (time_t)1;
            optval.tv_usec = (suseconds_t)0;

            // https://www.man7.org/linux/man-pages/man2/setsockopt.2.html
            // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
            // https://www.man7.org/linux/man-pages/man2/recvfrom.2.html
            // Allow 'recvfrom` to timeout.
            //
            // Special thanks to...
            // https://github.com/search?q=repo%3Amkerrisk%2Fman-pages+%22SO_RCVTIMEO%22&type=code
            // https://github.com/mkerrisk/man-pages/blob/ae6b221882ce71ba82fcdbe02419a225111502f0/man7/socket.7#L812
            // ...for helping me find SO_RCVTIMEO in...
            // https://www.man7.org/linux/man-pages/man7/socket.7.html

            errno = 0;
            const int setsockopt_result = setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &optval, (socklen_t)(sizeof(optval)));
            errnum = errno;
            man7_connection::write_function_results(__func__, "setsockopt", setsockopt_result, errnum);

            if (setsockopt_result == -1)
            {
                man7_connection::close_without_changing_errno(__func__, sfd);
                return 17;
            }
        }

        /* Read datagrams and echo them back to sender. */

        int num_tries_left = MAX_NUM_TRIES; // Decrease this variable by 1 every time recvfrom returns -1 and sets errno to EAGAIN
        for (int i = 0;; i++)
        {
            man7_connection::write_loop_variable(i);

            char host[NI_MAXHOST], service[NI_MAXSERV];

            peer_addrlen = sizeof(peer_addr);

            errno = 0;
            nread = recvfrom(sfd, buf, BUF_SIZE_getaddrinfo, 0, (sockaddr *)&peer_addr, &peer_addrlen); // https://www.man7.org/linux/man-pages/man2/recvfrom.2.html
            errnum = errno;
            man7_connection::write_function_results(__func__, "recvfrom", nread, errnum);
            buf[sizeof(buf) - 1] = 0;

            if (nread == -1)
            {
                if (errnum == EAGAIN)
                {
                    num_tries_left--;
                    if (num_tries_left <= 0)
                    {
                        man7_connection::write_num_tries_left("Encountered EAGAIN too many times. Server process ending.", num_tries_left);
                        man7_connection::close_without_changing_errno(__func__, sfd);
                        errno = 0;
                        return 0; // Yes, this is intentionally zero instead of a nonzero return value.
                    }
                    else
                    {
                        man7_connection::write_num_tries_left("Encountered EAGAIN. Reattempting recvfrom.", num_tries_left);
                        continue;
                    }
                }
                else
                {
                    continue; /* Ignore failed request */
                }
            }

            errno = 0;
            getinfo_result = getnameinfo((const sockaddr *)&peer_addr, peer_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV); // https://www.man7.org/linux/man-pages/man3/getnameinfo.3.html
            errnum = errno;
            man7_getaddrinfo_example_client::write_getinfo_results(__func__, "getnameinfo", getinfo_result, errnum);
            host[sizeof(host) - 1] = 0;
            service[sizeof(service) - 1] = 0;

            if (!getinfo_result)
            {
                write_num_bytes_received(nread, host, service);
            }

            errno = 0;
            const ssize_t sendto_result = sendto(sfd, buf, nread, 0, (const sockaddr *)&peer_addr, peer_addrlen); // https://www.man7.org/linux/man-pages/man2/sendto.2.html
            errnum = errno;
            man7_connection::write_function_results(__func__, "sendto", sendto_result, errnum);

            if (sendto_result != nread)
            {
                man7_connection::write_str("Error sending response");
            }
        }

        man7_connection::close_without_changing_errno(__func__, sfd);
        return 0;
    }

private:
    // Output the string atomically, and don't change errno.
    // Based on man7_getaddrinfo_example_client::write_num_bytes_received()
    static void write_num_bytes_received(ssize_t nread, const std::string &host, const std::string &service)
    {
        const int errnum = errno;
        std::string s("Received ");
        s += std::to_string(nread);
        s += ((nread == 1) ? " byte from " : " bytes from ");
        s += host;
        s += ':';
        s += service;
        man7_connection::write_str(s);
        errno = errnum;
    }

    // Output the string atomically, and don't change errno.
    // Based on man7_getaddrinfo_example_client::write_usage()
    static void write_usage(const std::string &argv_0)
    {
        const int errnum = errno;
        std::string s("Usage: ");
        s += argv_0;
        s += " port";
        man7_connection::write_str(s);
        errno = errnum;
    }
};

#endif // SANDBOX_CPP_MAN7_GETADDRINFO_EXAMPLE_SERVER
