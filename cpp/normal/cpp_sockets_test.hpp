#ifndef SANDBOX_CPP_SOCKETS_TEST
#define SANDBOX_CPP_SOCKETS_TEST

#include "cpp_sockets.hpp"
#include "man7_connection.hpp"

class cpp_sockets_test
{
public:
    static void test_cpp_sockets()
    {
        int errnum = 0;

        errno = 0;
        const int fd = cpp_sockets::f_socket_ip_stream_tcp();
        errnum = errno;

        man7_connection::write_function_results(__func__, "f_socket_ip_stream_tcp", fd, errnum);

        {
            int optval_tos = 94556;
            socklen_t optlen_tos = MAX_SOCKLEN;

            errno = 0;
            int getsockopt_result_tos = cpp_sockets::f_getsockopt_IP_and_IP_TOS(fd, optval_tos, optlen_tos);
            errnum = errno;

            std::string s_tos("f_getsockopt_IP_TOS -- optval_tos is ");
            s_tos += std::to_string(optval_tos);
            s_tos += " -- optlen_tos is ";
            s_tos += std::to_string(optlen_tos);

            man7_connection::write_function_results(__func__, s_tos, getsockopt_result_tos, errnum);
        }

        {
            int optval_ttl = 18635;
            socklen_t optlen_ttl = MAX_SOCKLEN;

            errno = 0;
            int getsockopt_result_ttl = cpp_sockets::f_getsockopt_IP_and_IP_TTL(fd, optval_ttl, optlen_ttl);
            errnum = errno;

            std::string s_ttl("f_getsockopt_IP_TTL -- optval_ttl is ");
            s_ttl += std::to_string(optval_ttl);
            s_ttl += " -- optlen_ttl is ";
            s_ttl += std::to_string(optlen_ttl);

            man7_connection::write_function_results(__func__, s_ttl, getsockopt_result_ttl, errnum);
        }

        {
            errno = 0;
            int close_result = cpp_sockets::f_close(fd);
            errnum = errno;

            man7_connection::write_function_results(__func__, "f_close", close_result, errnum);
        }
    }
};

#endif // SANDBOX_CPP_SOCKETS_TEST
