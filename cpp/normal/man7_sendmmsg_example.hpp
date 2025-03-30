// Adapted from "EXAMPLES" section of https://www.man7.org/linux/man-pages/man2/sendmmsg.2.html

#ifndef SANDBOX_CPP_MAN7_SENDMMSG_EXAMPLE
#define SANDBOX_CPP_MAN7_SENDMMSG_EXAMPLE

#include "man7_connection.hpp"
#include "cpp_sockets.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

class man7_sendmmsg_example
{
public:
    static int man7_sendmmsg_example_main(void)
    {
#ifdef _GNU_SOURCE
        int errnum = 0;

        errno = 0;
        const int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        errnum = errno;
        man7_connection::write_function_results(__func__, "socket", sockfd, errnum);

        if (sockfd == -1)
        {
            man7_connection::close_without_changing_errno(__func__, sockfd);
            return 1;
        }

        sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        addr.sin_port = htons(LOWER_BOUND_FOR_LOCAL_IP_PORT);

        errno = 0;
        const int connect_result = connect(sockfd, (sockaddr *)&addr, sizeof(addr));
        errnum = errno;
        man7_connection::write_function_results(__func__, "connect", connect_result, errnum);

        if (connect_result == -1)
        {
            man7_connection::close_without_changing_errno(__func__, sockfd);
            return 2;
        }

        iovec msg1[3];
        memset(msg1, 0, sizeof(msg1));
        char s1[] = "a";
        msg1[0].iov_base = s1;
        msg1[0].iov_len = strlen(s1);
        char s2[] = "bc";
        msg1[1].iov_base = s2;
        msg1[1].iov_len = strlen(s2);
        char s3[] = "defg";
        msg1[2].iov_base = s3;
        msg1[2].iov_len = strlen(s3);

        iovec msg2;
        memset(&msg2, 0, sizeof(msg2));
        char s4[] = "hijklmno";
        msg2.iov_base = s4;
        msg2.iov_len = strlen(s4);

        iovec msg3;
        memset(&msg3, 0, sizeof(msg3));
        char s5[] = "pqrstuvwxyz01234";
        msg3.iov_base = s5;
        msg3.iov_len = strlen(s5);

        mmsghdr msg[3];
        memset(msg, 0, sizeof(msg));
        msg[0].msg_hdr.msg_iov = msg1;
        msg[0].msg_hdr.msg_iovlen = sizeof(msg1) / sizeof(iovec);
        msg[1].msg_hdr.msg_iov = &msg2;
        msg[1].msg_hdr.msg_iovlen = 1;
        msg[2].msg_hdr.msg_iov = &msg3;
        msg[2].msg_hdr.msg_iovlen = 1;

        write_mmsghdrs(msg, sizeof(msg) / sizeof(mmsghdr));

        errno = 0;
        const int sendmmsg_result = sendmmsg(sockfd, msg, sizeof(msg) / sizeof(mmsghdr), 0);
        errnum = errno;

        write_mmsghdrs(msg, sizeof(msg) / sizeof(mmsghdr));

        write_sendmmsg_results(__func__, sendmmsg_result, errnum);

        man7_connection::close_without_changing_errno(__func__, sockfd);
        return (sendmmsg_result == -1) ? 3 : 0;
#else
        // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
        errno = ENOSYS;

        man7_connection::close_without_changing_errno(__func__, sockfd);
        return 4;
#endif // _GNU_SOURCE
    }

private:
    // https://www.man7.org/linux/man-pages/man2/write.2.html
    // https://www.man7.org/linux/man-pages/man3/write.3p.html
    // Output the string atomically, and don't change errno.
    static void write_sendmmsg_results(const std::string &name_of_calling_function, int return_value, int errnum)
    {
        const int errnum_2 = errno;
        std::string s("sendmmsg -- ");
        if (return_value == -1)
        {
            s += "Error";
        }
        else
        {
            s += std::to_string(return_value);
            s += (return_value == 1) ? " message sent" : " messages sent";
        }
        man7_connection::write_function_results(name_of_calling_function, s, return_value, errnum);
        errno = errnum_2;
    }

    // https://www.man7.org/linux/man-pages/man3/snprintf.3.html
    static std::string void_pointer_str(const void *pointer)
    {
        std::string s;

        if (!pointer)
        {
            s += "NULL";
        }
        else
        {
            s += "0x";
            const uint64_t integer = (uint64_t)pointer;
            for (int i = 0; i < 16; i++)
            {
                char c = '0' + ((char)(0xF & (integer >> (60 - 4 * i))));
                if (c > '9')
                {
                    c += 'a' - '9';
                }
                s += c;
            }
        }

        return s;
    }

    // "DESCRIPTION" section of https://www.man7.org/linux/man-pages/man2/sendmmsg.2.html
    // "EXAMPLES" section of https://www.man7.org/linux/man-pages/man2/sendmmsg.2.html
    // "SYNOPSIS" section of https://www.man7.org/linux/man-pages/man3/iovec.3type.html
    // "sendmsg()" section of https://www.man7.org/linux/man-pages/man2/sendmsg.2.html
    // https://www.man7.org/linux/man-pages/man2/write.2.html
    // https://www.man7.org/linux/man-pages/man3/write.3p.html
    // Output the string atomically, and don't change errno.
    static void write_mmsghdrs(const mmsghdr *messages, int num_messages)
    {
        const int errnum = errno;
        std::string s;

        if (!messages)
        {
            s += "NULL";
        }
        else
        {
            s += '{';
            for (int i = 0; i < num_messages; i++)
            {
                if (i > 0)
                {
                    s += ',';
                }
                s += '{';
                s += '{';
                s += void_pointer_str(messages[i].msg_hdr.msg_name);
                s += ',';
                s += std::to_string(messages[i].msg_hdr.msg_namelen);
                s += ',';
                s += '{';
                for (size_t j = 0; j < messages[i].msg_hdr.msg_iovlen; j++)
                {
                    if (j > 0)
                    {
                        s += ',';
                    }

                    s += '{';
                    if (messages[i].msg_hdr.msg_iov[j].iov_base)
                    {
                        s += '\"';
                        for (size_t k = 0; k < messages[i].msg_hdr.msg_iov[j].iov_len; k++)
                        {
                            const char c = ((const char *)(messages[i].msg_hdr.msg_iov[j].iov_base))[k];
                            if (!c)
                            {
                                break;
                            }
                            s += c;
                        }
                        s += '\"';
                    }
                    else
                    {
                        s += "NULL";
                    }
                    s += ',';
                    s += std::to_string(messages[i].msg_hdr.msg_iov[j].iov_len);
                    s += '}';
                }
                s += '}';
                s += ',';
                s += std::to_string(messages[i].msg_hdr.msg_iovlen);
                s += ',';
                s += void_pointer_str(messages[i].msg_hdr.msg_control);
                s += ',';
                s += std::to_string(messages[i].msg_hdr.msg_controllen);
                s += ',';
                s += std::to_string(messages[i].msg_hdr.msg_flags);
                s += '}';
                s += ',';
                s += std::to_string(messages[i].msg_len);
                s += '}';
            }
            s += '}';
        }
        man7_connection::write_str(s);
        errno = errnum;
    }
};

#endif // SANDBOX_CPP_MAN7_SENDMMSG_EXAMPLE
