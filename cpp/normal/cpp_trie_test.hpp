#ifndef SANDBOX_CPP_TRIE_TEST
#define SANDBOX_CPP_TRIE_TEST

#include "cpp_trie.hpp"
#include "cpp_assert.hpp"
#include <set>

class cpp_trie_test
{
private:
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

public:
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
        assert_after_erasing(trie, string_set, "", 0);   // IMPORTANT EDGE CASE
    }
};

#endif // SANDBOX_CPP_TRIE_TEST
