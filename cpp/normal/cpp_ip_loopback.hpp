#ifndef SANDBOX_CPP_IP_LOOPBACK
#define SANDBOX_CPP_IP_LOOPBACK

// https://en.cppreference.com/w/cpp/language/string_literal
//
// "Special and reserved addresses" section of https://www.man7.org/linux/man-pages/man7/ip.7.html:
// "INADDR_LOOPBACK (127.0.0.1) always refers to the local host via the loopback device;"
#define IPv4_LOOPBACK_STRING_LITERAL "127.0.0.1"

// https://en.cppreference.com/w/cpp/language/string_literal
//
// The "DESCRIPTION" section of https://www.man7.org/linux/man-pages/man7/ipv6.7.html
// says that "::1" is IPv6's loopback address.
#define IPv6_LOOPBACK_STRING_LITERAL "::1"

#endif // SANDBOX_CPP_IP_LOOPBACK
