#ifndef SANDBOX_CPP_SOCKETS
#define SANDBOX_CPP_SOCKETS

#include <cerrno>
#include "cpp_assert.hpp"
#include "cpp_alias.hpp"

// https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
#include <sys/socket.h>

// https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
#include <netinet/in.h>

// https://www.man7.org/linux/man-pages/man0/inttypes.h.0p.html
// #include <inttypes.h>

// https://www.man7.org/linux/man-pages/man0/stdint.h.0p.html
#include <stdint.h>

// https://www.man7.org/linux/man-pages/man0/arpa_inet.h.0p.html
#include <arpa/inet.h>

// https://www.man7.org/linux/man-pages/man0/netinet_tcp.h.0p.html
#include <netinet/tcp.h>

// https://www.man7.org/linux/man-pages/man0/sys_uio.h.0p.html
#include <sys/uio.h>

// https://www.man7.org/linux/man-pages/man0/unistd.h.0p.html
#include <unistd.h>

// https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
#include <netdb.h>

// https://www.man7.org/linux/man-pages/man0/sys_un.h.0p.html
#include <sys/un.h>

// https://www.man7.org/linux/man-pages/man0/fcntl.h.0p.html
// https://www.man7.org/linux/man-pages/man2/fcntl.2.html
// https://www.man7.org/linux/man-pages/man3/fcntl.3p.html
// https://www.man7.org/linux/man-pages/man2/fcntl64.2.html
// #include <fcntl.h>

// Special thanks to...
// https://github.com/search?q=repo%3Amkerrisk%2Fman-pages+%22AF_INET%22&type=code
// https://github.com/search?q=repo%3Amkerrisk%2Fman-pages+%22AF_INET%22&type=code&p=2
// ...for helping me find the "EXAMPLES" sections of...
// https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
// https://www.man7.org/linux/man-pages/man3/inet_ntop.3.html
// https://www.man7.org/linux/man-pages/man3/inet_pton.3.html
// https://www.man7.org/linux/man-pages/man2/select_tut.2.html
// https://www.man7.org/linux/man-pages/man2/sendmmsg.2.html

// Curious about byte orders? Then refer to:
// https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
// https://www.man7.org/linux/man-pages/man7/ip.7.html
// https://www.man7.org/linux/man-pages/man3/sockaddr_in.3type.html
// https://www.man7.org/linux/man-pages/man3/htons.3.html
// https://www.man7.org/linux/man-pages/man3/htonl.3.html
// https://www.man7.org/linux/man-pages/man3/ntohs.3.html
// https://www.man7.org/linux/man-pages/man3/ntohl.3.html

// TODO IOCTL

// TODO De-couple

// TODO What if strerror or strerrorname_np or strerrordesc_np or strsignal or sigdescr_np or sigabbrev_np returns NULL?
// TODO While I'm at it, centralize duplicate code related to strerror and strsignal.
// TODO Beware thread-unsafe strsignal
// TODO Handle any instance of 's += NULL;'
// TODO Handle any instance of 'std::string s(NULL);'

// TODO "EXAMPLES" section of https://www.man7.org/linux/man-pages/man2/select_tut.2.html

// https://www.man7.org/linux/man-pages/dir_all_alphabetic.html

// https://www.man7.org/linux/man-pages/man7/ip.7.html
// https://www.man7.org/linux/man-pages/man3/in_port_t.3type.html
// In host byte order
#define LOWER_BOUND_FOR_LOCAL_IP_PORT ((in_port_t)4097)

// https://en.cppreference.com/w/cpp/preprocessor/replace
// "Socket options" section of https://www.man7.org/linux/man-pages/man7/ip.7.html
// https://www.man7.org/linux/man-pages/man2/getsockopt.2.html
// WARNING: https://www.man7.org/linux/man-pages/man3/getsockopt.3p.html says that optval might become "silently truncated."
#define GSO_IP(optname) \
    int f_getsockopt_IP_and_##optname(int sockfd, int &optval, socklen_t &optlen) { return getsockopt(sockfd, IPPROTO_IP, optname, &optval, &optlen); }

// https://en.cppreference.com/w/cpp/preprocessor/replace
// "Socket options" section of https://www.man7.org/linux/man-pages/man7/tcp.7.html
// https://www.man7.org/linux/man-pages/man2/getsockopt.2.html
// WARNING: https://www.man7.org/linux/man-pages/man3/getsockopt.3p.html says that optval might become "silently truncated."
#define GSO_TCP(optname) \
    int f_getsockopt_TCP_and_##optname(int sockfd, int &optval, socklen_t &optlen) { return getsockopt(sockfd, IPPROTO_TCP, optname, &optval, &optlen); }

// https://en.cppreference.com/w/cpp/preprocessor/replace
// https://www.man7.org/linux/man-pages/man2/getsockopt.2.html
// WARNING: https://www.man7.org/linux/man-pages/man3/getsockopt.3p.html says that optval might become "silently truncated."
#define GSO_SOCKET(optname) \
    int f_getsockopt_SOCKET_and_##optname(int sockfd, int &optval, socklen_t &optlen) { return getsockopt(sockfd, SOL_SOCKET, optname, &optval, &optlen); }

// The "APPLICATION USAGE" section of https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html says,
// "To forestall portability problems, it is recommended that applications not use values larger than 231 -1 for the socklen_t type."
// I think they mean, "2 to the power of 31, minus 1", because
// https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html and
// https://www.man7.org/linux/man-pages/man3/socklen_t.3type.html imply
// that sizeof(socklen_t) >= 4.
#define MAX_SOCKLEN ((int32_t)0x7FFFFFFF)

namespace cpp_sockets
{
    static_assert(MAX_SOCKLEN > 0);

    // https://www.man7.org/linux/man-pages/man2/socket.2.html
    // If `domain` is `AF_UNIX`, then refer to https://www.man7.org/linux/man-pages/man7/unix.7.html
    // If `domain` is `AF_INET`, then refer to https://www.man7.org/linux/man-pages/man7/ip.7.html
    // If `domain` is `AF_INET6`, then refer to https://www.man7.org/linux/man-pages/man7/ipv6.7.html
    // Determine which value of `domain` you want before determining which value of `type` you want or which value of `protocol` you want.
    // If error, return -1. Otherwise, return new FD.
    int f_socket(int domain, int type, int protocol) { return socket(domain, type, protocol); }

    // https://www.man7.org/linux/man-pages/man0/unistd.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/close.2.html
    // https://www.man7.org/linux/man-pages/man3/close.3p.html
    // If error, return -1. Otherwise, return 0.
    int f_close(int fd) { return close(fd); }

    // https://www.man7.org/linux/man-pages/man7/ip.7.html
    // If error, return -1. Otherwise, return new FD.
    int f_socket_ip_stream_tcp() { return f_socket(AF_INET, SOCK_STREAM, 0); }

    // https://www.man7.org/linux/man-pages/man7/ip.7.html
    // If error, return -1. Otherwise, return new FD.
    int f_socket_ip_datagram_udp() { return f_socket(AF_INET, SOCK_DGRAM, 0); }

    // https://www.man7.org/linux/man-pages/man7/ip.7.html
    // If error, return -1. Otherwise, return new FD.
    int f_socket_ip_stream_sctp() { return f_socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP); }

    // https://www.man7.org/linux/man-pages/man7/ip.7.html
    // If error, return -1. Otherwise, return new FD.
    int f_socket_ip_datagram_udplite() { return f_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDPLITE); }

    // https://www.man7.org/linux/man-pages/man7/ip.7.html
    // If error, return -1. Otherwise, return new FD.
    int f_socket_ip_raw(int protocol) { return f_socket(AF_INET, SOCK_RAW, protocol); }

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // https://www.man7.org/linux/man-pages/man7/ip.7.html
    // https://www.man7.org/linux/man-pages/man3/sockaddr_in.3type.html
    // https://www.man7.org/linux/man-pages/man3/htons.3.html
    // https://www.man7.org/linux/man-pages/man3/htonl.3.html
    // https://www.man7.org/linux/man-pages/man3/ntohs.3.html
    // https://www.man7.org/linux/man-pages/man3/ntohl.3.html
    // https://www.man7.org/linux/man-pages/man2/bind.2.html
    // If error, return -1. Otherwise, return 0.
    int f_bind_ip(int sockfd, in_port_t port_in_host_byte_order, in_addr_t address_in_host_byte_order)
    {
        if (port_in_host_byte_order < LOWER_BOUND_FOR_LOCAL_IP_PORT)
        {
            errno = EACCES;
            return -1;
        }
        sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port_in_host_byte_order);
        addr.sin_addr.s_addr = htonl(address_in_host_byte_order);
        return bind(sockfd, (const sockaddr *)&addr, sizeof(addr));
    }

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // https://www.man7.org/linux/man-pages/man7/ip.7.html
    // https://www.man7.org/linux/man-pages/man3/sockaddr_in.3type.html
    // https://www.man7.org/linux/man-pages/man3/htons.3.html
    // https://www.man7.org/linux/man-pages/man3/htonl.3.html
    // https://www.man7.org/linux/man-pages/man3/ntohs.3.html
    // https://www.man7.org/linux/man-pages/man3/ntohl.3.html
    // https://www.man7.org/linux/man-pages/man2/connect.2.html
    // If error, return -1. Otherwise, return 0.
    int f_connect_ip(int sockfd, in_port_t port_in_host_byte_order, in_addr_t address_in_host_byte_order)
    {
        if (port_in_host_byte_order < LOWER_BOUND_FOR_LOCAL_IP_PORT)
        {
            errno = EACCES;
            return -1;
        }
        sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port_in_host_byte_order);
        addr.sin_addr.s_addr = htonl(address_in_host_byte_order);
        return connect(sockfd, (const sockaddr *)&addr, sizeof(addr));
    }

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // https://www.man7.org/linux/man-pages/man7/ip.7.html
    // https://www.man7.org/linux/man-pages/man3/sockaddr_in.3type.html
    // https://www.man7.org/linux/man-pages/man3/htons.3.html
    // https://www.man7.org/linux/man-pages/man3/htonl.3.html
    // https://www.man7.org/linux/man-pages/man3/ntohs.3.html
    // https://www.man7.org/linux/man-pages/man3/ntohl.3.html
    // https://www.man7.org/linux/man-pages/man2/accept.2.html
    // "EXAMPLES" section of https://www.man7.org/linux/man-pages/man2/bind.2.html
    // If error, return -1. Otherwise, return new FD.
    int f_accept_ip(int sockfd, in_port_t &peer_port_in_host_byte_order, in_addr_t &peer_address_in_host_byte_order, socklen_t &peer_address_length)
    {
        sockaddr_in addr = {0};
        socklen_t addrlen = sizeof(addr);

        int return_value = accept(sockfd, (sockaddr *)&addr, &addrlen);

        if (return_value != -1)
        {
            if (addr.sin_family != AF_INET)
            {
                // Shrug
            }
            if (ntohs(addr.sin_port) < LOWER_BOUND_FOR_LOCAL_IP_PORT)
            {
                // Shrug
            }
            peer_port_in_host_byte_order = ntohs(addr.sin_port);
            peer_address_in_host_byte_order = ntohl(addr.sin_addr.s_addr);
            peer_address_length = addrlen;
        }

        return return_value;
    }

    // https://www.man7.org/linux/man-pages/man2/accept.2.html
    // If error, return -1. Otherwise, return new FD.
    int f_accept_NULL(int sockfd) { return accept(sockfd, (sockaddr *)NULL, (socklen_t *)NULL); }

    // https://datatracker.ietf.org/doc/html/rfc791#section-3.1
    // "Socket options" section of https://www.man7.org/linux/man-pages/man7/ip.7.html
    GSO_IP(IP_TTL)
    GSO_IP(IP_TOS)

    // https://www.man7.org/linux/man-pages/man0/netinet_tcp.h.0p.html
    // "Socket options" section of https://www.man7.org/linux/man-pages/man7/tcp.7.html
    GSO_TCP(TCP_NODELAY)

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // "Socket options" section of https://www.man7.org/linux/man-pages/man7/socket.7.html
    GSO_SOCKET(SO_ACCEPTCONN)
    GSO_SOCKET(SO_BROADCAST)
    GSO_SOCKET(SO_DEBUG)
    GSO_SOCKET(SO_DONTROUTE)
    GSO_SOCKET(SO_KEEPALIVE)
    GSO_SOCKET(SO_OOBINLINE)
    GSO_SOCKET(SO_RCVBUF)
    GSO_SOCKET(SO_REUSEADDR)
    GSO_SOCKET(SO_SNDBUF)
    GSO_SOCKET(SO_TYPE)

    // https://www.man7.org/linux/man-pages/man2/connect.2.html
    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // "Socket options" section of https://www.man7.org/linux/man-pages/man7/socket.7.html
    GSO_SOCKET(SO_ERROR)

    // "Socket options" section of https://www.man7.org/linux/man-pages/man7/socket.7.html
    GSO_SOCKET(SO_DOMAIN)
    GSO_SOCKET(SO_PROTOCOL)

    // "Socket options" section of https://www.man7.org/linux/man-pages/man7/tcp.7.html
    GSO_TCP(TCP_MAXSEG)
    GSO_TCP(TCP_SYNCNT)

    // "Socket options" section of https://www.man7.org/linux/man-pages/man7/tcp.7.html
    int f_getsockopt_TCP_and_TCP_USER_TIMEOUT(int sockfd, unsigned int &optval, socklen_t &optlen) { return getsockopt(sockfd, IPPROTO_TCP, TCP_USER_TIMEOUT, &optval, &optlen); }

    // "DESCRIPTION" section of https://www.man7.org/linux/man-pages/man7/tcp.7.html
    // "A newly created TCP socket has no remote or local address and is
    // not fully specified. To create an outgoing TCP connection use
    // connect(2) to establish a connection to another TCP socket. To
    // receive new incoming connections, first bind(2) the socket to a
    // local address and port and then call listen(2) to put the socket
    // into the listening state. After that a new socket for each
    // incoming connection can be accepted using accept(2). A socket
    // which has had accept(2) or connect(2) successfully called on it is
    // fully specified and may transmit data. Data cannot be transmitted
    // on listening or not yet connected sockets."

    // "DESCRIPTION" section of https://www.man7.org/linux/man-pages/man7/tcp.7.html
    // "TCP supports urgent data. Urgent data is used to signal the
    // receiver that some important message is part of the data stream
    // and that it should be processed as soon as possible. To send
    // urgent data specify the MSG_OOB option to send(2). When urgent
    // data is received, the kernel sends a SIGURG signal to the process
    // or process group that has been set as the socket "owner" using the
    // SIOCSPGRP or FIOSETOWN ioctls (or the POSIX.1-specified fcntl(2)
    // F_SETOWN operation). When the SO_OOBINLINE socket option is
    // enabled, urgent data is put into the normal data stream (a program
    // can test for its location using the SIOCATMARK ioctl described
    // below), otherwise it can be received only when the MSG_OOB flag is
    // set for recv(2) or recvmsg(2)."

    // "DESCRIPTION" section of https://www.man7.org/linux/man-pages/man7/tcp.7.html
    // "When out-of-band data is present, select(2) indicates the file
    // descriptor as having an exceptional condition and poll(2)
    // indicates a POLLPRI event."

    // "Address formats" section of https://www.man7.org/linux/man-pages/man7/tcp.7.html
    // TCP is built on top of IP (see ip(7)). The address formats
    // defined by ip(7) apply to TCP. TCP supports point-to-point
    // communication only; broadcasting and multicasting are not
    // supported.

    // "DESCRIPTION" section of https://www.man7.org/linux/man-pages/man7/udp.7.html
    // "When a UDP socket is created, its local and remote addresses are
    // unspecified. Datagrams can be sent immediately using sendto(2) or
    // sendmsg(2) with a valid destination address as an argument. When
    // connect(2) is called on the socket, the default destination
    // address is set and datagrams can now be sent using send(2) or
    // write(2) without specifying a destination address. It is still
    // possible to send to other destinations by passing an address to
    // sendto(2) or sendmsg(2). In order to receive packets, the socket
    // can be bound to a local address first by using bind(2).
    // Otherwise, the socket layer will automatically assign a free local
    // port out of the range defined by
    // /proc/sys/net/ipv4/ip_local_port_range and bind the socket to
    // INADDR_ANY."

    // TODO More socket options:
    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // https://www.man7.org/linux/man-pages/man7/ipv6.7.html

    // https://www.man7.org/linux/man-pages/man0/sys_uio.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/readv.2.html
    // https://www.man7.org/linux/man-pages/man3/readv.3p.html
    // ASSERT_IS_FUNCTION(readv);

    // https://www.man7.org/linux/man-pages/man0/sys_uio.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/writev.2.html
    // https://www.man7.org/linux/man-pages/man3/writev.3p.html
    // ASSERT_IS_FUNCTION(writev);

    // https://www.man7.org/linux/man-pages/man0/arpa_inet.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/htonl.3.html
    // https://www.man7.org/linux/man-pages/man3/htonl.3p.html
    ASSERT_IS_FUNCTION(htonl);

    // https://www.man7.org/linux/man-pages/man0/arpa_inet.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/htons.3.html
    // https://www.man7.org/linux/man-pages/man3/htons.3p.html
    ASSERT_IS_FUNCTION(htons);

    // https://www.man7.org/linux/man-pages/man0/arpa_inet.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/ntohl.3.html
    // https://www.man7.org/linux/man-pages/man3/ntohl.3p.html
    ASSERT_IS_FUNCTION(ntohl);

    // https://www.man7.org/linux/man-pages/man0/arpa_inet.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/ntohs.3.html
    // https://www.man7.org/linux/man-pages/man3/ntohs.3p.html
    ASSERT_IS_FUNCTION(ntohs);

    // https://www.man7.org/linux/man-pages/man0/arpa_inet.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/inet_addr.3.html
    // https://www.man7.org/linux/man-pages/man3/inet_addr.3p.html
    ASSERT_IS_FUNCTION(inet_addr);

    // https://www.man7.org/linux/man-pages/man0/arpa_inet.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/inet_ntoa.3.html
    // https://www.man7.org/linux/man-pages/man3/inet_ntoa.3p.html
    ASSERT_IS_FUNCTION(inet_ntoa);

    // https://www.man7.org/linux/man-pages/man0/arpa_inet.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/inet_ntop.3.html
    // https://www.man7.org/linux/man-pages/man3/inet_ntop.3p.html
    ASSERT_IS_FUNCTION(inet_ntop);

    // https://www.man7.org/linux/man-pages/man0/arpa_inet.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/inet_pton.3.html
    // https://www.man7.org/linux/man-pages/man3/inet_pton.3p.html
    ASSERT_IS_FUNCTION(inet_pton);

    // https://www.man7.org/linux/man-pages/man0/stdint.h.0p.html
    ASSERT_IS_INTEGRAL(int8_t);
    ASSERT_IS_INTEGRAL(int16_t);
    ASSERT_IS_INTEGRAL(int32_t);
    ASSERT_IS_INTEGRAL(uint8_t);
    ASSERT_IS_INTEGRAL(uint16_t);
    ASSERT_IS_INTEGRAL(uint32_t);
    ASSERT_IS_SIGNED(int8_t);
    ASSERT_IS_SIGNED(int16_t);
    ASSERT_IS_SIGNED(int32_t);
    ASSERT_IS_UNSIGNED(uint8_t);
    ASSERT_IS_UNSIGNED(uint16_t);
    ASSERT_IS_UNSIGNED(uint32_t);
    ASSERT_ACTUAL_SIZEOF(int8_t, 1);
    ASSERT_ACTUAL_SIZEOF(int16_t, 2);
    ASSERT_ACTUAL_SIZEOF(int32_t, 4);
    ASSERT_ACTUAL_SIZEOF(uint8_t, 1);
    ASSERT_ACTUAL_SIZEOF(uint16_t, 2);
    ASSERT_ACTUAL_SIZEOF(uint32_t, 4);

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // https://www.man7.org/linux/man-pages/man0/stdint.h.0p.html
    static_assert(std::is_same_v<in_port_t, uint16_t>);
    static_assert(std::is_same_v<in_addr_t, uint32_t>);

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    ASSERT_IS_CLASS(in_addr);
    ASSERT_IS_CLASS(sockaddr_in);
    ASSERT_IS_CLASS(in6_addr);
    ASSERT_IS_CLASS(sockaddr_in6);
    ASSERT_IS_CLASS(ipv6_mreq);

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // void s_in6_addr_2()
    // {
    //     const in6_addr x_in6addr_any = IN6ADDR_ANY_INIT;
    //     const in6_addr x_in6addr_loopback = IN6ADDR_LOOPBACK_INIT;
    // }

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // Can be the `level` value for `getsockopt()` and `setsockopt()`
    DECLARE_CONSTANT_ALIAS(IPPROTO_IP);
    DECLARE_CONSTANT_ALIAS(IPPROTO_IPV6);
    DECLARE_CONSTANT_ALIAS(IPPROTO_ICMP);
    DECLARE_CONSTANT_ALIAS(IPPROTO_RAW);
    DECLARE_CONSTANT_ALIAS(IPPROTO_TCP);
    DECLARE_CONSTANT_ALIAS(IPPROTO_UDP);

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // Can be the local address used for a parameter of `bind()`
    DECLARE_CONSTANT_ALIAS(INADDR_ANY);

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // Can be the destination address used for a parameter of `connect()`, `sendmsg()`, and `sendto()`
    DECLARE_CONSTANT_ALIAS(INADDR_BROADCAST);

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    DECLARE_CONSTANT_ALIAS(INET_ADDRSTRLEN);
    static_assert(INET_ADDRSTRLEN == 16);
    DECLARE_CONSTANT_ALIAS(INET6_ADDRSTRLEN);
    static_assert(INET6_ADDRSTRLEN == 46);

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // Can be the `option_name` value for `getsockopt()` and `setsockopt()` if `IPPROTO_IPV6` is used
    DECLARE_CONSTANT_ALIAS(IPV6_JOIN_GROUP);
    DECLARE_CONSTANT_ALIAS(IPV6_LEAVE_GROUP);
    DECLARE_CONSTANT_ALIAS(IPV6_MULTICAST_HOPS);
    DECLARE_CONSTANT_ALIAS(IPV6_MULTICAST_IF);
    DECLARE_CONSTANT_ALIAS(IPV6_MULTICAST_LOOP);
    DECLARE_CONSTANT_ALIAS(IPV6_UNICAST_HOPS);
    DECLARE_CONSTANT_ALIAS(IPV6_V6ONLY);

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    void s_in6_addr()
    {
        const in6_addr x_in6_addr = IN6ADDR_LOOPBACK_INIT;
        IN6_IS_ADDR_UNSPECIFIED(&x_in6_addr);
        IN6_IS_ADDR_LOOPBACK(&x_in6_addr);
        IN6_IS_ADDR_MULTICAST(&x_in6_addr);
        IN6_IS_ADDR_LINKLOCAL(&x_in6_addr);
        IN6_IS_ADDR_SITELOCAL(&x_in6_addr);
        IN6_IS_ADDR_V4MAPPED(&x_in6_addr);
        IN6_IS_ADDR_V4COMPAT(&x_in6_addr);
        IN6_IS_ADDR_MC_NODELOCAL(&x_in6_addr);
        IN6_IS_ADDR_MC_LINKLOCAL(&x_in6_addr);
        IN6_IS_ADDR_MC_SITELOCAL(&x_in6_addr);
        IN6_IS_ADDR_MC_ORGLOCAL(&x_in6_addr);
        IN6_IS_ADDR_MC_GLOBAL(&x_in6_addr);
    }

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_CLASS(hostent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_CLASS(netent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_CLASS(protoent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_CLASS(servent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_CLASS(addrinfo);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    // Can be the `ai_flags` value for an `addrinfo`
    DECLARE_CONSTANT_ALIAS(AI_PASSIVE);
    DECLARE_CONSTANT_ALIAS(AI_CANONNAME);
    DECLARE_CONSTANT_ALIAS(AI_NUMERICHOST);
    DECLARE_CONSTANT_ALIAS(AI_NUMERICSERV);
    DECLARE_CONSTANT_ALIAS(AI_V4MAPPED);
    DECLARE_CONSTANT_ALIAS(AI_ALL);
    DECLARE_CONSTANT_ALIAS(AI_ADDRCONFIG);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    // Can be the `flags` argument for `getnameinfo()`
    DECLARE_CONSTANT_ALIAS(NI_NOFQDN);
    DECLARE_CONSTANT_ALIAS(NI_NUMERICHOST);
    DECLARE_CONSTANT_ALIAS(NI_NAMEREQD);
    DECLARE_CONSTANT_ALIAS(NI_NUMERICSERV);
#ifdef NI_NUMERICSCOPE
    DECLARE_CONSTANT_ALIAS(NI_NUMERICSCOPE);
#endif // NI_NUMERICSCOPE
    DECLARE_CONSTANT_ALIAS(NI_DGRAM);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    // "for use as error values for `getaddrinfo()` and `getnameinfo()`"
    DECLARE_CONSTANT_ALIAS(EAI_AGAIN);
    DECLARE_CONSTANT_ALIAS(EAI_BADFLAGS);
    DECLARE_CONSTANT_ALIAS(EAI_FAIL);
    DECLARE_CONSTANT_ALIAS(EAI_FAMILY);
    DECLARE_CONSTANT_ALIAS(EAI_MEMORY);
    DECLARE_CONSTANT_ALIAS(EAI_NONAME);
    DECLARE_CONSTANT_ALIAS(EAI_SERVICE);
    DECLARE_CONSTANT_ALIAS(EAI_SOCKTYPE);
    DECLARE_CONSTANT_ALIAS(EAI_SYSTEM);
    DECLARE_CONSTANT_ALIAS(EAI_OVERFLOW);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(endhostent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(endnetent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(endprotoent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(endservent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/freeaddrinfo.3p.html
    void f_freeaddrinfo(addrinfo *ai) { freeaddrinfo(ai); }

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(gai_strerror);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/freeaddrinfo.3p.html
    int f_getaddrinfo(const char *nodename, const char *servname, const addrinfo *hints, addrinfo **res) { return getaddrinfo(nodename, servname, hints, res); }

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(gethostent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/getnameinfo.3p.html
    int f_getnameinfo(const sockaddr *sa, socklen_t salen, char *node, socklen_t nodelen, char *service, socklen_t servicelen, int flags) { return getnameinfo(sa, salen, node, nodelen, service, servicelen, flags); }

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(getnetbyaddr);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(getnetbyname);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(getnetent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(getprotobyname);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(getprotobynumber);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(getprotoent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(getservbyname);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(getservbyport);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(getservent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(sethostent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(setnetent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(setprotoent);

    // https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
    ASSERT_IS_FUNCTION(setservent);

    // https://www.man7.org/linux/man-pages/man0/sys_un.h.0p.html
    ASSERT_IS_CLASS(sockaddr_un);

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    // const sockaddr_in6 c_sockaddr_in6 = {0};

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man0/sys_types.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/size_t.3type.html
    // https://www.man7.org/linux/man-pages/man3/ssize_t.3type.html
    ASSERT_IS_INTEGRAL(size_t);
    ASSERT_IS_UNSIGNED(size_t);
    ASSERT_IS_INTEGRAL(ssize_t);
    ASSERT_IS_SIGNED(ssize_t);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/socklen_t.3type.html
    ASSERT_IS_INTEGRAL(socklen_t);
    ASSERT_MIN_SIZEOF(socklen_t, 4);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/sa_family_t.3type.html
    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    ASSERT_IS_INTEGRAL(sa_family_t);
    ASSERT_IS_UNSIGNED(sa_family_t);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/sockaddr.3type.html
    ASSERT_IS_CLASS(sockaddr);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/sockaddr_storage.3type.html
    ASSERT_IS_CLASS(sockaddr_storage);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    ASSERT_IS_CLASS(msghdr);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/iovec.3type.html
    // https://www.man7.org/linux/man-pages/man0/sys_uio.h.0p.html
    ASSERT_IS_CLASS(iovec);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    ASSERT_IS_CLASS(cmsghdr);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    ASSERT_IS_CLASS(linger);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // Can be the `level` value for `getsockopt()` and `setsockopt()`
    DECLARE_CONSTANT_ALIAS(SOL_SOCKET);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // Can be the `cmsg_type` value for a `cmsghdr` when the latter's `cmsg_type` equals `SOL_SOCKET`
    DECLARE_CONSTANT_ALIAS(SCM_RIGHTS);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    DECLARE_CONSTANT_ALIAS(SOCK_DGRAM);
    DECLARE_CONSTANT_ALIAS(SOCK_RAW);
    DECLARE_CONSTANT_ALIAS(SOCK_SEQPACKET);
    DECLARE_CONSTANT_ALIAS(SOCK_STREAM);

    // https://www.man7.org/linux/man-pages/man3/sendmsg.3p.html
    // https://www.man7.org/linux/man-pages/man3/sendto.3p.html
    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // Can be the `option_name` value for `getsockopt()` and `setsockopt()`
    DECLARE_CONSTANT_ALIAS(SO_BROADCAST);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // Can be the `option_name` value for `getsockopt()` and `setsockopt()`
    DECLARE_CONSTANT_ALIAS(SO_ACCEPTCONN);
    DECLARE_CONSTANT_ALIAS(SO_DEBUG);
    DECLARE_CONSTANT_ALIAS(SO_DONTROUTE);
    DECLARE_CONSTANT_ALIAS(SO_ERROR);
    DECLARE_CONSTANT_ALIAS(SO_KEEPALIVE);
    DECLARE_CONSTANT_ALIAS(SO_LINGER);
    DECLARE_CONSTANT_ALIAS(SO_OOBINLINE);
    DECLARE_CONSTANT_ALIAS(SO_RCVBUF);
    DECLARE_CONSTANT_ALIAS(SO_RCVLOWAT);
    DECLARE_CONSTANT_ALIAS(SO_RCVTIMEO);
    DECLARE_CONSTANT_ALIAS(SO_REUSEADDR);
    DECLARE_CONSTANT_ALIAS(SO_SNDBUF);
    DECLARE_CONSTANT_ALIAS(SO_SNDLOWAT);
    DECLARE_CONSTANT_ALIAS(SO_SNDTIMEO);
    DECLARE_CONSTANT_ALIAS(SO_TYPE);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/listen.2.html
    // https://www.man7.org/linux/man-pages/man3/listen.3p.html
    // Can be the `backlog` value for `listen()`
    DECLARE_CONSTANT_ALIAS(SOMAXCONN);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // Can be the `msg_flags` for a `msghdr`
    // Can be the `flags` for `recv()`, `recvfrom()`, `recvmsg()`, `send()`, `sendmsg()`, and `sendto()`
    DECLARE_CONSTANT_ALIAS(MSG_CTRUNC);
    DECLARE_CONSTANT_ALIAS(MSG_DONTROUTE);
    DECLARE_CONSTANT_ALIAS(MSG_EOR);
    DECLARE_CONSTANT_ALIAS(MSG_OOB);
    DECLARE_CONSTANT_ALIAS(MSG_NOSIGNAL);
    DECLARE_CONSTANT_ALIAS(MSG_PEEK);
    DECLARE_CONSTANT_ALIAS(MSG_TRUNC);
    DECLARE_CONSTANT_ALIAS(MSG_WAITALL);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    DECLARE_CONSTANT_ALIAS(AF_INET);
    DECLARE_CONSTANT_ALIAS(AF_INET6);
    DECLARE_CONSTANT_ALIAS(AF_UNIX);
    DECLARE_CONSTANT_ALIAS(AF_UNSPEC);
    static_assert(AF_UNSPEC == 0);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    DECLARE_CONSTANT_ALIAS(SHUT_RD);
    DECLARE_CONSTANT_ALIAS(SHUT_RDWR);
    DECLARE_CONSTANT_ALIAS(SHUT_WR);

    // void s_msghdr_cmsghdr()
    // {
    //     // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    //     // https://en.cppreference.com/w/cpp/language/zero_initialization
    //     // https://en.cppreference.com/w/cpp/language/default_initialization
    //     // https://www.man7.org/linux/man-pages/man3/cmsg.3.html
    //     msghdr mhdr = {0};
    //     cmsghdr cmsg = {0};
    //     // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    //     // https://www.man7.org/linux/man-pages/man3/CMSG_DATA.3.html
    //     // https://www.man7.org/linux/man-pages/man3/cmsg_data.3.html
    //     auto cmsg_data = CMSG_DATA(&cmsg);
    //     // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    //     // https://www.man7.org/linux/man-pages/man3/CMSG_NXTHDR.3.html
    //     // https://www.man7.org/linux/man-pages/man3/cmsg_nxthdr.3.html
    //     auto cmsg_nxthdr = CMSG_NXTHDR(&mhdr, &cmsg);
    //     // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    //     // https://www.man7.org/linux/man-pages/man3/CMSG_FIRSTHDR.3.html
    //     // https://www.man7.org/linux/man-pages/man3/cmsg_firsthdr.3.html
    //     auto cmsg_firsthdr = CMSG_FIRSTHDR(&mhdr);
    // }

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/accept.2.html
    // https://www.man7.org/linux/man-pages/man3/accept.3p.html
    // https://www.man7.org/linux/man-pages/man2/accept4.2.html
    ASSERT_IS_FUNCTION(accept);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/bind.2.html
    // https://www.man7.org/linux/man-pages/man3/bind.3p.html
    ASSERT_IS_FUNCTION(bind);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/connect.2.html
    // https://www.man7.org/linux/man-pages/man3/connect.3p.html
    ASSERT_IS_FUNCTION(connect);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/getpeername.2.html
    // https://www.man7.org/linux/man-pages/man3/getpeername.3p.html
    ASSERT_IS_FUNCTION(getpeername);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/getsockname.2.html
    // https://www.man7.org/linux/man-pages/man3/getsockname.3p.html
    ASSERT_IS_FUNCTION(getsockname);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/getsockopt.2.html
    // https://www.man7.org/linux/man-pages/man3/getsockopt.3p.html
    ASSERT_IS_FUNCTION(getsockopt);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/listen.2.html
    // https://www.man7.org/linux/man-pages/man3/listen.3p.html
    ASSERT_IS_FUNCTION(listen);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/recv.2.html
    // https://www.man7.org/linux/man-pages/man3/recv.3p.html
    ASSERT_IS_FUNCTION(recv);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/recvfrom.2.html
    // https://www.man7.org/linux/man-pages/man3/recvfrom.3p.html
    ASSERT_IS_FUNCTION(recvfrom);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/recvmsg.2.html
    // https://www.man7.org/linux/man-pages/man3/recvmsg.3p.html
    ASSERT_IS_FUNCTION(recvmsg);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/send.2.html
    // https://www.man7.org/linux/man-pages/man3/send.3p.html
    ASSERT_IS_FUNCTION(send);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/sendmsg.2.html
    // https://www.man7.org/linux/man-pages/man3/sendmsg.3p.html
    ASSERT_IS_FUNCTION(sendmsg);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/sendto.2.html
    // https://www.man7.org/linux/man-pages/man3/sendto.3p.html
    ASSERT_IS_FUNCTION(sendto);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/setsockopt.2.html
    // https://www.man7.org/linux/man-pages/man3/setsockopt.3p.html
    ASSERT_IS_FUNCTION(setsockopt);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/shutdown.2.html
    // https://www.man7.org/linux/man-pages/man3/shutdown.3p.html
    ASSERT_IS_FUNCTION(shutdown);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man3/sockatmark.3.html
    // https://www.man7.org/linux/man-pages/man3/sockatmark.3p.html
    ASSERT_IS_FUNCTION(sockatmark);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/socket.2.html
    // https://www.man7.org/linux/man-pages/man3/socket.3p.html
    // "Socket options" section of https://www.man7.org/linux/man-pages/man7/socket.7.html
    ASSERT_IS_FUNCTION(socket);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/socketpair.2.html
    // https://www.man7.org/linux/man-pages/man3/socketpair.3p.html
    ASSERT_IS_FUNCTION(socketpair);

} // namespace cpp_sockets

#endif // SANDBOX_CPP_SOCKETS
