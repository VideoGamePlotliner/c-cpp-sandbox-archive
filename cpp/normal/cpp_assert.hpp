#ifndef SANDBOX_CPP_ASSERT
#define SANDBOX_CPP_ASSERT

// https://en.cppreference.com/w/cpp/header/type_traits
#include <type_traits>

// https://en.cppreference.com/w/cpp/utility/declval
#include <utility>

// https://www.man7.org/linux/man-pages/man0/stdint.h.0p.html
#include <stdint.h>

// https://en.cppreference.com/w/cpp/types/size_t
#include <cstddef>

#define ASSERT_IS_EMPTY_IF_AND_ONLY_IF_SIZE_IS_ZERO(t) assert(t.empty() == (t.size() == 0))
#define ASSERT_SIZE_IS(t, expected_size) assert(t.size() == static_cast<std::size_t>(expected_size))
#define ASSERT_CONTAINS(t, s) assert(t.find(s) != t.end())
#define ASSERT_DOES_NOT_CONTAIN(t, s) assert(t.find(s) == t.end())

#define ASSERT_IS_CLASS(type_name) static_assert(std::is_class_v<type_name>)
#define ASSERT_IS_INTEGRAL(type_name) static_assert(std::is_integral_v<type_name>)
#define ASSERT_IS_UNSIGNED(type_name) static_assert(std::is_unsigned_v<type_name>)
#define ASSERT_IS_SIGNED(type_name) static_assert(std::is_signed_v<type_name>)
#define ASSERT_MIN_SIZEOF(type_name, min_sizeof) static_assert(sizeof(type_name) >= min_sizeof)
#define ASSERT_ACTUAL_SIZEOF(type_name, actual_sizeof) static_assert(sizeof(type_name) == actual_sizeof)

// https://en.cppreference.com/w/cpp/utility/declval
// https://en.cppreference.com/w/cpp/language/decltype
// https://en.cppreference.com/w/cpp/keyword/decltype
// https://en.cppreference.com/w/cpp/types/is_function
#define ASSERT_IS_FUNCTION(func_name) static_assert(std::is_function_v<decltype(func_name)>)

#endif // SANDBOX_CPP_ASSERT
