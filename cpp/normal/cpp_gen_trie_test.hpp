#ifndef SANDBOX_CPP_GEN_TRIE_TEST
#define SANDBOX_CPP_GEN_TRIE_TEST

#include "../gen/output/cpp_gen_trie.hpp"
#include <iostream>

class cpp_gen_trie_test
{
public:
    static void test_CHAR_TO_STRING_LITERAL()
    {
        for (int i = -128; i < 128; i++)
        {
            char c = (char)i;
            std::cout << "From " << i << " to " << CHAR_TO_STRING_LITERAL(c) << '\n';
        }
    }
};

#endif // SANDBOX_CPP_GEN_TRIE_TEST
