#ifndef SANDBOX_CPP_ARGS_TEST
#define SANDBOX_CPP_ARGS_TEST

#include "cpp_args.hpp"

class cpp_args_test
{
private:
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

public:
    static void test_args_class(int argc, char const *argv[])
    {
        args a1(argc, argv);

        args a2(a1);
        args a3(0, nullptr);
        a3.operator_os(std::cout << '\n') << '\n';
        a3 = a1;

        a1.operator_os(std::cout << '\n') << '\n';
        a2.operator_os(std::cout << '\n') << '\n';
        a3.operator_os(std::cout << '\n') << '\n';

        std::cout << '\n';

        test_args_iterators(a1, argc, argv);
        test_args_iterators(a2, argc, argv);
        test_args_iterators(a3, argc, argv);

        args a4{"__func__ is", __func__, nullptr, "CHESS PIECES INCLUDE...", "king", "and", "", "queen"};

        a3 = a4;
        a3.operator_os(std::cout << '\n') << '\n';
        a4.operator_os(std::cout << '\n') << '\n';

        std::cout << '\n';
    }
};

#endif // SANDBOX_CPP_ARGS_TEST
