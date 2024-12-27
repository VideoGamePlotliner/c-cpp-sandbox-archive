#ifndef SANDBOX_CPP_ARGS
#define SANDBOX_CPP_ARGS

#include <iostream>
#include <stdexcept>
#include <string>

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
    args(int argc, char const *argv[]); // May throw std::invalid_argument
    args(const args &other) noexcept;
    ~args() noexcept;
    args &operator=(const args &other) noexcept;

private:
    void clear() noexcept; // Should be private

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
        bool operator!=(const iterator &other) const noexcept { return current_index != other.current_index; }

    public:
        iterator &operator++() noexcept
        {
            current_index++;
            return *this;
        }

    public:
        // May throw std::out_of_range
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
        bool operator!=(const const_iterator &other) const noexcept { return current_index != other.current_index; }

    public:
        const_iterator &operator++() noexcept
        {
            current_index++;
            return *this;
        }

    public:
        // May throw std::out_of_range
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
    std::ostream &operator_os(std::ostream &os) const noexcept;
};

#endif // SANDBOX_CPP_ARGS
