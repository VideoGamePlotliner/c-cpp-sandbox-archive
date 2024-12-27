#include "cpp_args.hpp"
#include "cpp_trie.hpp"
#include "cpp_assert.hpp"
#include "cpp_sockets.hpp"
#include "man7_client.hpp"
#include "man7_server.hpp"
#include <iostream>
#include <cassert>
#include <string.h>

// https://www.man7.org/linux/man-pages/man2/wait.2.html
#include <sys/wait.h>

// https://www.man7.org/linux/man-pages/man3/strerror.3.html
#ifndef STD_COUT_ERROR_WITH_RETURN_VALUE
#ifdef _GNU_SOURCE
#define STD_COUT_ERROR_WITH_RETURN_VALUE(func_name_str, return_value, errnum) \
    std::cout << "Process " << getpid() << " -- " << func_name_str << " returned " << return_value << " and errno is " << errnum << ", whose name is \"" << strerrorname_np(errnum) << "\" and whose desc is \"" << strerrordesc_np(errnum) << "\"\n";
#else
#define STD_COUT_ERROR_WITH_RETURN_VALUE(func_name_str, return_value, errnum) \
    std::cout << "Process " << getpid() << " -- " << func_name_str << " returned " << return_value << " and errno is " << errnum << ", which means \"" << strerror(errnum) << "\"\n";
#endif // _GNU_SOURCE
#endif // STD_COUT_ERROR_WITH_RETURN_VALUE

static void std_cout_Trie_class(const Trie &trie)
{
    std::cout << '\n';
    std::cout << "trie.size() == " << trie.size() << '\n';
    std::cout << "trie.empty() == " << (trie.empty() ? "true" : "false") << '\n';
    trie.operator_os(std::cout << "trie is ") << '\n';
}

static void test_Trie_class()
{
    Trie trie;
    TRIE_EMPTY_AND_ASSERT(trie);
    std_cout_Trie_class(trie);

    TRIE_INSERT_AND_ASSERT(trie, "fo");
    ASSERT_TRIE_SIZE_IS(trie, 1);
    std_cout_Trie_class(trie);

    TRIE_INSERT_AND_ASSERT(trie, "foo");
    ASSERT_TRIE_SIZE_IS(trie, 2);
    std_cout_Trie_class(trie);

    TRIE_INSERT_AND_ASSERT(trie, "b");
    ASSERT_TRIE_SIZE_IS(trie, 3);
    std_cout_Trie_class(trie);

    TRIE_INSERT_AND_ASSERT(trie, "bar");
    ASSERT_TRIE_SIZE_IS(trie, 4);
    std_cout_Trie_class(trie);

    TRIE_INSERT_AND_ASSERT(trie, "foo");
    ASSERT_TRIE_SIZE_IS(trie, 4);
    std_cout_Trie_class(trie);

    // IMPORTANT EDGE CASE
    TRIE_INSERT_AND_ASSERT(trie, "");
    ASSERT_TRIE_SIZE_IS(trie, 5);
    std_cout_Trie_class(trie);

    TRIE_INSERT_AND_ASSERT(trie, "abc");
    ASSERT_TRIE_SIZE_IS(trie, 6);
    std_cout_Trie_class(trie);

    TRIE_INSERT_AND_ASSERT(trie, "xyz");
    ASSERT_TRIE_SIZE_IS(trie, 7);
    std_cout_Trie_class(trie);

    TRIE_INSERT_AND_ASSERT(trie, "a");
    ASSERT_TRIE_SIZE_IS(trie, 8);
    std_cout_Trie_class(trie);

    TRIE_INSERT_AND_ASSERT(trie, "foobar");
    ASSERT_TRIE_SIZE_IS(trie, 9);
    std_cout_Trie_class(trie);

    // IMPORTANT EDGE CASE
    TRIE_INSERT_AND_ASSERT(trie, "");
    ASSERT_TRIE_SIZE_IS(trie, 9);
    std_cout_Trie_class(trie);

    std::cout << '\n';
    for (auto &&i : trie)
    {
        std::cout << '"' << i << '"' << '\n';
    }
    std::cout << '\n';
    for (const auto &i : trie)
    {
        std::cout << '"' << i << '"' << '\n';
    }
    std::cout << '\n';

    ASSERT_TRIE_CONTAINS_STRING(trie, "foo");
    ASSERT_TRIE_SIZE_IS(trie, 9);
    std_cout_Trie_class(trie);

    // IMPORTANT EDGE CASE
    ASSERT_TRIE_CONTAINS_STRING(trie, "");
    ASSERT_TRIE_SIZE_IS(trie, 9);
    std_cout_Trie_class(trie);

    ASSERT_TRIE_DOES_NOT_CONTAIN_STRING(trie, "foobay");
    ASSERT_TRIE_SIZE_IS(trie, 9);
    std_cout_Trie_class(trie);

    TRIE_ERASE_AND_ASSERT(trie, "xyz");
    ASSERT_TRIE_SIZE_IS(trie, 8);
    std_cout_Trie_class(trie);

    // IMPORTANT EDGE CASE
    TRIE_ERASE_AND_ASSERT(trie, "");
    ASSERT_TRIE_SIZE_IS(trie, 7);
    std_cout_Trie_class(trie);

    TRIE_ERASE_AND_ASSERT(trie, "foo");
    ASSERT_TRIE_SIZE_IS(trie, 6);
    std_cout_Trie_class(trie);

    TRIE_ERASE_AND_ASSERT(trie, "foobay");
    ASSERT_TRIE_SIZE_IS(trie, 6);
    std_cout_Trie_class(trie);

    trie.clear();
    ASSERT_TRIE_EMPTY(trie);
    std_cout_Trie_class(trie);
}

static void test_args_class(int argc, char const *argv[])
{
    args a1(argc, argv);

    args a2(a1);
    args a3(0, nullptr);
    a3 = a1;

    a1.operator_os(std::cout << '\n') << '\n';
    a2.operator_os(std::cout << '\n') << '\n';
    a3.operator_os(std::cout << '\n') << '\n';

    for (auto &&arg : a1)
    {
        std::cout << arg << '\n';
    }
    for (auto &&const_arg : static_cast<const args &>(a1))
    {
        std::cout << const_arg << '\n';
    }
}

static void test_cpp_sockets()
{
    for (int i = 0; i < 3; i++)
    {
        std::cout << '\n';
    }

    int errnum = 0;

    errno = 0;
    int fd = cpp_sockets::f_socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    errnum = errno;

    STD_COUT_ERROR_WITH_RETURN_VALUE("f_socket", fd, errnum);
}

// Adapted from the "Example output" section of https://www.man7.org/linux/man-pages/man7/unix.7.html
static void test_man7()
{
    // https://www.man7.org/linux/man-pages/man2/fork.2.html
    // https://www.man7.org/linux/man-pages/man2/wait.2.html

    int errnum = 0;

    errno = 0;
    const pid_t pid_server = fork();
    errnum = errno;

    if (pid_server == -1)
    {
        std::cout << __func__ << " -- Error\n";
        STD_COUT_ERROR_WITH_RETURN_VALUE("fork", pid_server, errnum);
    }
    else if (pid_server == 0)
    {
        std::cout << __func__ << " -- Child process " << getpid() << '\n';
        exit(man7_server_main());
    }
    else
    {
        std::cout << __func__ << " -- Parent process " << getpid() << " -- fork returned " << pid_server << '\n';

        {
            int argc0 = 3;
            const char *argv0[3] = {"./client", "3", "4"};

            int return_value_0 = man7_client_main_fork(argc0, argv0);

            std::cout << __func__ << " -- Parent process " << getpid() << " -- return_value_0 is " << return_value_0 << '\n';
        }
        {
            int argc1 = 3;
            const char *argv1[3] = {"./client", "11", "-5"};

            int return_value_1 = man7_client_main_fork(argc1, argv1);

            std::cout << __func__ << " -- Parent process " << getpid() << " -- return_value_1 is " << return_value_1 << '\n';
        }
        {
            int argc2 = 2;
            const char *argv2[2] = {"./client", "DOWN"};

            int return_value_2 = man7_client_main_fork(argc2, argv2);

            std::cout << __func__ << " -- Parent process " << getpid() << " -- return_value_2 is " << return_value_2 << '\n';
        }
        waitpid(pid_server, NULL, 0);
    }
}

int main(int argc, char const *argv[])
{
    for (int i = -128; i < 128; i++)
    {
        char c = (char)i;
        std::cout << "From " << i << " to " << CHAR_TO_STRING_LITERAL(c) << '\n';
    }

    test_Trie_class();
    test_args_class(argc, argv);
    test_cpp_sockets();
    test_man7();
    return 0;
}
