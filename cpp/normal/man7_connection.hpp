// Adapted from the "File connection.h" section of https://www.man7.org/linux/man-pages/man7/unix.7.html

#ifndef SANDBOX_CPP_MAN7_CONNECTION
#define SANDBOX_CPP_MAN7_CONNECTION

#define SOCKET_NAME "/tmp/9Lq7BNBnBycd6nxy_cpp.socket"
#define BUFFER_SIZE 12

#include <string>
#include <string.h>
#include <unistd.h> // https://www.man7.org/linux/man-pages/man2/getpid.2.html

class man7_connection
{
public:
    // https://www.man7.org/linux/man-pages/man2/write.2.html
    // https://www.man7.org/linux/man-pages/man3/write.3p.html
    // Output the string atomically, and don't change errno.
    static void write_str(const std::string &str)
    {
        const int errnum = errno;
        std::string s;
        s += "Process ";
        s += std::to_string(getpid());
        s += " -- ";
        s += str;
        s += '\n';
        write(STDOUT_FILENO, s.c_str(), s.size());
        errno = errnum;
    }

    // https://www.man7.org/linux/man-pages/man3/strerror.3.html
    // https://www.man7.org/linux/man-pages/man2/write.2.html
    // https://www.man7.org/linux/man-pages/man3/write.3p.html
    // Output the string atomically, and don't change errno.
    static void write_function_results(const std::string &name_of_calling_function, const std::string &string_containing_name_of_function_called, int return_value, int errnum)
    {
        const int errnum_2 = errno;
        std::string s(name_of_calling_function);
        s += " called ";
        s += string_containing_name_of_function_called;
        s += " -- return_value is ";
        s += std::to_string(return_value);
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
        write_str(s);
        errno = errnum_2;
    }

    // https://www.man7.org/linux/man-pages/man0/unistd.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/close.2.html
    // https://www.man7.org/linux/man-pages/man3/close.3p.html
    // Don't change errno.
    static void close_without_changing_errno(const std::string &name_of_calling_function, int fd)
    {
        const int errnum = errno;
        close(fd);
        std::string s(name_of_calling_function);
        s += " called ";
        s += __func__;
        s += " -- fd is ";
        s += std::to_string(fd);
        write_str(s);
        errno = errnum;
    }

    // https://www.man7.org/linux/man-pages/man2/write.2.html
    // https://www.man7.org/linux/man-pages/man3/write.3p.html
    // Typically, str is solely comprised of one or more sentences.
    // Output the string atomically, and don't change errno.
    static void write_num_tries_left(const std::string &str, int num_tries_left)
    {
        const int errnum = errno;
        std::string s(str);
        s += " (Number of tries left is ";
        s += std::to_string(num_tries_left);
        s += ".)";
        write_str(s);
        errno = errnum;
    }
    
    // Output the string atomically, and don't change errno.
    static void write_loop_variable(int i)
    {
        const int errnum = errno;
        std::string s("Loop variable equals ");
        s += std::to_string(i);
        man7_connection::write_str(s);
        errno = errnum;
    }
};

#endif // SANDBOX_CPP_MAN7_CONNECTION
