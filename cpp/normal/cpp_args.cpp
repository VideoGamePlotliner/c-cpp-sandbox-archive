#include "cpp_args.hpp"

#include <stdexcept>
#include <cstring>

//
//
//
//
//
//
// args
//
//

args::args(int argc, char const *argv[]) : c(0), v(nullptr)
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
                auto len = std::strlen(argv[i]);

                if (len > 0)
                {
                    char *vi = new char[len + 1];
                    std::strncpy(vi, argv[i], len);
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

args::args(const args &other) noexcept : c(0), v(nullptr)
{
    *this = other;
}

args::~args() noexcept
{
    clear();
}

args &args::operator=(const args &other) noexcept
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
                auto len = std::strlen(other.v[i]);

                if (len > 0)
                {
                    char *vi = new char[len + 1];
                    std::strncpy(vi, other.v[i], len);
                    vi[len] = 0; // NOT v[len] = 0
                    v[i] = vi;
                }
                else
                {
                    v[i] = nullptr; // NOT v = nullptr
                }
            }
        }
    }
    return *this;
}

void args::clear() noexcept
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

std::ostream &args::operator_os(std::ostream &os) const noexcept
{
    os << '{' << c << ",{";
    for (int i = 0; i < c; i++)
    {
        if (i > 0)
        {
            os << ',';
        }
        os << "{\"" << v[i] << "\"}";
    }
    return os << "}}";
}
