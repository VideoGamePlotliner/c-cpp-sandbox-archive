#ifndef SANDBOX_CPP_TRIE
#define SANDBOX_CPP_TRIE

#include <vector>
#include <string>
#include <set>
#include <map>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include "../gen/output/cpp_gen_trie.hpp"

//
//
//
//
//
//
// Trie
//
//
class Trie final
{
private:
    // https://en.cppreference.com/w/cpp/language/exceptions#Exception_safety
    // https://en.cppreference.com/w/cpp/container/set

private:
    //
    //
    //
    //
    //
    //
    // TrieNode
    //
    //
    class TrieNode final
    {
    private:
        // https://en.cppreference.com/w/cpp/container/map
        class TrieNodeChildren final
        {
        private:
            std::map<char, TrieNode> children;

        public:
            // https://en.cppreference.com/w/cpp/container/map/find
            auto find(char c) const noexcept { return children.find(c); }

            // https://en.cppreference.com/w/cpp/container/map/end
            auto end() const noexcept { return children.end(); }

            // https://en.cppreference.com/w/cpp/container/map/end
            auto end() noexcept { return children.end(); }

            // https://en.cppreference.com/w/cpp/container/map/at
            const TrieNode &at(char c) const { return children.at(c); }

            // https://en.cppreference.com/w/cpp/container/map/at
            TrieNode &at(char c) { return children.at(c); }

            // https://en.cppreference.com/w/cpp/container/map/empty
            bool empty() const noexcept { return children.empty(); }

            // https://en.cppreference.com/w/cpp/container/map/begin
            auto begin() const noexcept { return children.begin(); }

            // https://en.cppreference.com/w/cpp/container/map/begin
            auto begin() noexcept { return children.begin(); }

            // https://en.cppreference.com/w/cpp/container/map/clear
            void clear() noexcept { children.clear(); }

            // https://en.cppreference.com/w/cpp/container/map/insert
            auto insert(const std::pair<char, TrieNode> &pair) noexcept { return children.insert(pair); }

            // https://en.cppreference.com/w/cpp/container/map/size
            auto size() const noexcept { return children.size(); }
        };

    private:
        // Is this node the final node of a string in this trie?
        bool is_final_node = false;

        // The list of child nodes and the characters they correspond to
        // std::map<char, TrieNode> children;
        TrieNodeChildren children;

    public:
        bool is_final() const noexcept { return is_final_node; }
        bool has_child_at(char c) const noexcept { return children.find(c) != children.end(); }
        const TrieNode &child_at(char c) const { return children.at(c); }
        bool children_empty() const { return children.empty(); }

    public:
        // The 'bool' part of the return value is whether this node has any
        // non-empty children. If the 'bool' part of the return value is
        // false, then the 'char' part of the return value is meaningless
        // and irrelevant.
        std::pair<char, bool> character_of_first_non_empty_child() const
        {
            // MUST TEST

            for (auto &&pair : children)
            {
                if (!pair.second.empty())
                {
                    return {pair.first, true};
                }
            }
            return {0, false};
        }

        // The 'bool' part of the return value is whether this node has at
        // least one non-empty child whose corresponding character is greater
        // than the parameter. If the 'bool' part of the return value is
        // false, then the 'char' part of the return value is meaningless and
        // irrelevant.
        std::pair<char, bool> character_of_next_non_empty_child(char infimum) const
        {
            // MUST TEST

            for (auto &&pair : children)
            {
                if (pair.first > infimum && !pair.second.empty())
                {
                    return {pair.first, true};
                }
            }
            return {0, false};
        }

    public:
        // https://en.cppreference.com/w/cpp/utility/pair
        // https://en.cppreference.com/w/cpp/utility/pair/make_pair
        // https://en.cppreference.com/w/cpp/container/map/find
        // https://en.cppreference.com/w/cpp/container/map/insert
        // https://en.cppreference.com/w/cpp/container/map/at
        // https://en.cppreference.com/w/cpp/language/initialization
        void insert(const std::string &s) noexcept
        {
            // MUST RE-TEST

            if (s.empty())
            {
                is_final_node = true;
            }
            else
            {
                char c = s.at(0);
                if (children.find(c) == children.end())
                {
                    children.insert({c, {}});
                }
                children.at(c).insert(s.substr(1));
            }
        }

        // https://en.cppreference.com/w/cpp/container/set/erase
        // void erase(const std::string &s) noexcept
        // {
        //     throw std::logic_error(__PRETTY_FUNCTION__);
        // }

        // https://en.cppreference.com/w/cpp/container/set/clear
        void clear() noexcept
        {
            // MUST RE-TEST

            is_final_node = false;
            for (auto &&pair : children)
            {
                pair.second.clear();
            }
            children.clear();
        }

        // https://en.cppreference.com/w/cpp/container/set/size
        // https://en.cppreference.com/w/cpp/types/size_t
        // https://en.cppreference.com/w/cpp/container/map/size
        std::size_t size() const noexcept
        {
            // MUST RE-TEST

            std::size_t size_so_far = (is_final_node ? 1 : 0);
            for (auto &&pair : children)
            {
                size_so_far += pair.second.size();
            }
            return size_so_far;
        }

        // https://en.cppreference.com/w/cpp/container/set/empty
        bool empty() const noexcept
        {
            // MUST RE-TEST

            if (is_final_node)
            {
                return false;
            }
            for (auto &&pair : children)
            {
                if (!pair.second.empty())
                {
                    return false;
                }
            }
            return true;
        }

    public:
        // https://en.cppreference.com/w/cpp/language/operators
        // "Stream extraction and insertion" section
        std::ostream &operator_os(std::ostream &os) const noexcept
        {
            // MUST RE-TEST

            os << (is_final_node ? "{true,{" : "{false,{");
            auto num_children = children.size();
            int i = 0;
            for (auto &&pair : children)
            {
                os << '{' << CHAR_TO_STRING_LITERAL(pair.first) << ',';

                pair.second.operator_os(os) << '}';

                if (i < num_children - 1)
                {
                    os << ',';
                }
                i++;
            }
            return os << "}}";
        }
    };

    //
    //
    //
    //
    //
    //
    // TrieConstIterator
    //
    //
    class TrieConstIterator final
    {
    private:
        // v.empty() means *this == trie.end()
        // If !v.empty(), then v.at(0).first is meaningless and irrelevant, but v.at(0).first should be zero.
        std::vector<std::pair<char, TrieNode>> v;

    private:
        // Throws std::out_of_range if v.empty()
        void depth_first_search_for_final_node(const bool check_whether_final_before_moving_down)
        {
            // MUST TEST

            if (v.empty())
            {
                throw std::out_of_range(__PRETTY_FUNCTION__);
            }

            std::pair<char, bool> pair{0, true};
            while (pair.second)
            {
                if (check_whether_final_before_moving_down)
                {
                    if (v_back().second.is_final())
                    {
                        break;
                    }
                }
                pair = v_back().second.character_of_first_non_empty_child();
                if (pair.second)
                {
                    char c = pair.first;
                    assert(v_back().second.has_child_at(c));
                    const TrieNode &child = v_back().second.child_at(c);
                    v.push_back({c, child});
                }
                if (!check_whether_final_before_moving_down)
                {
                    if (v_back().second.is_final())
                    {
                        break;
                    }
                }
            }
            assert(v_back().second.is_final());
        }

        // Throws std::out_of_range if v.empty()
        const std::pair<char, TrieNode> &v_back() const
        {
            if (v.empty())
            {
                throw std::out_of_range(__PRETTY_FUNCTION__);
            }
            return v.at(v.size() - 1);
        }

    public:
        // Used for trie.end()
        TrieConstIterator() = default;

        // Used for trie.begin()
        TrieConstIterator(const TrieNode &root)
        {
            // MUST TEST

            assert(root.empty() ^ root.character_of_first_non_empty_child().second);

            if (!root.character_of_first_non_empty_child().second)
            {
                return;
            }

            v.push_back({0, root});

            if (root.is_final())
            {
                // Trie contains ""
                return;
            }

            depth_first_search_for_final_node(true);
        }

        // If s is absent from root's trie, then *this initially equals trie.end()
        TrieConstIterator(const TrieNode &root, const std::string &s)
        {
            // MUST RE-TEST

            // HANDLE whenever trie contains "" and/or s is ""
            if (s.empty())
            {
                if (root.is_final())
                {
                    // Trie contains ""
                    // this->v should not be empty
                    v.push_back({0, root});
                    return;
                }
                else
                {
                    // Trie does not contain ""
                    // this->v should remain empty
                    return;
                }
            }

            std::vector<std::pair<char, TrieNode>> pairs;

            const TrieNode *node = &root;

            for (char c : s)
            {
                if (!node->has_child_at(c))
                {
                    // s is absent from root's trie, so *this initially equals trie.end()
                    pairs.clear();
                    break;
                }
                node = &node->child_at(c);
                pairs.push_back({c, *node});
            }

            if (!pairs.empty())
            {
                if (!pairs.back().second.is_final())
                {
                    // s is absent from root's trie, so *this initially equals trie.end()
                    pairs.clear();
                }
            }

            for (auto &&pair : pairs)
            {
                v.push_back(pair);
            }
        }

    public:
        bool operator==(const TrieConstIterator &other) const noexcept
        {
            // MUST TEST

            if (v.empty() || other.v.empty())
            {
                return v.empty() == other.v.empty();
            }
            return *(*this) == *other;
        }

        bool operator!=(const TrieConstIterator &other) const noexcept
        {
            // MUST TEST

            if (v.empty() || other.v.empty())
            {
                return v.empty() != other.v.empty();
            }
            return *(*this) != *other;
        }

    public:
        // Throws std::out_of_range if v.empty()
        TrieConstIterator &operator++()
        {
            // MUST RE-TEST

            if (v.empty())
            {
                throw std::out_of_range(__PRETTY_FUNCTION__);
            }

            assert(v_back().second.is_final());

            if (!v_back().second.empty() && !v_back().second.children_empty())
            {
                depth_first_search_for_final_node(false);
                return *this;
            }

            while (!v.empty())
            {
                char c1 = v_back().first;
                v.pop_back();
                if (!v.empty())
                {
                    std::pair<char, bool> pair = v_back().second.character_of_next_non_empty_child(c1);
                    char c2 = pair.first;
                    if (pair.second)
                    {
                        assert(v_back().second.has_child_at(c1));
                        assert(v_back().second.has_child_at(c2));
                        const TrieNode &child = v_back().second.child_at(c2);
                        v.push_back({c2, child});

                        depth_first_search_for_final_node(true);
                        return *this;
                    }
                }
            }

            return *this;
        }

        std::string operator*() const
        {
            // MUST RE-TEST

            if (v.empty())
            {
                throw std::out_of_range(std::string("Scenario 1 in ") + __PRETTY_FUNCTION__);
            }
            if (v.size() == 1)
            {
                if (v.at(0).second.is_final())
                {
                    return "";
                }
                else
                {
                    throw std::out_of_range(std::string("Scenario 2 in ") + __PRETTY_FUNCTION__);
                }
            }
            std::string s;
            bool first_pair = true;
            for (auto &&pair : v)
            {
                if (!first_pair)
                {

                    s += pair.first;
                }
                first_pair = false;
            }
            return s;
        }
    };

private:
    TrieNode root;

public:
    // https://en.cppreference.com/w/cpp/utility/pair
    // https://en.cppreference.com/w/cpp/utility/pair/make_pair
    // https://en.cppreference.com/w/cpp/container/map/find
    // https://en.cppreference.com/w/cpp/container/map/insert
    // https://en.cppreference.com/w/cpp/container/map/at
    // https://en.cppreference.com/w/cpp/language/initialization
    std::pair<TrieConstIterator, bool> insert(const std::string &s) noexcept
    {
        // MUST RE-TEST

        bool already_inserted = (find(s) != end());

        // DON'T FORGET THIS EXCLAMATION POINT.
        if (!already_inserted)
        {
            root.insert(s);
        }

        return {find(s), already_inserted};
    }

    // https://en.cppreference.com/w/cpp/container/set/erase
    bool erase(const std::string &s) noexcept
    {
        // MUST RE-TEST

        bool already_inserted = (find(s) != end());

        // DON'T INCLUDE AN EXCLAMATION POINT.
        if (already_inserted)
        {
            // root.erase(s);

            // Find faster way to do this:
            std::set<std::string> set;

            for (std::string str : *this)
            {
                set.insert(str);
            }
            set.erase(s);
            clear();
            for (std::string str : set)
            {
                insert(str);
            }
        }

        // DON'T FORGET THIS EXCLAMATION POINT.
        return !already_inserted;
    }

    // https://en.cppreference.com/w/cpp/container/set/clear
    void clear() noexcept { root.clear(); }

    // https://en.cppreference.com/w/cpp/container/set/size
    auto size() const noexcept { return root.size(); }

    // https://en.cppreference.com/w/cpp/container/set/empty
    bool empty() const noexcept { return root.empty(); }

public:
    // https://en.cppreference.com/w/cpp/container/set/find
    TrieConstIterator find(const std::string &s) const noexcept { return {root, s}; }

    // https://en.cppreference.com/w/cpp/language/range-for
    // https://en.cppreference.com/w/cpp/container/set/begin
    // https://en.cppreference.com/w/cpp/language/member_functions#Member_functions_with_cv-qualifiers
    TrieConstIterator begin() const noexcept { return {root}; }

    // https://en.cppreference.com/w/cpp/language/range-for
    // https://en.cppreference.com/w/cpp/container/set/end
    // https://en.cppreference.com/w/cpp/language/member_functions#Member_functions_with_cv-qualifiers
    TrieConstIterator end() const noexcept { return {}; }

public:
    // https://en.cppreference.com/w/cpp/language/operators
    // "Stream extraction and insertion" section
    std::ostream &operator_os(std::ostream &os) const noexcept { return root.operator_os(os << '{') << '}'; }
};

#ifndef TRIE_ASSERTS
#define TRIE_ASSERTS

#define ASSERT_TRIE_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(t) assert(t.empty() == (t.size() == 0))
#define ASSERT_TRIE_EMPTY(t) assert(t.empty())
#define ASSERT_TRIE_SIZE_IS(t, expected_size) assert(t.size() == expected_size)
#define ASSERT_TRIE_CONTAINS_STRING(t, s) assert(t.find(s) != t.end())
#define ASSERT_TRIE_DOES_NOT_CONTAIN_STRING(t, s) assert(t.find(s) == t.end())

#define TRIE_INSERT_AND_ASSERT(t, s)                     \
    ASSERT_TRIE_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(t); \
    if (t.find(s) == t.end())                            \
        assert(!t.insert(s).second);                     \
    else                                                 \
        assert(t.insert(s).second);                      \
    ASSERT_TRIE_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(t); \
    ASSERT_TRIE_CONTAINS_STRING(t, s)

#define TRIE_ERASE_AND_ASSERT(t, s)                      \
    ASSERT_TRIE_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(t); \
    if (t.find(s) == t.end())                            \
        assert(t.erase(s));                              \
    else                                                 \
        assert(!t.erase(s));                             \
    ASSERT_TRIE_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(t); \
    ASSERT_TRIE_DOES_NOT_CONTAIN_STRING(t, s)

#define TRIE_EMPTY_AND_ASSERT(t)                         \
    ASSERT_TRIE_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(t); \
    ASSERT_TRIE_EMPTY(t)

#endif // TRIE_ASSERTS

#endif // SANDBOX_CPP_TRIE
