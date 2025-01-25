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

#define ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(t) assert(t.empty() == (t.size() == 0))
#define ASSERT_SIZE_IS(t, expected_size) assert(t.size() == expected_size)
#define ASSERT_CONTAINS(t, s) assert(t.find(s) != t.end())
#define ASSERT_DOES_NOT_CONTAIN(t, s) assert(t.find(s) == t.end())

// https://www.man7.org/linux/man-pages/man2/wait.2.html
// Don't change errno.
static std::string wstatus_explanation_str(int wstatus)
{
    int errnum = errno;
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

    errno = errnum;
    return s;
}

static void std_cout_Trie_class(const Trie &trie)
{
    trie.operator_os(std::cout << "\ntrie is ") << '\n';
}

// https://en.cppreference.com/w/cpp/language/range-for
static void assert_equality(const Trie &trie, const std::set<std::string> &string_set)
{
    auto iterator_1 = trie.begin();
    auto iterator_2 = string_set.begin();

    for (; iterator_1 != trie.end() && iterator_2 != string_set.end(); ++iterator_1, ++iterator_2)
    {
        assert(*iterator_1 == *iterator_2);
    }

    assert(iterator_1 == trie.end());
    assert(iterator_2 == string_set.end());
}

static void assert_emptiness(const Trie &trie, const std::set<std::string> &string_set)
{
    assert_equality(trie, string_set);

    assert(trie.empty());
    assert(string_set.empty());

    ASSERT_SIZE_IS(trie, 0);
    ASSERT_SIZE_IS(string_set, 0);

    std_cout_Trie_class(trie);
}

static void assert_after_inserting(Trie &trie, std::set<std::string> &string_set, const std::string &what_to_insert, const int expected_size_after_inserting)
{
    assert_equality(trie, string_set);

    ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(trie);
    ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(string_set);

    if (trie.find(what_to_insert) == trie.end())
    {
        assert(trie.insert(what_to_insert).second);
    }
    else
    {
        assert(!trie.insert(what_to_insert).second);
    }

    if (string_set.find(what_to_insert) == string_set.end())
    {
        assert(string_set.insert(what_to_insert).second);
    }
    else
    {
        assert(!string_set.insert(what_to_insert).second);
    }

    ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(trie);
    ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(string_set);

    ASSERT_CONTAINS(trie, what_to_insert);
    ASSERT_CONTAINS(string_set, what_to_insert);

    ASSERT_SIZE_IS(trie, expected_size_after_inserting);
    ASSERT_SIZE_IS(string_set, expected_size_after_inserting);

    assert_equality(trie, string_set);

    std_cout_Trie_class(trie);
}

static void assert_after_erasing(Trie &trie, std::set<std::string> &string_set, const std::string &what_to_insert, const int expected_size_after_erasing)
{
    assert_equality(trie, string_set);

    ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(trie);
    ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(string_set);

    if (trie.find(what_to_insert) == trie.end())
    {
        assert(!trie.erase(what_to_insert));
    }
    else
    {
        assert(trie.erase(what_to_insert));
    }

    if (string_set.find(what_to_insert) == string_set.end())
    {
        assert(!string_set.erase(what_to_insert));
    }
    else
    {
        assert(string_set.erase(what_to_insert));
    }

    ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(trie);
    ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(string_set);

    ASSERT_DOES_NOT_CONTAIN(trie, what_to_insert);
    ASSERT_DOES_NOT_CONTAIN(string_set, what_to_insert);

    ASSERT_SIZE_IS(trie, expected_size_after_erasing);
    ASSERT_SIZE_IS(string_set, expected_size_after_erasing);

    assert_equality(trie, string_set);

    std_cout_Trie_class(trie);
}

static void assert_presence(const Trie &trie, const std::set<std::string> &string_set, const std::string &what_to_search_for, const int expected_size)
{
    assert_equality(trie, string_set);

    ASSERT_CONTAINS(trie, what_to_search_for);
    ASSERT_CONTAINS(string_set, what_to_search_for);

    ASSERT_SIZE_IS(trie, expected_size);
    ASSERT_SIZE_IS(string_set, expected_size);

    std_cout_Trie_class(trie);
}

static void assert_absence(const Trie &trie, const std::set<std::string> &string_set, const std::string &what_to_search_for, const int expected_size)
{
    assert_equality(trie, string_set);

    ASSERT_DOES_NOT_CONTAIN(trie, what_to_search_for);
    ASSERT_DOES_NOT_CONTAIN(string_set, what_to_search_for);

    ASSERT_SIZE_IS(trie, expected_size);
    ASSERT_SIZE_IS(string_set, expected_size);

    std_cout_Trie_class(trie);
}

static void assert_after_clearing(Trie &trie, std::set<std::string> &string_set)
{
    assert_equality(trie, string_set);

    ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(trie);
    ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(string_set);

    trie.clear();
    string_set.clear();

    assert(trie.empty());
    assert(string_set.empty());

    ASSERT_SIZE_IS(trie, 0);
    ASSERT_SIZE_IS(string_set, 0);

    assert_equality(trie, string_set);

    std_cout_Trie_class(trie);
}

static void test_Trie_iterators(Trie &trie)
{
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
}

static void test_Trie_class()
{
    Trie trie;
    std::set<std::string> string_set;

    assert_emptiness(trie, string_set);

    assert_after_inserting(trie, string_set, "fo", 1);
    assert_after_inserting(trie, string_set, "foo", 2);
    assert_after_inserting(trie, string_set, "b", 3);
    assert_after_inserting(trie, string_set, "bar", 4);
    assert_after_inserting(trie, string_set, "foo", 4);
    assert_after_inserting(trie, string_set, "", 5); // IMPORTANT EDGE CASE
    assert_after_inserting(trie, string_set, "abc", 6);
    assert_after_inserting(trie, string_set, "xyz", 7);
    assert_after_inserting(trie, string_set, "a", 8);
    assert_after_inserting(trie, string_set, "foobar", 9);
    assert_after_inserting(trie, string_set, "", 9); // IMPORTANT EDGE CASE

    test_Trie_iterators(trie);

    assert_presence(trie, string_set, "foo", 9);
    assert_presence(trie, string_set, "", 9); // IMPORTANT EDGE CASE

    assert_absence(trie, string_set, "foobaR", 9);

    assert_after_erasing(trie, string_set, "xyz", 8);
    assert_after_erasing(trie, string_set, "", 7); // IMPORTANT EDGE CASE
    assert_after_erasing(trie, string_set, "foo", 6);
    assert_after_erasing(trie, string_set, "foobaR", 6);

    {
        Trie copy(trie);
        assert_equality(copy, string_set);
    }

    assert_after_clearing(trie, string_set);

    assert_after_inserting(trie, string_set, "", 1); // IMPORTANT EDGE CASE
    assert_after_erasing(trie, string_set, "", 0); // IMPORTANT EDGE CASE
}

static void assert_string_equality(const char *s1, const char *s2)
{
    if (s1)
    {
        assert(s2);
        assert(std::string(s1) == std::string(s2));
    }
    else
    {
        assert(!s2);
    }
}

static void test_args_iterators(args &a, int argc, char const *argv[])
{
    int i = 0;

    i = 0;
    for (auto &&arg : a)
    {
        std::cout << (arg ? arg : "nullptr") << '\n';
        assert(i >= 0 && i < argc);
        assert_string_equality(arg, argv[i]);
        i++;
    }

    i = 0;
    for (auto &&const_arg : static_cast<const args &>(a))
    {
        std::cout << (const_arg ? const_arg : "nullptr") << '\n';
        assert(i >= 0 && i < argc);
        assert_string_equality(const_arg, argv[i]);
        i++;
    }

    i = 0;
    for (auto c_iterator = a.cbegin(); c_iterator != a.cend(); ++c_iterator)
    {
        std::cout << (*c_iterator ? *c_iterator : "nullptr") << '\n';
        assert(i >= 0 && i < argc);
        assert_string_equality(*c_iterator, argv[i]);
        i++;
    }
}

static void test_args_class(int argc, char const *argv[])
{
    args a1(argc, argv);

    args a2(a1);
    args a3(0, nullptr);
    a3.operator_os(std::cout << '\n') << '\n';
    a3 = a1;

    // args a3{4, {"cpp/normal/cpp_main", "king", "", "queen"}};

    a1.operator_os(std::cout << '\n') << '\n';
    a2.operator_os(std::cout << '\n') << '\n';
    a3.operator_os(std::cout << '\n') << '\n';

    std::cout << '\n';

    test_args_iterators(a1, argc, argv);
    test_args_iterators(a2, argc, argv);
    test_args_iterators(a3, argc, argv);
}

static void test_cpp_sockets()
{
    int errnum = 0;

    errno = 0;
    int fd = cpp_sockets::f_socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    errnum = errno;

    write_function_results(__func__, "f_socket", fd, errnum);

    errno = 0;
    int return_value = cpp_sockets::f_close(fd);
    errnum = errno;

    write_function_results(__func__, "f_close", return_value, errnum);
}

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
            write_function_results(__func__, "waitpid (error)", waitpid_result, errnum);
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

            write_function_results(__func__, wstatus_explanation_str(wstatus), waitpid_result, errnum);
        }

    } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

    return return_value;
}

// https://www.man7.org/linux/man-pages/man2/fork.2.html
// https://www.man7.org/linux/man-pages/man2/wait.2.html
static int man7_client_main_fork(int argc, const char *argv[])
{
    int errnum = 0;

    errno = 0;
    const pid_t pid_client = fork();
    errnum = errno;

    if (pid_client < 0)
    {
        write_function_results(__func__, "fork (error)", pid_client, errnum);
        return 2000;
    }
    else if (pid_client == 0)
    {
        write_function_results(__func__, "fork (child process)", pid_client, errnum);

        errno = 0;
        int client_result = man7_client_main(argc, argv);
        errnum = errno;
        write_function_results(__func__, "man7_client_main", client_result, errnum);

        exit(client_result);
    }
    else
    {
        write_function_results(__func__, "fork (parent process)", pid_client, errnum);

        errno = 0;
        int waitpid_special_result = waitpid_special(pid_client);
        errnum = errno;

        write_function_results(__func__, "waitpid_special", waitpid_special_result, errnum);

        return waitpid_special_result;
    }
}

// Adapted from the "Example output" section of https://www.man7.org/linux/man-pages/man7/unix.7.html
static int man7_client_main_fork_call_1()
{
    int argc = 3;
    const char *argv[3] = {"./client", "3", "4"};

    return man7_client_main_fork(argc, argv);
}

// Adapted from the "Example output" section of https://www.man7.org/linux/man-pages/man7/unix.7.html
static int man7_client_main_fork_call_2()
{
    int argc = 3;
    const char *argv[3] = {"./client", "11", "-5"};

    return man7_client_main_fork(argc, argv);
}

// Adapted from the "Example output" section of https://www.man7.org/linux/man-pages/man7/unix.7.html
static int man7_client_main_fork_call_3()
{
    int argc = 2;
    const char *argv[2] = {"./client", "DOWN"};

    return man7_client_main_fork(argc, argv);
}

// Adapted from the "Example output" section of https://www.man7.org/linux/man-pages/man7/unix.7.html
// https://www.man7.org/linux/man-pages/man2/fork.2.html
// https://www.man7.org/linux/man-pages/man2/wait.2.html
static void test_man7()
{
    int errnum = 0;

    errno = 0;
    const pid_t pid_server = fork();
    errnum = errno;

    if (pid_server < 0)
    {
        write_function_results(__func__, "fork (error)", pid_server, errnum);
    }
    else if (pid_server == 0)
    {
        write_function_results(__func__, "fork (child process)", pid_server, errnum);

        errno = 0;
        int server_result = man7_server_main();
        errnum = errno;
        write_function_results(__func__, "man7_server_main", server_result, errnum);

        exit(server_result);
    }
    else
    {
        write_function_results(__func__, "fork (parent process)", pid_server, errnum);

        errno = 0;
        int call_1_result = man7_client_main_fork_call_1();
        errnum = errno;

        write_function_results(__func__, "man7_client_main_fork_call_1", call_1_result, errnum);

        errno = 0;
        int call_2_result = man7_client_main_fork_call_2();
        errnum = errno;

        write_function_results(__func__, "man7_client_main_fork_call_2", call_2_result, errnum);

        errno = 0;
        int call_3_result = man7_client_main_fork_call_3();
        errnum = errno;

        write_function_results(__func__, "man7_client_main_fork_call_3", call_3_result, errnum);

        errno = 0;
        int waitpid_special_result = waitpid_special(pid_server);
        errnum = errno;

        write_function_results(__func__, "waitpid_special", waitpid_special_result, errnum);
    }
}

static void test_CHAR_TO_STRING_LITERAL()
{
    for (int i = -128; i < 128; i++)
    {
        char c = (char)i;
        std::cout << "From " << i << " to " << CHAR_TO_STRING_LITERAL(c) << '\n';
    }
}

int main(int argc, char const *argv[])
{
    test_CHAR_TO_STRING_LITERAL();
    std::cout << "\n\n\n";
    test_args_class(argc, argv);
    std::cout << "\n\n\n";
    test_cpp_sockets();
    std::cout << "\n\n\n";
    test_man7();
    std::cout << "\n\n\n";
    test_Trie_class();
    return 0;
}
