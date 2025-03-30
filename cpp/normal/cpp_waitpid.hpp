#ifndef SANDBOX_CPP_WAITPID
#define SANDBOX_CPP_WAITPID

#include "man7_connection.hpp"
#include <string>

// https://www.man7.org/linux/man-pages/man2/wait.2.html
#include <sys/wait.h>

class cpp_waitpid
{
private:
    // https://www.man7.org/linux/man-pages/man2/wait.2.html
    // Don't change errno.
    static void write_wstatus_explanation(const std::string &name_of_calling_function, int wstatus, int return_value, int errnum)
    {
        const int errnum_2 = errno;
        std::string s("waitpid");

        if (WIFEXITED(wstatus))
        {
            s += " (child exit with status ";
            s += std::to_string(WEXITSTATUS(wstatus));
            s += ')';
        }
        else if (WIFSIGNALED(wstatus))
        {
            s += " (child term by sig ";
            s += std::to_string(WTERMSIG(wstatus));
#ifdef WCOREDUMP
            s += WCOREDUMP(wstatus) ? " and core dump occurred" : " and core dump did not occur";
#endif // WCOREDUMP
            s += ')';
        }
        else if (WIFSTOPPED(wstatus))
        {
            s += " (child stop by sig ";
            s += std::to_string(WSTOPSIG(wstatus));
            s += ')';
        }
#ifdef WIFCONTINUED
        else if (WIFCONTINUED(wstatus))
        {
            s += " (child cont)";
        }
#endif // WIFCONTINUED

        man7_connection::write_function_results(name_of_calling_function, s, return_value, errnum);
        errno = errnum_2;
    }

public:
    // https://www.man7.org/linux/man-pages/man2/fork.2.html
    // https://www.man7.org/linux/man-pages/man2/wait.2.html
    // Unlike `waitpid()`, this function does not return a PID.
    static int waitpid_special(pid_t pid)
    {
        int return_value = 1000;
        int errnum = 0;
        int wstatus = 0;

        do
        {
            wstatus = -1;

            errno = 0;
#ifdef WIFCONTINUED
            pid_t waitpid_result = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
#else
            pid_t waitpid_result = waitpid(pid, &wstatus, WUNTRACED);
#endif // WIFCONTINUED
            errnum = errno;

            if (waitpid_result == -1)
            {
                return_value = 1001;
                man7_connection::write_function_results(__func__, "waitpid (error)", waitpid_result, errnum);
                break;
            }
            else
            {
                if (WIFEXITED(wstatus))
                {
                    return_value = WEXITSTATUS(wstatus);
                }
                else if (WIFSIGNALED(wstatus))
                {
                    return_value = 1002;
                }

                write_wstatus_explanation(__func__, wstatus, waitpid_result, errnum);
            }

        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

        return return_value;
    }
};

#endif // SANDBOX_CPP_WAITPID
