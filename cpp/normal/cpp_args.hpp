#ifndef SANDBOX_CPP_ARGS
#define SANDBOX_CPP_ARGS

#include <iostream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <cassert>
#include <initializer_list>

//
//
//
//
//
//
// args
//
//
class args final
{
private:
    // https://en.cppreference.com/w/cpp/error/exception
    // https://en.cppreference.com/w/cpp/error/invalid_argument
    // https://en.cppreference.com/w/cpp/language/new
    // https://en.cppreference.com/w/cpp/language/initialization

private:
    int c = 0;          // argc
    char **v = nullptr; // argv

public:
    // May throw std::invalid_argument
    args(int argc, char const *argv[]) : c(0), v(nullptr)
    {
        if (argc < 0)
        {
            std::string s(__PRETTY_FUNCTION__);
            s += " -- argc must not be negative";
            throw std::invalid_argument(s);
        }
        else if (argc == 0)
        {
            if (argv)
            {
                std::string s(__PRETTY_FUNCTION__);
                s += " -- If argc is zero, then argv must be nullptr.";
                throw std::invalid_argument(s);
            }
        }
        else
        {
            // argc > 0

            if (argv)
            {
                c = argc;

                v = new char *[c];

                for (int i = 0; i < c; i++)
                {
                    if (argv[i])
                    {
                        auto len = std::strlen(argv[i]);
                        char *vi = new char[len + 1];
                        if (len > 0)
                        {
                            std::strncpy(vi, argv[i], len);
                        }
                        vi[len] = 0; // NOT v[len] = 0
                        v[i] = vi;
                    }
                    else
                    {
                        v[i] = nullptr;
                    }
                }
            }
            else
            {
                std::string s(__PRETTY_FUNCTION__);
                s += " -- If argc is positive, then argv must be a nonzero pointer.";
                throw std::invalid_argument(s);
            }
        }
    }

    args(const args &other) noexcept : c(0), v(nullptr) { *this = other; }
    ~args() noexcept { clear(); }

    args &operator=(const args &other) noexcept
    {
        if (this != &other)
        {
            clear();

            if ((other.c > 0) && other.v)
            {
                c = other.c;

                v = new char *[c];

                for (int i = 0; i < c; i++)
                {
                    if (other.v[i])
                    {
                        auto len = std::strlen(other.v[i]);
                        char *vi = new char[len + 1];
                        if (len > 0)
                        {
                            std::strncpy(vi, other.v[i], len);
                        }
                        vi[len] = 0; // NOT v[len] = 0
                        v[i] = vi;
                    }
                    else
                    {
                        v[i] = nullptr;
                    }
                }
            }
        }
        return *this;
    }

private:
    //
    // https://en.cppreference.com/w/cpp/language/initialization
    // "If the initializer is of syntax (3), the object is direct-initialized."
    //
    // https://en.cppreference.com/w/cpp/language/direct_initialization
    // "2) Initialization of an object of non-class type with a single brace-
    // enclosed initializer (note: for class types and other uses of braced-
    // init-list, see list-initialization)(since C++11)."
    //
    // https://en.cppreference.com/w/cpp/language/list_initialization
    // "Initializes an object from a brace-enclosed initializer list."
    //
    // https://en.cppreference.com/w/cpp/language/initialization
    // "Syntaxes (2-4) [for an initializer clause] are collectively called
    // brace-enclosed initializer list."
    //
    // https://en.cppreference.com/w/cpp/utility/initializer_list
    //

public:
    // May throw std::invalid_argument
    args(std::initializer_list<const char *> il) : c(0), v(nullptr)
    {
        int argc = static_cast<int>(il.size());
        if (argc < 0)
        {
            std::string s(__PRETTY_FUNCTION__);
            s += " -- static_cast<int>(il.size()) must not be negative";
            throw std::invalid_argument(s);
        }
        else if (argc == 0)
        {
            c = 0;
            v = nullptr;
        }
        else
        {
            // argc > 0

            c = argc;

            v = new char *[c];

            int i = 0;
            for (const char *argv_i : il)
            {
                if (i < c)
                {
                    if (argv_i)
                    {
                        auto len = std::strlen(argv_i);
                        char *vi = new char[len + 1];
                        if (len > 0)
                        {
                            std::strncpy(vi, argv_i, len);
                        }
                        vi[len] = 0; // NOT v[len] = 0
                        v[i] = vi;
                    }
                    else
                    {
                        v[i] = nullptr;
                    }
                }
                i++;
            }
        }
    }

private:
    // Should be private
    void clear() noexcept
    {
        if (v)
        {
            for (int i = 0; i < c; i++)
            {
                if (v[i])
                {
                    delete[] (v[i]);
                    v[i] = nullptr;
                }
            }
            delete[] v;
            v = nullptr;
        }
        c = 0;
    }

private:
    // https://en.cppreference.com/w/cpp/language/range-for
    // https://en.cppreference.com/w/cpp/language/member_functions#Member_functions_with_cv-qualifiers
    // https://en.cppreference.com/w/cpp/container/vector/begin
    // https://en.cppreference.com/w/cpp/container/vector/end
    // https://en.cppreference.com/w/cpp/container/array/begin
    // https://en.cppreference.com/w/cpp/container/array/end

private:
    class iterator final
    {
    private:
        int current_index = 0;
        args &args_ref;

    public:
        iterator(int initial_index, args &a) : current_index(initial_index), args_ref(a) {}

    public:
        bool operator!=(const iterator &other) const noexcept
        {
            assert(&args_ref == &other.args_ref);
            return current_index != other.current_index;
        }

    public:
        iterator &operator++() noexcept
        {
            current_index++;
            return *this;
        }

    public:
        // May throw std::out_of_range
        // Be careful with this function's return value, because it might be nullptr.
        auto &operator*()
        {
            if (current_index < 0 || current_index >= args_ref.c)
            {
                std::string s(__PRETTY_FUNCTION__);
                s += " -- Cannot dereference value at index ";
                s += std::to_string(current_index);
                s += ", because argc is ";
                s += std::to_string(args_ref.c);
                throw std::out_of_range(s);
            }
            return args_ref.v[current_index];
        }
    };

    class const_iterator final
    {
    private:
        int current_index = 0;
        const args &args_ref;

    public:
        const_iterator(int initial_index, const args &a) : current_index(initial_index), args_ref(a) {}

    public:
        bool operator!=(const const_iterator &other) const noexcept
        {
            assert(&args_ref == &other.args_ref);
            return current_index != other.current_index;
        }

    public:
        const_iterator &operator++() noexcept
        {
            current_index++;
            return *this;
        }

    public:
        // May throw std::out_of_range
        // Be careful with this function's return value, because it might be nullptr.
        const auto &operator*() const
        {
            if (current_index < 0 || current_index >= args_ref.c)
            {
                std::string s(__PRETTY_FUNCTION__);
                s += " -- Cannot dereference value at index ";
                s += std::to_string(current_index);
                s += ", because argc is ";
                s += std::to_string(args_ref.c);
                throw std::out_of_range(s);
            }
            return args_ref.v[current_index];
        }
    };

public:
    auto begin() noexcept { return iterator(0, *this); }
    auto end() noexcept { return iterator(c, *this); }
    auto begin() const noexcept { return const_iterator(0, *this); }
    auto end() const noexcept { return const_iterator(c, *this); }
    auto cbegin() const noexcept { return const_iterator(0, *this); }
    auto cend() const noexcept { return const_iterator(c, *this); }

public:
    // https://en.cppreference.com/w/cpp/language/operators
    // "Stream extraction and insertion" section
    std::ostream &operator_os(std::ostream &os) const noexcept
    {
        os << '{' << c << ',';
        if (v)
        {
            os << '{';
            for (int i = 0; i < c; i++)
            {
                if (i > 0)
                {
                    os << ',';
                }

                if (v[i])
                {
                    os << '\"' << v[i] << '\"';
                }
                else
                {
                    os << "nullptr";
                }
            }
            os << '}';
        }
        else
        {
            os << "nullptr";
        }
        return os << '}';
    }
};

#endif // SANDBOX_CPP_ARGS
