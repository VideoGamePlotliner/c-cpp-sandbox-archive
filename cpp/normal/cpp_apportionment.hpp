#ifndef SANDBOX_CPP_APPORTIONMENT
#define SANDBOX_CPP_APPORTIONMENT

#include <cmath>
#include <cstdint>
#include <map>
#include <string>
#include <stdexcept>
#include <cassert>
#include <iostream>

// On 27 Jan 2025,
// https://www.census.gov/topics/public-sector/congressional-apportionment/about/computing.html
// said,
//
// Computing Apportionment
//
// Article 1, Section 2, of the United States Constitution states:
//
// "Representatives and direct Taxes shall be apportioned among the several States which
// may be included within this Union, according to their respective Numbers...The actual
// Enumeration shall be made within three Years after the first Meeting of the Congress
// of the United States, and within every subsequent Term of ten Years, in such Manner as
// they shall by Law direct."
//
// Therein lies the primary mandate of the U.S. census, apportionment of the House of
// Representatives. Since that first census in 1790, five methods of apportionment have
// been used. The current method used, the Method of Equal Proportions, was adopted by
// congress in 1941 following the census of 1940. This method assigns seats in the House
// of Representatives according to a "priority" value. The priority value is determined
// by multiplying the population of a state by a "multiplier."
//
// Each of the 50 states is given one seat out of the current total of 435. The next, or
// 51st seat, goes to the state with the highest priority value and becomes that state's
// second seat. This continues until all 435 seats have been assigned to a state. This is
// how it is done.
//
// Equal Proportions Method
//
// P - represents a state's total population
//
// n - represents the number of seats a state would have if it gained a seat (because all
//     states automatically received one seat the next seat gained is "seat two," and the
//     next "seat three," and the next "seat four," and so on.)
//
// The multiplier equals:
// 1 / sqrt(n * (n - 1))
//
// ...
//
// Continue until an appropriate number of multipliers have been calculated.
//
// Once the "multipliers" have been calculated, the next step is to multiply this figure
// by the population total for each of the 50 states (the District of Columbia is not
// included in these calculations). The resulting numbers are the priority values. Make
// sure you compute enough multipliers and priority values to cover the largest number of
// seats in the U.S. House of Representatives that any one state stands to gain. For
// example, if the largest number of seats currently assigned to a state is 60,
// multipliers and priority values must be calculated for at least the 60th seat. If you
// are using a computer, you should compute multipliers for seats 2 through 70. This will
// assure you have enough multipliers and priority values for apportionment.
//
// Once you've calculated priority values for the total number of potential seats for
// each state, the next step is to rank and assign seat numbers to the resulting priority
// values starting with seat 51, until all 435 seats have been assigned (remember, each
// state automatically received one seat). Next, tally the number of seats for each state
// to arrive at the total number of seats in the House of Representatives apportioned to
// each state.

//
//
//
//
//
//
// apportionment
//
//
class apportionment final
{
private:
    // https://en.cppreference.com/w/cpp/container/map
    // https://en.cppreference.com/w/cpp/container/map/at
    // https://en.cppreference.com/w/cpp/container/map/find
    // https://en.cppreference.com/w/cpp/container/map/end
    // https://en.cppreference.com/w/cpp/container/map/insert
    // https://en.cppreference.com/w/cpp/container/map/size
    // https://en.cppreference.com/w/cpp/container/map/clear
    // https://en.cppreference.com/w/cpp/utility/pair
    // https://en.cppreference.com/w/cpp/utility/pair/make_pair
    // https://en.cppreference.com/w/cpp/string/basic_string
    // https://en.cppreference.com/w/cpp/string/basic_string/to_string
    // https://en.cppreference.com/w/cpp/language/typedef
    // https://en.cppreference.com/w/cpp/language/floating_literal
    // https://en.cppreference.com/w/cpp/types/integer
    // https://en.cppreference.com/w/cpp/numeric/math/sqrt
    // https://en.cppreference.com/w/cpp/error/invalid_argument
    // https://en.cppreference.com/w/cpp/error/assert

public:
    // A state's number of U.S. Representatives.
    // These numbers should be less than UINT32_MAX.
    typedef uint32_t R;

    // A state's population.
    // These numbers should be positive.
    typedef uint64_t P;

    // A state's name.
    // These strings should be non-empty.
    typedef std::string N;

    // Each key is the name of a given state. Each corresponding mapped value is the
    // number of U.S. Representatives that that state should have, given a size of the
    // U.S. House of Representatives.
    typedef std::map<N, R> M;

private:
    // Each key is the name of a given state. Each corresponding mapped value is the
    // population of that state. There should be at least one key.
    const std::map<N, P> population_for_each_state;

    // Each key is a potential size of the U.S. House of Representatives. Each key should
    // be at least the number of states. Also, each key present in `population_for_each_state`
    // must be present in `num_reps_for_each_size_and_state`, and vice versa.
    std::map<R, M> num_reps_for_each_size_and_state;

public:
    apportionment() : apportionment(us_2020_populations()) {}
    apportionment(const std::map<N, P> &the_population_for_each_state) : population_for_each_state(the_population_for_each_state) {}

private:
    // https://www2.census.gov/programs-surveys/decennial/2020/data/apportionment/apportionment-2020-tableA.xlsx
    // As of 27 Jan 2025
    static std::map<N, P> us_2020_populations()
    {
        return {
            {"Alabama", 5030053},
            {"Alaska", 736081},
            {"Arizona", 7158923},
            {"Arkansas", 3013756},
            {"California", 39576757},
            {"Colorado", 5782171},
            {"Connecticut", 3608298},
            {"Delaware", 990837},
            // {"District of Columbia", (689545 + 1988)},
            {"Florida", 21570527},
            {"Georgia", 10725274},
            {"Hawaii", 1460137},
            {"Idaho", 1841377},
            {"Illinois", 12822739},
            {"Indiana", 6790280},
            {"Iowa", 3192406},
            {"Kansas", 2940865},
            {"Kentucky", 4509342},
            {"Louisiana", 4661468},
            {"Maine", 1363582},
            {"Maryland", 6185278},
            {"Massachusetts", 7033469},
            {"Michigan", 10084442},
            {"Minnesota", 5709752},
            {"Mississippi", 2963914},
            {"Missouri", 6160281},
            {"Montana", 1085407},
            {"Nebraska", 1963333},
            {"Nevada", 3108462},
            {"New Hampshire", 1379089},
            {"New Jersey", 9294493},
            {"New Mexico", 2120220},
            {"New York", 20215751},
            {"North Carolina", 10453948},
            {"North Dakota", 779702},
            {"Ohio", 11808848},
            {"Oklahoma", 3963516},
            {"Oregon", 4241500},
            {"Pennsylvania", 13011844},
            {"Rhode Island", 1098163},
            {"South Carolina", 5124712},
            {"South Dakota", 887770},
            {"Tennessee", 6916897},
            {"Texas", 29183290},
            {"Utah", 3275252},
            {"Vermont", 643503},
            {"Virginia", 8654542},
            {"Washington", 7715946},
            {"West Virginia", 1795045},
            {"Wisconsin", 5897473},
            {"Wyoming", 577719}
            // ,{"Puerto Rico", (3285874 + 0)}
        };
    };

    // https://www2.census.gov/programs-surveys/decennial/2020/data/apportionment/apportionment-2020-table01.xlsx
    // As of 27 Jan 2025
    static M us_2020_reps()
    {
        return {{"Alabama", 7},
                {"Alaska", 1},
                {"Arizona", 9},
                {"Arkansas", 4},
                {"California", 52},
                {"Colorado", 8},
                {"Connecticut", 5},
                {"Delaware", 1},
                {"Florida", 28},
                {"Georgia", 14},
                {"Hawaii", 2},
                {"Idaho", 2},
                {"Illinois", 17},
                {"Indiana", 9},
                {"Iowa", 4},
                {"Kansas", 4},
                {"Kentucky", 6},
                {"Louisiana", 6},
                {"Maine", 2},
                {"Maryland", 8},
                {"Massachusetts", 9},
                {"Michigan", 13},
                {"Minnesota", 8},
                {"Mississippi", 4},
                {"Missouri", 8},
                {"Montana", 2},
                {"Nebraska", 3},
                {"Nevada", 4},
                {"New Hampshire", 2},
                {"New Jersey", 12},
                {"New Mexico", 3},
                {"New York", 26},
                {"North Carolina", 14},
                {"North Dakota", 1},
                {"Ohio", 15},
                {"Oklahoma", 5},
                {"Oregon", 6},
                {"Pennsylvania", 17},
                {"Rhode Island", 2},
                {"South Carolina", 7},
                {"South Dakota", 1},
                {"Tennessee", 9},
                {"Texas", 38},
                {"Utah", 4},
                {"Vermont", 1},
                {"Virginia", 11},
                {"Washington", 10},
                {"West Virginia", 2},
                {"Wisconsin", 8},
                {"Wyoming", 1}};
    };

private:
    std::size_t get_num_states() const
    {
        return population_for_each_state.size();
    }

    void add_next_rep(M &state_nums_reps, const N &name_of_state_with_next_rep) const
    {
        state_nums_reps.at(name_of_state_with_next_rep)++;
    }

    void fill_with_ones(M &state_nums_reps) const
    {
        state_nums_reps.clear();
        for (auto &&pair : population_for_each_state)
        {
            state_nums_reps.insert(std::make_pair(pair.first, 1));
        }
    }

    // The return value for this function must be positive.
    static long double multiplier(R current_state_num_reps)
    {
        long double m = current_state_num_reps;
        m *= (m + 1.0L);
        m = sqrtl(m);
        m = 1.0L / m;
        assert(m > 0.0L);
        return m;
    }

    // The return value for this function must be positive.
    long double priority_value(R current_state_num_reps, const N &state_name) const
    {
        long double pv = multiplier(current_state_num_reps) * population_for_each_state.at(state_name);
        assert(pv > 0.0L);
        return pv;
    }

    N get_name_of_state_with_next_rep(const M &state_nums_reps) const
    {
        assert(!population_for_each_state.empty());

        N name_of_state_with_next_rep;

        long double max_priority_value_so_far = -1.0L;

        bool first_state = true;

        for (auto &&pair : population_for_each_state)
        {
            const R current_state_num_reps = state_nums_reps.at(pair.first);

            const long double priority_value_for_this_state = priority_value(current_state_num_reps, pair.first);

            assert(priority_value_for_this_state > 0.0L);

            if (first_state || max_priority_value_so_far < priority_value_for_this_state)
            {
                max_priority_value_so_far = priority_value_for_this_state;
                name_of_state_with_next_rep = pair.first;
            }

            first_state = false;
        }

        assert(max_priority_value_so_far > 0.0L);

        return name_of_state_with_next_rep;
    }

    // https://en.cppreference.com/w/cpp/language/operators
    // "Stream extraction and insertion" section
    static std::ostream &operator_os(std::ostream &os, const M &m)
    {
        os << '{';

        bool first_pair = true;
        for (auto &&pair : m)
        {
            if (!first_pair)
            {
                os << ',';
            }
            os << '{' << '\"' << pair.first << '\"' << ',' << pair.second << '}';
            first_pair = false;
        }

        os << '}';
        return os;
    }

private:
    // The return value for this function must be positive.
    long double persons_per_rep(const M &state_nums_reps, const N &state_name) const
    {
        const P population = population_for_each_state.at(state_name);
        const R num_reps = state_nums_reps.at(state_name);
        const long double ratio = static_cast<long double>(population) / static_cast<long double>(num_reps);

        assert(population > 0);
        assert(num_reps > 0);
        assert(ratio > 0.0L);

        return ratio;
    }

    // The return value for this function must be positive.
    long double max_persons_per_rep(const M &state_nums_reps) const
    {
        assert(!population_for_each_state.empty());

        bool first_state = true;

        long double max_persons_per_rep_so_far = -1.0L;

        for (auto &&pair : population_for_each_state)
        {
            const long double current_state_persons_per_rep = persons_per_rep(state_nums_reps, pair.first);

            if (first_state || max_persons_per_rep_so_far < current_state_persons_per_rep)
            {
                max_persons_per_rep_so_far = current_state_persons_per_rep;
            }

            first_state = false;
        }

        assert(max_persons_per_rep_so_far > 0.0L);
        return max_persons_per_rep_so_far;
    }

    // The return value for this function must be positive.
    long double min_persons_per_rep(const M &state_nums_reps) const
    {
        assert(!population_for_each_state.empty());

        bool first_state = true;

        // https://en.cppreference.com/w/cpp/types/numeric_limits/infinity
        long double min_persons_per_rep_so_far = std::numeric_limits<long double>::infinity();

        for (auto &&pair : population_for_each_state)
        {
            const long double current_state_persons_per_rep = persons_per_rep(state_nums_reps, pair.first);

            if (first_state || min_persons_per_rep_so_far > current_state_persons_per_rep)
            {
                min_persons_per_rep_so_far = current_state_persons_per_rep;
            }

            first_state = false;
        }

        assert(min_persons_per_rep_so_far > 0.0L);
        return min_persons_per_rep_so_far;
    }

    // The return value for this function must be positive.
    P population_of_all_states() const
    {
        P population_so_far = 0;
        for (auto &&pair : population_for_each_state)
        {
            population_so_far += pair.second;
        }
        assert(population_so_far > 0);
        return population_so_far;
    }

    // The return value for this function must be positive.
    static R num_reps_of_all_states(const M &state_nums_reps)
    {
        R num_reps_so_far = 0;
        for (auto &&pair : state_nums_reps)
        {
            num_reps_so_far += pair.second;
        }
        assert(num_reps_so_far > 0);
        return num_reps_so_far;
    }

    // The return value for this function must be positive.
    long double avg_persons_per_rep(R total_num_reps) const
    {
        assert(total_num_reps > 0);

        const P population = population_of_all_states();
        const long double ratio = static_cast<long double>(population) / static_cast<long double>(total_num_reps);

        assert(population > 0);
        assert(ratio > 0.0L);

        return ratio;
    }

public:
    M num_reps_for_each_state(R total_num_reps)
    {
        if (num_reps_for_each_size_and_state.find(total_num_reps) == num_reps_for_each_size_and_state.end())
        {
            M state_nums_reps;

            const std::size_t num_states = get_num_states();

            assert(num_states > 0);

            if (total_num_reps < num_states)
            {
                std::string s(__PRETTY_FUNCTION__);
                s += " -- total_num_reps must be at least ";
                s += std::to_string(num_states);
                throw std::invalid_argument(s);
            }
            else if (total_num_reps == num_states)
            {
                fill_with_ones(state_nums_reps);
            }
            else
            {
                state_nums_reps = num_reps_for_each_state(total_num_reps - 1);

                const N name_of_state_with_next_rep = get_name_of_state_with_next_rep(state_nums_reps);

                add_next_rep(state_nums_reps, name_of_state_with_next_rep);
            }

            if (!num_reps_for_each_size_and_state.insert(std::make_pair(total_num_reps, state_nums_reps)).second)
            {
                assert(num_reps_for_each_size_and_state.at(total_num_reps) == state_nums_reps);
            }
        }
        return num_reps_for_each_size_and_state.at(total_num_reps);
    }

    static void test_this_class_part_1()
    {
        apportionment a;
        M m1(a.num_reps_for_each_state(435));
        M m2(us_2020_reps());
        operator_os(std::cout << '\n', m1) << '\n';
        operator_os(std::cout << '\n', m2) << '\n';
        assert(m1 == m2);
    }

    static void test_this_class_part_2()
    {
        std::map<N, P> the_population_for_each_state(us_2020_populations());

        // On 27 Jan 2025, the "WHAT WILL THIS COST IDAHO?" section of
        // https://greateridaho.org/faq said,
        //
        // "The cost will be negotiated between the two states. Remember, Oregon
        // should want to cut their losses because they subsidize eastern Oregon.
        // Idaho would be doing western Oregon a favor to take these counties on. The
        // state government's assets are owned on behalf of all Oregonians. Since
        // eastern Oregonians are part owners of state assets and liabilities, a
        // reasonable starting point for negotiations would recognize the propriety
        // of eastern Oregon (9% of Oregon's population) taking 9% of the state
        // government's assets and liabilities with them, in return for giving up
        // claims to the rest of Oregon's state government assets. For Idaho, any
        // initial cost would be paid off over time. Under Idaho's lower taxes and
        // regulation, the economy of these eastern counties will surge, providing a
        // big benefit to Idaho's state budget. The benefit to Idaho's budget of
        // adding southern and eastern Oregon would be $170 million annually,
        // according to a Claremont Institute study."

        P unsigned_population_change = static_cast<P>(0.09 * the_population_for_each_state.at("Oregon"));
        the_population_for_each_state.at("Oregon") -= unsigned_population_change;
        the_population_for_each_state.at("Idaho") += unsigned_population_change;

        apportionment a(the_population_for_each_state);
        M m(a.num_reps_for_each_state(435));
        operator_os(std::cout << '\n', m) << '\n';
    }

    static void test_this_class_part_3()
    {
        std::map<N, P> the_population_for_each_state(us_2020_populations());

        the_population_for_each_state.insert(std::make_pair(
            // https://www2.census.gov/programs-surveys/decennial/2020/data/apportionment/apportionment-2020-tableA.xlsx
            // As of 27 Jan 2025
            "~District of Columbia", (689545 + 1988)));
        the_population_for_each_state.insert(std::make_pair(
            // https://www2.census.gov/programs-surveys/decennial/2020/data/apportionment/apportionment-2020-tableA.xlsx
            // As of 27 Jan 2025
            "~Puerto Rico", (3285874 + 0)));

        apportionment a(the_population_for_each_state);
        M m(a.num_reps_for_each_state(440));
        operator_os(std::cout << '\n', m) << '\n';
    }

    static void test_this_class_part_4()
    {
        std::cout << "\n\n\n";

        apportionment a;

        const R min_total_num_reps = static_cast<R>(435);
        const R max_total_num_reps = static_cast<R>(2 * min_total_num_reps + 1);

        std::cout << "total_num_reps\tmax_over_min\tmax_over_avg\tavg_over_min\n";

        for (R total_num_reps = min_total_num_reps; total_num_reps <= max_total_num_reps; total_num_reps++)
        {
            M m(a.num_reps_for_each_state(total_num_reps));

            const long double max = a.max_persons_per_rep(m);
            const long double min = a.min_persons_per_rep(m);
            const long double avg = a.avg_persons_per_rep(total_num_reps);

            const long double max_over_min = max / min;
            const long double max_over_avg = max / avg;
            const long double avg_over_min = avg / min;

            std::cout << total_num_reps << '\t' << max_over_min << '\t' << max_over_avg << '\t' << avg_over_min << '\n';
        }
    }
};

#endif // SANDBOX_CPP_APPORTIONMENT
