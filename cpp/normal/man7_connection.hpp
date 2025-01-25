// Adapted from the "File connection.h" section of https://www.man7.org/linux/man-pages/man7/unix.7.html

#ifndef SANDBOX_CPP_MAN7_CONNECTION
#define SANDBOX_CPP_MAN7_CONNECTION

#define SOCKET_NAME "/tmp/9Lq7BNBnBycd6nxy_cpp.socket"
#define BUFFER_SIZE 12

// https://www.man7.org/linux/man-pages/man3/strerror.3.html
static std::string function_results_str(const std::string &name_of_calling_function, const std::string &name_of_function_called, int return_value, int errnum)
{
    std::string s;
    s += name_of_calling_function;
    s += " called ";
    s += name_of_function_called;
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
    return s;
}

// https://www.man7.org/linux/man-pages/man2/write.2.html
// https://www.man7.org/linux/man-pages/man3/write.3p.html
// Output the string atomically, and don't change errno.
void write_str(const std::string &str)
{
    int errnum = errno;
    std::string s;
    s += "Process ";
    s += std::to_string(getpid());
    s += " -- ";
    s += str;
    s += '\n';
    write(STDOUT_FILENO, s.c_str(), s.size());
    errno = errnum;
}

// https://www.man7.org/linux/man-pages/man2/write.2.html
// https://www.man7.org/linux/man-pages/man3/write.3p.html
// Output the string atomically, and don't change errno.
void write_function_results(const std::string &name_of_calling_function, const std::string &name_of_function_called, int return_value, int errnum)
{
    int errnum_2 = errno;
    write_str(function_results_str(name_of_calling_function, name_of_function_called, return_value, errnum));
    errno = errnum_2;
}

// https://www.man7.org/linux/man-pages/man0/unistd.h.0p.html
// https://www.man7.org/linux/man-pages/man2/close.2.html
// https://www.man7.org/linux/man-pages/man3/close.3p.html
// Don't change errno.
void close_without_changing_errno(int fd)
{
    int errnum = errno;
    close(fd);
    errno = errnum;
}

#endif // SANDBOX_CPP_MAN7_CONNECTION
