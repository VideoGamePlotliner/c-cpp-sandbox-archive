#include <iostream>

// https://en.cppreference.com/w/cpp/language/escape
// https://en.cppreference.com/w/cpp/language/string_literal
static void append_escape(std::string &s, int i)
{
    switch ((char)i)
    {
    case '\'':
        s += "\\\'";
        break;
    case '\"':
        s += "\\\"";
        break;
    case '\?':
        s += "\\\?";
        break;
    case '\\':
        s += "\\\\";
        break;
    case '\a':
        s += "\\a";
        break;
    case '\b':
        s += "\\b";
        break;
    case '\f':
        s += "\\f";
        break;
    case '\n':
        s += "\\n";
        break;
    case '\r':
        s += "\\r";
        break;
    case '\t':
        s += "\\t";
        break;
    case '\v':
        s += "\\v";
        break;
    case '\0':
        s += "\\0";
        break;
    default:
        if ((i >= 1 && i <= 6) || (i >= 14 && i <= 31) || i == 127)
        {
            char x = i / 16;

            // Here, i is at least 1 and less than 128,
            // so x is at least 0 and less than 8,
            // so it is implied that !(x > 9).
            //
            // if (x > 9)
            // {
            //     x += 'a' - 10;
            // }
            // else
            {
                x += '0';
            }
            char y = i % 16;
            if (y > 9)
            {
                y += 'a' - 10;
            }
            else
            {
                y += '0';
            }
            s += "\\x";
            s += x;
            s += y;
        }
        else if (i >= 0 && i <= 127)
        {
            s += ((char)i);
        }
        break;
    }
}

// https://en.cppreference.com/w/cpp/language/escape
// https://en.cppreference.com/w/cpp/language/string_literal
static std::string formula_for_CHAR_TO_STRING_LITERAL(int i = 0)
{
    if (i < 0 || i > 127)
    {
        return "\"Could not convert this value because this value is not an integer from 0 to 127.\"";
    }
    else
    {
        std::string s;
        s += "((c == '";
        append_escape(s, i);
        s += "') ? \"'";
        if ((i >= 0 && i <= 31) || i == 127)
        {
            s += '\\';
        }
        else if (i == '\'' || i == '\"' || i == '\?' || i == '\\')
        {
            s += "\\\\";
        }
        append_escape(s, i);
        s += "'\" : \\\n";
        s += formula_for_CHAR_TO_STRING_LITERAL(i + 1);
        s += ")";
        return s;
    }
}

int main(int argc, char const *argv[])
{
    std::cout << "#ifndef SANDBOX_GEN_TRIE";
    std::cout << '\n';
    std::cout << "#define SANDBOX_GEN_TRIE";
    std::cout << '\n';
    std::cout << "";
    std::cout << '\n';
    std::cout << "// This file was generated from one or more other files.";
    std::cout << '\n';
    std::cout << "// Hence, changes made to this file will be destroyed if";
    std::cout << '\n';
    std::cout << "// the generation program for this file is run again.";
    std::cout << '\n';
    std::cout << "";
    std::cout << '\n';
    std::cout << "#ifndef CHAR_TO_STRING_LITERAL";
    std::cout << '\n';
    std::cout << "// https://en.cppreference.com/w/cpp/language/string_literal";
    std::cout << '\n';
    std::cout << "// https://en.cppreference.com/w/cpp/language/escape";
    std::cout << '\n';
    std::cout << "#define CHAR_TO_STRING_LITERAL(c) \\";
    std::cout << '\n';
    std::cout << formula_for_CHAR_TO_STRING_LITERAL();
    std::cout << '\n';
    std::cout << "#endif // CHAR_TO_STRING_LITERAL";
    std::cout << '\n';
    std::cout << "";
    std::cout << '\n';
    std::cout << "#endif // SANDBOX_GEN_TRIE";
    std::cout << '\n';
    return 0;
}
