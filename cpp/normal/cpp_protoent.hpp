#ifndef SANDBOX_CPP_PROTOENT
#define SANDBOX_CPP_PROTOENT

#include <ostream>

// https://www.man7.org/linux/man-pages/man3/getprotoent.3.html
#include <netdb.h>

// https://www.man7.org/linux/man-pages/man3/getprotoent.3.html
// https://www.man7.org/linux/man-pages/man7/attributes.7.html
class cpp_protoent
{
public:
    // https://en.cppreference.com/w/cpp/language/operators
    // "Stream extraction and insertion" section
    static std::ostream &operator_os(std::ostream &os)
    {
        os << '{';
        setprotoent(0);
        protoent *pe = NULL;
        bool first_pe = true;
        do
        {
            pe = getprotoent();

            if (pe)
            {
                if (!first_pe)
                {
                    os << ',';
                }
                first_pe = false;

                os << '{';

                if (pe->p_name)
                {
                    os << '\"' << pe->p_name << '\"';
                }
                else
                {
                    os << "NULL";
                }

                os << ',';

                if (pe->p_aliases)
                {
                    os << '{';

                    bool first_alias = true;

                    for (int i = 0; pe->p_aliases[i]; i++)
                    {
                        if (!first_alias)
                        {
                            os << ',';
                        }

                        os << '\"' << pe->p_aliases[i] << '\"';

                        first_alias = false;
                    }

                    os << '}';
                }
                else
                {
                    os << "NULL";
                }

                os << ',' << pe->p_proto << '}';
            }
        } while (pe);

        endprotoent();
        return os << '}';
    }
};

#endif // SANDBOX_CPP_PROTOENT
