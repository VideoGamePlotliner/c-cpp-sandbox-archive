#ifndef SANDBOX_CPP_TRIE
#define SANDBOX_CPP_TRIE

#include <vector>
#include <string>
#include <set>
#include <map>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <utility>
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
        // https://en.cppreference.com/w/cpp/container/map/find
        // https://en.cppreference.com/w/cpp/container/map/end
        // https://en.cppreference.com/w/cpp/container/map/at
        // https://en.cppreference.com/w/cpp/container/map/empty
        // https://en.cppreference.com/w/cpp/container/map/begin
        // https://en.cppreference.com/w/cpp/container/map/clear
        // https://en.cppreference.com/w/cpp/container/map/insert
        // https://en.cppreference.com/w/cpp/container/map/size
        // https://en.cppreference.com/w/cpp/container/map/erase
        // https://en.cppreference.com/w/cpp/utility/pair/make_pair

    private:
        // Is this node the final node of a string in this trie?
        bool is_final_node = false;

        // The list of child nodes and the characters they correspond to
        std::map<char, TrieNode *> children;

    public:
        TrieNode() = default;
        TrieNode(const TrieNode &other) { *this = other; }
        ~TrieNode() { clear(); }
        TrieNode &operator=(const TrieNode &other)
        {
            if (this != &other)
            {
                clear();

                is_final_node = other.is_final_node;

                for (auto &&pair : other.children)
                {
                    char c = pair.first;
                    const TrieNode *child = pair.second;
                    if (child)
                    {
                        assert(insert_child_at(c, new TrieNode(*child)));
                    }
                    else
                    {
                        assert(insert_child_at(c, nullptr));
                    }
                }
            }
            return *this;
        };

    public:
        bool is_final() const { return is_final_node; }
        void mark_as_final() { is_final_node = true; }
        void mark_as_non_final() { is_final_node = false; }
        bool has_child_at(char c) const { return children.find(c) != children.end(); }
        TrieNode *child_at(char c) { return children.at(c); }
        const TrieNode *child_at(char c) const { return children.at(c); }
        bool has_no_children() const { return children.empty(); }
        bool insert_child_at(char c, TrieNode *new_child) { return children.insert(std::make_pair(c, new_child)).second; }
        bool erase_child_at(char c) { return static_cast<bool>(children.erase(c)); }

    public:
        // The 'bool' part of the return value is whether this node has any
        // non-empty children. If the 'bool' part of the return value is
        // false, then the 'char' part of the return value is meaningless
        // and irrelevant.
        std::pair<char, bool> character_of_first_non_empty_child() const
        {
            for (auto &&pair : children)
            {
                if (pair.second && !pair.second->empty())
                {
                    char c = pair.first;
                    return std::make_pair(c, true);
                }
            }
            return std::make_pair(0, false);
        }

        // The 'bool' part of the return value is whether this node has at
        // least one non-empty child whose corresponding character is greater
        // than the parameter. If the 'bool' part of the return value is
        // false, then the 'char' part of the return value is meaningless and
        // irrelevant.
        std::pair<char, bool> character_of_next_non_empty_child(char infimum) const
        {
            for (auto &&pair : children)
            {
                char c = pair.first;
                if (c > infimum && pair.second && !pair.second->empty())
                {
                    return std::make_pair(c, true);
                }
            }
            return std::make_pair(0, false);
        }

        // https://en.cppreference.com/w/cpp/container/set/size
        // 'size_type' in https://en.cppreference.com/w/cpp/container/set
        std::size_t size() const
        {
            std::size_t size_so_far = (is_final_node ? 1 : 0);
            for (auto &&pair : children)
            {
                if (pair.second)
                {
                    size_so_far += pair.second->size();
                }
            }
            return size_so_far;
        }

        // If this TrieNode both is non-final and has zero non-empty
        // children, return true. Otherwise, return false.
        bool empty() const { return !is_final_node && !character_of_first_non_empty_child().second; }

        // https://en.cppreference.com/w/cpp/container/set/clear
        void clear()
        {
            is_final_node = false;

            for (auto &&pair : children)
            {
                TrieNode *child = pair.second;
                pair.second = nullptr;
                if (child)
                {
                    child->clear();
                    assert(child->has_no_children());
                    delete child;
                }
            }
            children.clear();
        }

        // https://en.cppreference.com/w/cpp/language/operators
        // "Stream extraction and insertion" section
        std::ostream &operator_os(std::ostream &os) const
        {
            os << (is_final_node ? "{true,{" : "{false,{");

            bool first_pair = true;

            for (auto &&pair : children)
            {
                char c = pair.first;
                os << '{' << CHAR_TO_STRING_LITERAL(c) << ',';

                if (pair.second)
                {
                    pair.second->operator_os(os) << '}';
                }
                else
                {
                    os << "nullptr}";
                }

                if (!first_pair)
                {
                    os << ',';
                }
                first_pair = false;
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
        // The return value of *(*this) -- unless at_end is true.
        std::string s;

        // Equal to &trie.root; not equal to nullptr.
        const TrieNode *const root;

        // Does this TrieConstIterator equal trie.end()?
        bool at_end = false;

    public:
        // If str is absent from root's trie, then s initially equals "",
        // at_end initially equals true, and *this initially equals trie.end().
        TrieConstIterator(const std::string &str, const TrieNode &root_node, bool starting_at_end) : s(str), root(&root_node), at_end(starting_at_end)
        {
            if (at_end)
            {
                assert(s.empty());
            }
            else if (s.empty())
            {
                // Root's trie contains "" if and only if root->is_final()
                at_end = !root->is_final();
            }
            else
            {
                const TrieNode *node = root;

                for (char c : s)
                {
                    if (node && node->has_child_at(c))
                    {
                        node = node->child_at(c);
                    }
                    else
                    {
                        // s is absent from root's trie, so *this initially equals trie.end()
                        at_end = true;
                        break;
                    }
                }

                if (at_end)
                {
                    // s is absent from root's trie, so *this initially equals trie.end()
                    s.clear();
                }
                else if (node && node->is_final())
                {
                    // s is present in root's trie, so *this does not initially equal trie.end()
                    at_end = false;
                }
                else
                {
                    // s is absent from root's trie, so *this initially equals trie.end()
                    at_end = true;
                }

                if (at_end)
                {
                    // s is absent from root's trie, so *this initially equals trie.end()
                    s.clear();
                }
            }
            assert(root);
        }

    public:
        bool operator==(const TrieConstIterator &other) const { return (at_end == other.at_end) && (s == other.s) && (root == other.root); }

        bool operator!=(const TrieConstIterator &other) const { return !((*this) == other); }

    private:
        // The same logic as trie.find(s) != trie.end()
        bool root_contains_s() const
        {
            TrieConstIterator other(s, *root, false);
            return !other.at_end;
        }

    public:
        // https://en.cppreference.com/w/cpp/error/out_of_range
        // Will throw std::out_of_range if and only if at_end is true
        const std::string &operator*() const
        {
            if (at_end)
            {
                throw std::out_of_range(__PRETTY_FUNCTION__);
            }
            assert(root_contains_s());
            return s;
        }

        // https://en.cppreference.com/w/cpp/error/out_of_range
        // Will throw std::out_of_range if and only if at_end is true right before this function is called
        TrieConstIterator &operator++()
        {
            if (at_end)
            {
                throw std::out_of_range(__PRETTY_FUNCTION__);
            }

            assert(root_contains_s());

            // IMPORTANT EDGE CASE: trie only contains ""
            if (root->size() == 1 && s.empty() && root->is_final())
            {
                at_end = true;
                return *this;
            }

            const std::string s_before_change = s;
            std::vector<const TrieNode *> node_stack;

            // Set up the node stack.
            node_stack.push_back(root);
            for (char c : s)
            {
                if (!node_stack.empty() && v_back(node_stack) && v_back(node_stack)->has_child_at(c))
                {
                    node_stack.push_back(v_back(node_stack)->child_at(c));
                }
            }

            assert(!node_stack.empty());

            if (v_back(node_stack))
            {
                assert(v_back(node_stack)->is_final());
            }

            // Try to move as far down as possible.
            bool has_moved_at_all_yet = false;
            while (!node_stack.empty() && v_back(node_stack) && (!has_moved_at_all_yet || s == s_before_change || !v_back(node_stack)->is_final()))
            {
                std::pair<char, bool> pair = v_back(node_stack)->character_of_first_non_empty_child();
                if (!pair.second)
                {
                    // Can't move down. Try to move right one step.
                    bool has_moved_right_yet = false;
                    while (!has_moved_right_yet && !s.empty() && node_stack.size() > 1)
                    {
                        char infimum = s_back();
                        std::pair<char, bool> pair_next = v_penultimate(node_stack)->character_of_next_non_empty_child(infimum);
                        if (!pair_next.second)
                        {
                            // Can't move right. Move up one step.
                            s_pop_back();
                            v_pop_back(node_stack);
                            has_moved_at_all_yet = true;
                        }
                        else
                        {
                            // Move right one step.
                            char c_next = pair_next.first;
                            s_back() = c_next;
                            v_pop_back(node_stack);
                            node_stack.push_back(v_back(node_stack)->child_at(c_next));
                            has_moved_right_yet = true;
                            has_moved_at_all_yet = true;
                        }
                    }

                    if (!at_end && s.empty() && node_stack.size() == 1 && v_back(node_stack) == root)
                    {
                        at_end = true;
                        return *this;
                    }
                }
                else
                {
                    // Move down one step.
                    char c = pair.first;
                    s += c;
                    node_stack.push_back(v_back(node_stack)->child_at(c));
                    has_moved_at_all_yet = true;
                }
            }

            assert(!node_stack.empty());
            assert((v_back(node_stack) && !v_back(node_stack)->is_final()) == at_end);

            return *this;
        }

    private:
        // https://en.cppreference.com/w/cpp/string/basic_string/back
        // https://en.cppreference.com/w/cpp/error/out_of_range
        // Will throw std::out_of_range if and only if s is empty
        char &s_back()
        {
            if (s.empty())
            {
                throw std::out_of_range(__PRETTY_FUNCTION__);
            }
            return s.at(s.size() - 1);
        }

        // https://en.cppreference.com/w/cpp/string/basic_string/pop_back
        // https://en.cppreference.com/w/cpp/error/out_of_range
        // Will throw std::out_of_range if and only if s is empty
        void s_pop_back()
        {
            if (s.empty())
            {
                throw std::out_of_range(__PRETTY_FUNCTION__);
            }
            s.pop_back();
        }
    };

private:
    // https://en.cppreference.com/w/cpp/container/vector/pop_back
    // https://en.cppreference.com/w/cpp/error/out_of_range
    // Will throw std::out_of_range if and only if v is empty
    static void v_pop_back(std::vector<TrieNode *> &v)
    {
        if (v.empty())
        {
            throw std::out_of_range(__PRETTY_FUNCTION__);
        }
        v.pop_back();
    }

    // https://en.cppreference.com/w/cpp/container/vector/pop_back
    // https://en.cppreference.com/w/cpp/error/out_of_range
    // Will throw std::out_of_range if and only if v is empty
    static void v_pop_back(std::vector<const TrieNode *> &v)
    {
        if (v.empty())
        {
            throw std::out_of_range(__PRETTY_FUNCTION__);
        }
        v.pop_back();
    }

    // https://en.cppreference.com/w/cpp/container/vector/back
    // https://en.cppreference.com/w/cpp/error/out_of_range
    // Will throw std::out_of_range if and only if v is empty
    static TrieNode *v_back(const std::vector<TrieNode *> &v)
    {
        if (v.empty())
        {
            throw std::out_of_range(__PRETTY_FUNCTION__);
        }
        return v.at(v.size() - 1);
    }

    // https://en.cppreference.com/w/cpp/container/vector/back
    // https://en.cppreference.com/w/cpp/error/out_of_range
    // Will throw std::out_of_range if and only if v is empty
    static const TrieNode *v_back(const std::vector<const TrieNode *> &v)
    {
        if (v.empty())
        {
            throw std::out_of_range(__PRETTY_FUNCTION__);
        }
        return v.at(v.size() - 1);
    }

    // https://en.cppreference.com/w/cpp/error/out_of_range
    // Will throw std::out_of_range if and only if v.size() < 2
    static const TrieNode *v_penultimate(const std::vector<const TrieNode *> &v)
    {
        if (v.size() < 2)
        {
            throw std::out_of_range(__PRETTY_FUNCTION__);
        }
        return v.at(v.size() - 2);
    }

private:
    TrieNode root;

public:
    // https://en.cppreference.com/w/cpp/utility/pair
    // https://en.cppreference.com/w/cpp/utility/pair/make_pair
    // https://en.cppreference.com/w/cpp/container/map/find
    // https://en.cppreference.com/w/cpp/container/map/insert
    // https://en.cppreference.com/w/cpp/container/map/at
    // https://en.cppreference.com/w/cpp/language/initialization
    // https://en.cppreference.com/w/cpp/container/set/insert
    // If insertion takes place, the 'bool' part of the return value is true. Otherwise, return false.
    std::pair<TrieConstIterator, bool> insert(const std::string &s)
    {
        bool should_insert = (find(s) == end());

        if (/* DON'T INCLUDE AN EXCLAMATION POINT HERE */ should_insert)
        {
            TrieNode *node = &root;
            for (char c : s)
            {
                if (node && !node->has_child_at(c))
                {
                    assert(node->insert_child_at(c, new TrieNode));
                }
                if (node)
                {
                    node = node->child_at(c);
                }
            }
            if (node)
            {
                node->mark_as_final();
            }
            else
            {
                should_insert = false;
            }
        }

        return std::make_pair(find(s), should_insert);
    }

    // https://en.cppreference.com/w/cpp/container/set/erase
    // If erasure takes place, return true. Otherwise, return false.
    bool erase(const std::string &s)
    {
        bool should_erase = (find(s) != end());

        if (should_erase)
        {
            if (s.empty())
            {
                root.mark_as_non_final();
            }
            else
            {
                std::vector<TrieNode *> node_stack;

                // Set up the node stack.
                node_stack.push_back(&root);
                for (char c : s)
                {
                    if (!node_stack.empty() && v_back(node_stack) && v_back(node_stack)->has_child_at(c))
                    {
                        node_stack.push_back(v_back(node_stack)->child_at(c));
                    }
                }

                assert(node_stack.size() > 1);

                if (v_back(node_stack))
                {
                    assert(v_back(node_stack)->is_final());

                    if (v_back(node_stack)->has_no_children())
                    {
                        int i = s.length() - 1;
                        do
                        {
                            const TrieNode *old_back = v_back(node_stack);
                            v_pop_back(node_stack);
                            TrieNode *new_back = v_back(node_stack);
                            if (new_back)
                            {
                                char c = s.at(i);
                                assert(new_back->erase_child_at(c));
                            }
                            if (old_back)
                            {
                                assert(old_back->has_no_children());
                                delete old_back;
                            }
                            i--;
                        } while (node_stack.size() > 1 && v_back(node_stack) && !v_back(node_stack)->is_final() && v_back(node_stack)->has_no_children());
                    }
                    else
                    {
                        v_back(node_stack)->mark_as_non_final();
                    }
                }
                else
                {
                    should_erase = false;
                }
            }
        }
        return should_erase;
    }

    // https://en.cppreference.com/w/cpp/container/set/clear
    void clear() { root.clear(); }

    // https://en.cppreference.com/w/cpp/container/set/size
    // 'size_type' in https://en.cppreference.com/w/cpp/container/set
    std::size_t size() const { return root.size(); }

    // https://en.cppreference.com/w/cpp/container/set/empty
    bool empty() const { return root.empty(); }

public:
    // https://en.cppreference.com/w/cpp/container/set/find
    // If s is absent from this trie, return trie.end().
    TrieConstIterator find(const std::string &s) const { return TrieConstIterator(s, root, false); }

    // https://en.cppreference.com/w/cpp/language/range-for
    // https://en.cppreference.com/w/cpp/container/set/begin
    // https://en.cppreference.com/w/cpp/language/member_functions#Member_functions_with_cv-qualifiers
    TrieConstIterator begin() const
    {
        std::string s;
        const TrieNode *node = &root;
        while (node && !node->is_final())
        {
            std::pair<char, bool> pair = node->character_of_first_non_empty_child();
            if (!pair.second)
            {
                return end();
            }
            char c = pair.first;
            s += c;
            node = node->child_at(c);
        }
        return find(s);
    }

    // https://en.cppreference.com/w/cpp/language/range-for
    // https://en.cppreference.com/w/cpp/container/set/end
    // https://en.cppreference.com/w/cpp/language/member_functions#Member_functions_with_cv-qualifiers
    TrieConstIterator end() const { return TrieConstIterator("", root, true); }

public:
    // https://en.cppreference.com/w/cpp/language/operators
    // "Stream extraction and insertion" section
    std::ostream &operator_os(std::ostream &os) const { return root.operator_os(os << '{') << '}'; }
};

#endif // SANDBOX_CPP_TRIE
