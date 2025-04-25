// Adapted from "EXAMPLES" section of https://www.man7.org/linux/man-pages/man2/select_tut.2.html

#ifndef SANDBOX_CPP_MAN7_SELECT_TUT_EXAMPLE
#define SANDBOX_CPP_MAN7_SELECT_TUT_EXAMPLE

#include "man7_connection.hpp"
#include "cpp_sockets.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdexcept>
#include <cassert>

#ifndef BUF_SIZE_select_tut
#define BUF_SIZE_select_tut 1024
#endif // BUF_SIZE_select_tut

// Curious about byte orders? Then refer to:
// https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
// https://www.man7.org/linux/man-pages/man7/ip.7.html
// https://www.man7.org/linux/man-pages/man3/sockaddr_in.3type.html
// https://www.man7.org/linux/man-pages/man3/htons.3.html
// https://www.man7.org/linux/man-pages/man3/htonl.3.html
// https://www.man7.org/linux/man-pages/man3/ntohs.3.html
// https://www.man7.org/linux/man-pages/man3/ntohl.3.html

class man7_select_tut_example
{
private:
    static const int MAX_NUM_LOOPS = 3;
    static const int SELECT_TIMEOUT_IN_SECONDS = 1;

private:
    int forward_port = 0;

private:
    static int maximum_of_two_ints(int x, int y)
    {
        return (x > y) ? x : y;
    }

    // Based on man7_connection::close_without_changing_errno()
    // Don't change errno.
    static void shut_fd_without_changing_errno(const std::string &name_of_calling_function, int &fd)
    {
        const int errnum = errno;
        const int old_fd = fd;
        if (fd >= 0)
        {
            shutdown(fd, SHUT_RDWR);
            close(fd);
            fd = -1;
        }
        const int new_fd = fd;
        std::string s(name_of_calling_function);
        s += " called ";
        s += __func__;
        s += " -- fd was ";
        s += std::to_string(old_fd);
        s += " -- fd is now ";
        s += std::to_string(new_fd);
        man7_connection::write_str(s);
        errno = errnum;
    }

private:
    static int listen_socket(int listen_port_in_host_byte_order)
    {
        if (listen_port_in_host_byte_order > 65535)
        {
            errno = EOVERFLOW;
            return -1;
        }
        if (listen_port_in_host_byte_order < ((int)LOWER_BOUND_FOR_LOCAL_IP_PORT))
        {
            // Refer to 'errno = EACCES;' in cpp_sockets::f_bind_ip()
            errno = EACCES;
            return -1;
        }

        int errnum = 0;

        errno = 0;
        int lfd = cpp_sockets::f_socket_ip_stream_tcp();
        errnum = errno;
        man7_connection::write_function_results(__func__, "f_socket_ip_stream_tcp", lfd, errnum);

        if (lfd == -1)
        {
            return -1;
        }

        const int yes = 1;

        errno = 0;
        const int setsockopt_result = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)); // https://www.man7.org/linux/man-pages/man2/setsockopt.2.html
        errnum = errno;
        man7_connection::write_function_results(__func__, "setsockopt", setsockopt_result, errnum);

        if (setsockopt_result == -1)
        {
            shut_fd_without_changing_errno(__func__, lfd);
            return -1;
        }

        errno = 0;
        const int bind_result = cpp_sockets::f_bind_ip(lfd, listen_port_in_host_byte_order, ntohl((in_addr_t)0));
        errnum = errno;
        man7_connection::write_function_results(__func__, "f_bind_ip", bind_result, errnum);

        if (bind_result == -1)
        {
            shut_fd_without_changing_errno(__func__, lfd);
            return -1;
        }

        write_accepting_connections(listen_port_in_host_byte_order);

        errno = 0;
        const int listen_result = listen(lfd, 10); // https://www.man7.org/linux/man-pages/man2/listen.2.html
        errnum = errno;
        man7_connection::write_function_results(__func__, "listen", listen_result, errnum);

        if (listen_result == -1)
        {
            shut_fd_without_changing_errno(__func__, lfd);
            return -1;
        }

        return lfd;
    }

    static int connect_socket(int connect_port_in_host_byte_order, const char *address)
    {
        if (connect_port_in_host_byte_order > 65535)
        {
            errno = EOVERFLOW;
            return -1;
        }
        if (connect_port_in_host_byte_order < ((int)LOWER_BOUND_FOR_LOCAL_IP_PORT))
        {
            // Refer to 'errno = EACCES;' in cpp_sockets::f_bind_ip()
            errno = EACCES;
            return -1;
        }
        if (!address)
        {
            errno = EINVAL;
            return -1;
        }

        int errnum = 0;

        errno = 0;
        int cfd = cpp_sockets::f_socket_ip_stream_tcp();
        errnum = errno;
        man7_connection::write_function_results(__func__, "f_socket_ip_stream_tcp", cfd, errnum);

        if (cfd == -1)
        {
            return -1;
        }

        sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_port = htons(connect_port_in_host_byte_order);
        addr.sin_family = AF_INET;

        errno = 0;
        const bool failure = !inet_aton(address, (in_addr *)&addr.sin_addr.s_addr); // https://www.man7.org/linux/man-pages/man3/inet_aton.3.html
        errnum = errno;

        write_inet_aton_results(__func__, address, failure, errnum);

        if (failure)
        {
            shut_fd_without_changing_errno(__func__, cfd);
            return -1;
        }

        errno = 0;
        const int connect_result = connect(cfd, (const sockaddr *)&addr, sizeof(addr)); // https://www.man7.org/linux/man-pages/man2/connect.2.html
        errnum = errno;
        man7_connection::write_function_results(__func__, "connect", connect_result, errnum);

        if (connect_result == -1)
        {
            shut_fd_without_changing_errno(__func__, cfd);
            return -1;
        }

        return cfd;
    }

    // https://www.man7.org/linux/man-pages/man2/write.2.html
    // https://www.man7.org/linux/man-pages/man3/write.3p.html
    // https://www.man7.org/linux/man-pages/man3/strerror.3.html
    // Output the string atomically, and don't change errno.
    // Based on man7_inet_pton_example::write_inet_ntop_results()
    static void write_inet_aton_results(const std::string &name_of_calling_function, const std::string &addr_str, bool failure, int errnum)
    {
        const int errnum_2 = errno;
        std::string s;
        s += name_of_calling_function;
        s += " called inet_aton -- ";
        s += (failure ? "Failure (bad IP address format)" : "Success");
        s += " -- Address string is ";
        s += addr_str;
        s += " -- errnum is ";
        s += std::to_string(errnum);
#ifdef _GNU_SOURCE
        s += ", whose name is \"";
        s += strerrorname_np(errnum);
        s += "\" and whose desc is \"";
        s += strerrordesc_np(errnum);
        s += '\"';
#else
        s += ", which means \"";
        s += strerror(errnum);
        s += '\"';
#endif // _GNU_SOURCE
        man7_connection::write_str(s);
        errno = errnum_2;
    }

private:
    // TODO What if strerror or strerrorname_np or strerrordesc_np or strsignal or sigdescr_np or sigabbrev_np returns NULL?
    // TODO While I'm at it, centralize duplicate code related to strerror and strsignal.
    // TODO Beware thread-unsafe strsignal

    // According to https://www.man7.org/linux/man-pages/man2/signal.2.html, don't use signal(SIGPIPE, SIG_IGN);
    // https://www.man7.org/linux/man-pages/man2/sigaction.2.html
    static int change_sigaction(int signum, struct sigaction &oldact)
    {
        memset(&oldact, 0, sizeof(oldact));
        oldact.sa_handler = SIG_DFL;

        struct sigaction newact = {0};
        newact.sa_handler = SIG_IGN;

        int errnum = 0;
        errno = 0;
        const int sigaction_result = sigaction(signum, &newact, &oldact);
        errnum = errno;
        write_sigaction_function_results(__func__, "sigaction", signum, sigaction_result, errnum);

        return sigaction_result;
    }

    // TODO What if strerror or strerrorname_np or strerrordesc_np or strsignal or sigdescr_np or sigabbrev_np returns NULL?
    // TODO While I'm at it, centralize duplicate code related to strerror and strsignal.
    // TODO Beware thread-unsafe strsignal

    // Based on man7_getaddrinfo_example::write_getinfo_results()
    // Output the string atomically, and don't change errno.
    static void write_sigaction_function_results(const std::string &name_of_calling_function, const std::string &name_of_function_called, int signum, int return_value, int errnum)
    {
        const int errnum_2 = errno;
        std::string s(name_of_function_called);
        s += " -- signum is ";
        s += std::to_string(signum);

#ifdef _GNU_SOURCE
        s += ", whose name is \"SIG";
        s += sigabbrev_np(signum);
        s += "\" and whose desc is \"";
        s += sigdescr_np(signum);
        s += '\"';
#else
        switch (signum)
        {
        case SIGPIPE:
            s += ", which means \"SIGPIPE\"";
            break;
        default:
            break;
        }
#endif // _GNU_SOURCE

        man7_connection::write_function_results(name_of_calling_function, s, return_value, errnum);
        errno = errnum_2;
    }

    // TODO What if strerror or strerrorname_np or strerrordesc_np or strsignal or sigdescr_np or sigabbrev_np returns NULL?
    // TODO While I'm at it, centralize duplicate code related to strerror and strsignal.
    // TODO Beware thread-unsafe strsignal

    // According to https://www.man7.org/linux/man-pages/man2/signal.2.html, don't use signal(SIGPIPE, SIG_IGN);
    // https://www.man7.org/linux/man-pages/man2/sigaction.2.html
    // https://www.man7.org/linux/man-pages/man3/strerror.3.html
    // https://www.man7.org/linux/man-pages/man3/strsignal.3.html
    // Based on man7_connection::close_without_changing_errno()
    // Don't change errno.
    static void revert_sigaction(const std::string &name_of_calling_function, int signum, const struct sigaction &oldact)
    {
        const int errnum_2 = errno;

        int errnum = 0;
        errno = 0;
        const int sigaction_result = sigaction(signum, &oldact, NULL);
        errnum = errno;
        write_sigaction_function_results(__func__, "sigaction", signum, sigaction_result, errnum);
        write_sigaction_function_results(name_of_calling_function, __func__, signum, sigaction_result, errnum);

        errno = errnum_2;
    }

public:
    int man7_select_tut_example_main(int argc, char const *argv[])
    {
        assert(ntohl(htonl((in_addr_t)0)) == ((in_addr_t)0));
        assert(htonl(ntohl((in_addr_t)0)) == ((in_addr_t)0));

        int errnum = 0;

        int h = -1;
        int ready = -1, nfds = -1;
        int fd1 = -1, fd2 = -1;
        int buf1_avail = 0, buf1_written = 0;
        int buf2_avail = 0, buf2_written = 0;
        char buf1[BUF_SIZE_select_tut], buf2[BUF_SIZE_select_tut];
        fd_set readfds, writefds, exceptfds;
        ssize_t nbytes = -1;

        memset(buf1, 0, sizeof(buf1));
        memset(buf2, 0, sizeof(buf2));
        memset(&readfds, 0, sizeof(readfds));
        memset(&writefds, 0, sizeof(writefds));
        memset(&exceptfds, 0, sizeof(exceptfds));

        if (argc != 4)
        {
            man7_connection::write_str("Usage: fwd <listen-port> <forward-to-port> <forward-to-ip-address>");

            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            // Refer to 'errno = (argc < 2) ? EINVAL : E2BIG;' in 'man7_getaddrinfo_example_server::man7_getaddrinfo_example_server_main()'
            errno = (argc < 4) ? EINVAL : E2BIG;

            return 1;
        }

        // This block is similar to a/the try-catch block in man7_getaddrinfo_example_client::man7_getaddrinfo_example_client_main()
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

        // This block is similar to a/the try-catch block in man7_getaddrinfo_example_server::man7_getaddrinfo_example_server_main()
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
                return 4;
            }
        }
        catch (...)
        {
            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            errno = EINVAL;

            return 5;
        }

        //
        //
        //
        // One edge case is that atoi(argv[1]) == atoi(argv[2]), which is actually fine -- and even preferable -- for this function.
        //
        //
        //

        // This block is similar to an if block in man7_getaddrinfo_example_client::man7_getaddrinfo_example_client_main()
        if (strcmp(argv[3], "127.0.0.1") && strcmp(argv[3], "::1"))
        {
            // i.e., argv[3] does not equal "127.0.0.1" and does not equal "::1"

            // The "DESCRIPTION" section of https://www.man7.org/linux/man-pages/man7/ipv6.7.html
            // says that "::1" is IPv6's loopback address.

            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            errno = EADDRNOTAVAIL;

            return 6;
        }

        // According to https://www.man7.org/linux/man-pages/man2/signal.2.html, don't use signal(SIGPIPE, SIG_IGN);
        // https://www.man7.org/linux/man-pages/man2/sigaction.2.html
        struct sigaction oldact_SIGPIPE = {0};
        errno = 0;
        const int change_sigaction_result = change_sigaction(SIGPIPE, oldact_SIGPIPE);
        errnum = errno;
        write_sigaction_function_results(__func__, "change_sigaction", SIGPIPE, change_sigaction_result, errnum);

        if (change_sigaction_result == -1)
        {
            return 7;
        }

        forward_port = atoi(argv[2]);

        h = listen_socket(atoi(argv[1]));
        if (h == -1)
        {
            shut_fd_without_changing_errno(__func__, h);
            shut_fd_without_changing_errno(__func__, fd1);
            shut_fd_without_changing_errno(__func__, fd2);
            revert_sigaction(__func__, SIGPIPE, oldact_SIGPIPE);
            return 8;
        }

        for (int i = 0; i < MAX_NUM_LOOPS; i++)
        {
            man7_connection::write_loop_variable(i);
            nfds = 0;

            FD_ZERO(&readfds);
            FD_ZERO(&writefds);
            FD_ZERO(&exceptfds);
            FD_SET(h, &readfds);
            nfds = maximum_of_two_ints(nfds, h);

            if (fd1 > 0 && buf1_avail < BUF_SIZE_select_tut)
                FD_SET(fd1, &readfds);
            /* Note: nfds is updated below, when fd1 is added to
               exceptfds. */
            if (fd2 > 0 && buf2_avail < BUF_SIZE_select_tut)
                FD_SET(fd2, &readfds);

            if (fd1 > 0 && buf2_avail - buf2_written > 0)
                FD_SET(fd1, &writefds);
            if (fd2 > 0 && buf1_avail - buf1_written > 0)
                FD_SET(fd2, &writefds);

            if (fd1 > 0)
            {
                FD_SET(fd1, &exceptfds);
                nfds = maximum_of_two_ints(nfds, fd1);
            }
            if (fd2 > 0)
            {
                FD_SET(fd2, &exceptfds);
                nfds = maximum_of_two_ints(nfds, fd2);
            }

            // https://www.man7.org/linux/man-pages/man3/timeval.3type.html
            // https://www.man7.org/linux/man-pages/man3/time_t.3type.html
            // "timeout" in https://www.man7.org/linux/man-pages/man2/select.2.html
            timeval timeout;
            memset(&timeout, 0, sizeof(timeout));
            timeout.tv_sec = (time_t)SELECT_TIMEOUT_IN_SECONDS;
            timeout.tv_usec = (suseconds_t)0;

            errno = 0;
            ready = select(nfds + 1, &readfds, &writefds, &exceptfds, &timeout); // Blocks; TODO Postpone the following: fork and have child process use telnet to send to this process? What about signals?
            errnum = errno;
            man7_connection::write_function_results(__func__, "select", ready, errnum);

            if (ready == -1 && errnum == EINTR)
                continue;

            if (ready == -1)
            {
                shut_fd_without_changing_errno(__func__, h);
                shut_fd_without_changing_errno(__func__, fd1);
                shut_fd_without_changing_errno(__func__, fd2);
                revert_sigaction(__func__, SIGPIPE, oldact_SIGPIPE);
                return 9;
            }

            if (FD_ISSET(h, &readfds))
            {
                sockaddr_in client_addr;
                socklen_t addrlen = sizeof(client_addr);
                memset(&client_addr, 0, addrlen);

                errno = 0;
                const int fd = accept(h, (sockaddr *)&client_addr, &addrlen); // https://www.man7.org/linux/man-pages/man2/accept.2.html
                errnum = errno;
                man7_connection::write_function_results(__func__, "accept", fd, errnum);

                if (fd != -1)
                {
                    shut_fd_without_changing_errno(__func__, fd1);
                    shut_fd_without_changing_errno(__func__, fd2);
                    buf1_avail = buf1_written = 0;
                    buf2_avail = buf2_written = 0;
                    fd1 = fd;

                    errno = 0;
                    fd2 = connect_socket(forward_port, argv[3]);
                    errnum = errno;
                    man7_connection::write_function_results(__func__, "connect_socket", fd2, errnum);

                    if (fd2 == -1)
                        shut_fd_without_changing_errno(__func__, fd1);
                    else
                        write_connect_from(client_addr);

                    /* Skip any events on the old, closed file
                       descriptors. */

                    continue;
                }
            }

            /* NB: read OOB data before normal reads. */

            if (fd1 > 0 && FD_ISSET(fd1, &exceptfds))
            {
                char c = -1;

                errno = 0;
                nbytes = recv(fd1, &c, 1, MSG_OOB); // https://www.man7.org/linux/man-pages/man2/recv.2.html
                errnum = errno;
                write_function_results_with_fd_str_and_ssize_t(__func__, "recv (fd1)", fd1, nbytes, errnum);

                if (nbytes < 1)
                    shut_fd_without_changing_errno(__func__, fd1);
                else
                {
                    errno = 0;
                    const ssize_t send_result = send(fd2, &c, 1, MSG_OOB); // https://www.man7.org/linux/man-pages/man2/send.2.html
                    errnum = errno;
                    write_function_results_with_fd_str_and_ssize_t(__func__, "send (fd2)", fd2, send_result, errnum);
                }
            }
            if (fd2 > 0 && FD_ISSET(fd2, &exceptfds))
            {
                char c = -1;

                errno = 0;
                nbytes = recv(fd2, &c, 1, MSG_OOB); // https://www.man7.org/linux/man-pages/man2/recv.2.html
                errnum = errno;
                write_function_results_with_fd_str_and_ssize_t(__func__, "recv (fd2)", fd2, nbytes, errnum);

                if (nbytes < 1)
                    shut_fd_without_changing_errno(__func__, fd2);
                else
                {
                    errno = 0;
                    const ssize_t send_result = send(fd1, &c, 1, MSG_OOB); // https://www.man7.org/linux/man-pages/man2/send.2.html
                    errnum = errno;
                    write_function_results_with_fd_str_and_ssize_t(__func__, "send (fd1)", fd1, send_result, errnum);
                }
            }
            if (fd1 > 0 && FD_ISSET(fd1, &readfds))
            {
                errno = 0;
                nbytes = read(fd1, buf1 + buf1_avail, BUF_SIZE_select_tut - buf1_avail); // https://www.man7.org/linux/man-pages/man2/read.2.html
                errnum = errno;
                write_function_results_with_fd_str_and_ssize_t(__func__, "read (fd1)", fd1, nbytes, errnum);

                if (nbytes < 1)
                    shut_fd_without_changing_errno(__func__, fd1);
                else
                    buf1_avail += nbytes;
            }
            if (fd2 > 0 && FD_ISSET(fd2, &readfds))
            {
                errno = 0;
                nbytes = read(fd2, buf2 + buf2_avail, BUF_SIZE_select_tut - buf2_avail); // https://www.man7.org/linux/man-pages/man2/read.2.html
                errnum = errno;
                write_function_results_with_fd_str_and_ssize_t(__func__, "read (fd2)", fd2, nbytes, errnum);

                if (nbytes < 1)
                    shut_fd_without_changing_errno(__func__, fd2);
                else
                    buf2_avail += nbytes;
            }
            if (fd1 > 0 && FD_ISSET(fd1, &writefds) && buf2_avail > 0)
            {
                errno = 0;
                nbytes = write(fd1, buf2 + buf2_written, buf2_avail - buf2_written); // https://www.man7.org/linux/man-pages/man2/write.2.html
                errnum = errno;
                write_function_results_with_fd_str_and_ssize_t(__func__, "write (fd1)", fd1, nbytes, errnum);

                if (nbytes < 1)
                    shut_fd_without_changing_errno(__func__, fd1);
                else
                    buf2_written += nbytes;
            }
            if (fd2 > 0 && FD_ISSET(fd2, &writefds) && buf1_avail > 0)
            {
                errno = 0;
                nbytes = write(fd2, buf1 + buf1_written, buf1_avail - buf1_written); // https://www.man7.org/linux/man-pages/man2/write.2.html
                errnum = errno;
                write_function_results_with_fd_str_and_ssize_t(__func__, "write (fd2)", fd2, nbytes, errnum);

                if (nbytes < 1)
                    shut_fd_without_changing_errno(__func__, fd2);
                else
                    buf1_written += nbytes;
            }

            /* Check if write data has caught read data. */

            if (buf1_written == buf1_avail)
                buf1_written = buf1_avail = 0;
            if (buf2_written == buf2_avail)
                buf2_written = buf2_avail = 0;

            /* One side has closed the connection, keep
               writing to the other side until empty. */

            if (fd1 < 0 && buf1_avail - buf1_written == 0)
                shut_fd_without_changing_errno(__func__, fd2);
            if (fd2 < 0 && buf2_avail - buf2_written == 0)
                shut_fd_without_changing_errno(__func__, fd1);
        }
        shut_fd_without_changing_errno(__func__, h);
        shut_fd_without_changing_errno(__func__, fd1);
        shut_fd_without_changing_errno(__func__, fd2);
        revert_sigaction(__func__, SIGPIPE, oldact_SIGPIPE);
        return 0;
    }

private:
    // Output the string atomically, and don't change errno.
    static void write_connect_from(const sockaddr_in &addr)
    {
        const int errnum = errno;
        std::string s("connect from ");
        s += inet_ntoa(addr.sin_addr);
        man7_connection::write_str(s);
        errno = errnum;
    }

    // Output the string atomically, and don't change errno.
    static void write_accepting_connections(int listen_port_in_host_byte_order)
    {
        const int errnum = errno;
        std::string s("accepting connections on port ");
        s += std::to_string(listen_port_in_host_byte_order);
        man7_connection::write_str(s);
        errno = errnum;
    }

    // Output the string atomically, and don't change errno.
    static void write_function_results_with_fd_str_and_ssize_t(const std::string &name_of_calling_function, const std::string &string_containing_name_of_function_called, int fd, ssize_t return_value, int errnum)
    {
        const int errnum_2 = errno;
        std::string s(string_containing_name_of_function_called);
        s += " -- fd is ";
        s += std::to_string(fd);
        man7_connection::write_function_results_with_ssize_t(name_of_calling_function, s, return_value, errnum);
        errno = errnum_2;
    }
};

#endif // SANDBOX_CPP_MAN7_SELECT_TUT_EXAMPLE
