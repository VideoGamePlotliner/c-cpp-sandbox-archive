#ifndef SANDBOX_CPP_ALIAS
#define SANDBOX_CPP_ALIAS

// https://en.cppreference.com/w/cpp/preprocessor/replace
// https://en.cppreference.com/w/cpp/language/namespace_alias
// https://en.cppreference.com/w/cpp/language/type_alias

// https://en.cppreference.com/w/cpp/language/constexpr
#define DECLARE_CONSTANT_ALIAS(name) constexpr auto const c_##name = name

#endif // SANDBOX_CPP_ALIAS
