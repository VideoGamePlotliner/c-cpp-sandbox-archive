// Adapted from https://www.man7.org/linux/man-pages/man3/inet_pton.3.html and https://www.man7.org/linux/man-pages/man3/inet_ntop.3.html

#ifndef SANDBOX_CPP_MAN7_INET_PTON_EXAMPLE
#define SANDBOX_CPP_MAN7_INET_PTON_EXAMPLE

#include "man7_connection.hpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string>

class man7_inet_pton_example
{
public:
    static int man7_inet_pton_example_main(int domain, const std::string &addr_str)
    {
        switch (domain)
        {
        case AF_INET:
            return man7_inet_pton_example_ipv4(addr_str);
        case AF_INET6:
            return man7_inet_pton_example_ipv6(addr_str);
        default:
            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            // "ERRORS" section of https://www.man7.org/linux/man-pages/man3/inet_pton.3.html
            errno = EAFNOSUPPORT;
            return 1;
        }
    }

private:
    // https://www.man7.org/linux/man-pages/man3/strerror.3.html
    // Based on man7_connection::function_results_str()
    // Don't change errno.
    static std::string inet_ntop_results_str(const std::string &name_of_calling_function, const std::string &addr_str, bool failure, int errnum)
    {
        const int errnum_2 = errno;
        std::string s;
        s += name_of_calling_function;
        s += " called inet_ntop -- ";
        s += (failure ? "Failure" : "Success");
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
        errno = errnum_2;
        return s;
    }

    // https://www.man7.org/linux/man-pages/man2/write.2.html
    // https://www.man7.org/linux/man-pages/man3/write.3p.html
    // Output the string atomically, and don't change errno.
    // Based on man7_connection::write_function_results()
    static void write_inet_ntop_results(const std::string &name_of_calling_function, const std::string &addr_str, bool failure, int errnum)
    {
        const int errnum_2 = errno;
        man7_connection::write_str(inet_ntop_results_str(name_of_calling_function, addr_str, failure, errnum));
        errno = errnum_2;
    }

    // Don't change errno.
    static std::string inet_pton_str(const std::string &addr_str, int result)
    {
        const int errnum = errno;
        std::string s("inet_pton -- ");
        if (result == 0)
        {
            s += "Not in presentation format";
        }
        else if (result < 0)
        {
            s += "Other failure";
        }
        else
        {
            s += "Success";
        }
        s += " -- Address string is ";
        s += addr_str;
        errno = errnum;
        return s;
    }

    // https://www.man7.org/linux/man-pages/man2/write.2.html
    // https://www.man7.org/linux/man-pages/man3/write.3p.html
    // Output the string atomically, and don't change errno.
    // Based on write_inet_ntop_results()
    static void write_inet_pton_results(const std::string &name_of_calling_function, const std::string &addr_str, int result, int errnum)
    {
        const int errnum_2 = errno;
        man7_connection::write_function_results(name_of_calling_function, inet_pton_str(addr_str, result), result, errnum);
        errno = errnum_2;
    }

private:
    static std::string as_hex(uint8_t i)
    {
        std::string s;
        {
            char x = '0' + ((char)(0xF & (i >> 4)));
            if (x > '9')
            {
                x += 'A' - '9';
            }
            s += x;
        }
        {
            char y = '0' + ((char)(0xF & (i >> 0)));
            if (y > '9')
            {
                y += 'A' - '9';
            }
            s += y;
        }
        return s;
    }

private:
    // Output the string atomically, and don't change errno.
    static void write_in_addr(const in_addr &addr)
    {
        const int errnum = errno;
        std::string s("IPv4 address ");
        for (int i = 0; i < 4; i++)
        {
            if (i > 0)
            {
                s += '.';
            }
            s += std::to_string(((in_addr_t)0xFF) & (addr.s_addr >> (24 - 8 * i)));
        }
        // assert(addr.s_addr ==
        //        ((((in_addr_t)0xFF) & (addr.s_addr >> 24)) << 24) +
        //            ((((in_addr_t)0xFF) & (addr.s_addr >> 16)) << 16) +
        //            ((((in_addr_t)0xFF) & (addr.s_addr >> 8)) << 8) +
        //            ((((in_addr_t)0xFF) & (addr.s_addr >> 0)) << 0));
        man7_connection::write_str(s);
        errno = errnum;
    }

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // Output the string atomically, and don't change errno.
    static void write_in6_addr(const in6_addr &addr)
    {
        const int errnum = errno;
        std::string s("IPv6 address ");
        for (int i = 0; i < 8; i++)
        {
            if (i > 0)
            {
                s += ':';
            }
            s += as_hex(addr.s6_addr[2 * i]);
            s += as_hex(addr.s6_addr[2 * i + 1]);
        }

        man7_connection::write_str(s);
        errno = errnum;
    }

private:
    static void check_str_equality(const std::string &addr_str, const std::string &addr_str_2)
    {
        if (addr_str != addr_str_2)
        {
            man7_connection::write_str("Hmm. addr_str != addr_str_2.");
        }
    }

private:
    static void change_errnum_if_inet_pton_returns_zero(int result, int &errnum)
    {
        if (result == 0)
        {
            // https://www.man7.org/linux/man-pages/man3/errno.3.html
            // https://www.man7.org/linux/man-pages/man3/errno.3p.html
            // https://www.man7.org/linux/man-pages/man0/errno.h.0p.html
            // "ERRORS" section of https://www.man7.org/linux/man-pages/man2/connect.2.html
            errno = ENXIO;
            errnum = errno;
        }
    }

private:
    static int man7_inet_pton_example_ipv4(const std::string &addr_str)
    {
        int errnum = 0;

        // "APPLICATION USAGE" section of https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
        in_addr parsed_addr = {0};

        errno = 0;
        int result = inet_pton(AF_INET, addr_str.c_str(), &parsed_addr);
        errnum = errno;

        change_errnum_if_inet_pton_returns_zero(result, errnum);

        write_inet_pton_results(__func__, addr_str, result, errnum);

        write_in_addr(parsed_addr);

        if (result == 0)
        {
            return 2;
        }
        else if (result < 0)
        {
            return 3;
        }

        char addr_str_2[INET_ADDRSTRLEN];

        addr_str_2[sizeof(addr_str_2) - 1] = 0;

        errno = 0;
        const bool failure = !inet_ntop(AF_INET, &parsed_addr, addr_str_2, INET_ADDRSTRLEN);
        errnum = errno;

        addr_str_2[sizeof(addr_str_2) - 1] = 0;

        write_inet_ntop_results(__func__, addr_str_2, failure, errnum);

        if (failure)
        {
            return 4;
        }

        check_str_equality(addr_str, addr_str_2);
        return 0;
    }

    static int man7_inet_pton_example_ipv6(const std::string &addr_str)
    {
        int errnum = 0;

        // "APPLICATION USAGE" section of https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
        in6_addr parsed_addr = {0};

        errno = 0;
        int result = inet_pton(AF_INET6, addr_str.c_str(), &parsed_addr);
        errnum = errno;

        change_errnum_if_inet_pton_returns_zero(result, errnum);

        write_inet_pton_results(__func__, addr_str, result, errnum);

        write_in6_addr(parsed_addr);

        if (result == 0)
        {
            return 5;
        }
        else if (result < 0)
        {
            return 6;
        }

        char addr_str_2[INET6_ADDRSTRLEN];

        addr_str_2[sizeof(addr_str_2) - 1] = 0;

        errno = 0;
        const bool failure = !inet_ntop(AF_INET6, &parsed_addr, addr_str_2, INET6_ADDRSTRLEN);
        errnum = errno;

        addr_str_2[sizeof(addr_str_2) - 1] = 0;

        write_inet_ntop_results(__func__, addr_str_2, failure, errnum);

        if (failure)
        {
            return 7;
        }

        check_str_equality(addr_str, addr_str_2);
        return 0;
    }
};

#endif // SANDBOX_CPP_MAN7_INET_PTON_EXAMPLE
