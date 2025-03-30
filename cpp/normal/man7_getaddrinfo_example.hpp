#ifndef SANBOX_MAN7_GETADDRINFO_EXAMPLE
#define SANBOX_MAN7_GETADDRINFO_EXAMPLE

#include "man7_gnu_source.hpp"

// https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
// https://www.man7.org/linux/man-pages/man3/getnameinfo.3.html
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>
#include <cerrno>
#include "man7_connection.hpp"

#ifndef BUF_SIZE_getaddrinfo
#define BUF_SIZE_getaddrinfo 500
#endif // BUF_SIZE_getaddrinfo

class man7_getaddrinfo_example
{
public:
    // https://www.man7.org/linux/man-pages/man2/write.2.html
    // https://www.man7.org/linux/man-pages/man3/write.3p.html
    // https://www.man7.org/linux/man-pages/man3/strerror.3.html
    // Output the string atomically, and don't change errno.
    // Based on man7_connection::write_function_results()
    static void write_getinfo_results(const std::string &name_of_calling_function, const std::string &name_of_function_called, int return_value, int errnum)
    {
        const int errnum_2 = errno;
        std::string s(name_of_function_called);
        s += " -- return_value is ";

        // "RETURN VALUE" section of https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
        // "RETURN VALUE" section of https://www.man7.org/linux/man-pages/man3/getnameinfo.3.html
        switch (return_value)
        {
        case EAI_ADDRFAMILY:
            s += "EAI_ADDRFAMILY";
            break;
        case EAI_AGAIN:
            s += "EAI_AGAIN";
            break;
        case EAI_BADFLAGS:
            s += "EAI_BADFLAGS";
            break;
        case EAI_FAIL:
            s += "EAI_FAIL";
            break;
        case EAI_FAMILY:
            s += "EAI_FAMILY";
            break;
        case EAI_MEMORY:
            s += "EAI_MEMORY";
            break;
        case EAI_NODATA:
            s += "EAI_NODATA";
            break;
        case EAI_NONAME:
            s += "EAI_NONAME";
            break;
        case EAI_OVERFLOW:
            s += "EAI_OVERFLOW";
            break;
        case EAI_SERVICE:
            s += "EAI_SERVICE";
            break;
        case EAI_SOCKTYPE:
            s += "EAI_SOCKTYPE";
            break;
        case EAI_SYSTEM:
            s += "EAI_SYSTEM";
            break;
        default:
            s += std::to_string(return_value);
            break;
        }

        s += ", which means \"";
        s += gai_strerror(return_value);
        s += '\"';
        man7_connection::write_function_results(name_of_calling_function, s, return_value, errnum);
        errno = errnum_2;
    }
};

#endif // SANBOX_MAN7_GETADDRINFO_EXAMPLE
