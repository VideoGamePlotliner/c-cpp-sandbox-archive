#ifndef SANDBOX_CPP_SOCKETS
#define SANDBOX_CPP_SOCKETS

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

// https://www.man7.org/linux/man-pages/man0/netdb.h.0p.html
// https://www.man7.org/linux/man-pages/man0/sys_un.h.0p.html
// https://www.man7.org/linux/man-pages/man0/stddef.h.0p.html
// https://www.man7.org/linux/man-pages/man0/stdio.h.0p.html
// https://www.man7.org/linux/man-pages/man0/unistd.h.0p.html
// https://www.man7.org/linux/man-pages/man3/freeaddrinfo.3p.html
// https://www.man7.org/linux/man-pages/man3/getnameinfo.3p.html

// https://www.man7.org/linux/man-pages/dir_all_alphabetic.html

namespace cpp_sockets
{

    // https://www.man7.org/linux/man-pages/man2/socket.2.html
    // If `domain` is `AF_UNIX`, then refer to https://www.man7.org/linux/man-pages/man7/unix.7.html
    // If `domain` is `AF_INET`, then refer to https://www.man7.org/linux/man-pages/man7/ip.7.html
    // If `domain` is `AF_INET6`, then refer to https://www.man7.org/linux/man-pages/man7/ipv6.7.html
    // Determine which value of `domain` you want before determining which value of `type` you want or which value of `protocol` you want.
    // If error, return -1. Otherwise, return new FD.
    int f_socket(int domain, int type, int protocol) { return socket(domain, type, protocol); }

    // https://www.man7.org/linux/man-pages/man0/sys_uio.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/readv.2.html
    // https://www.man7.org/linux/man-pages/man3/readv.3p.html
    // ASSERT_IS_FUNCTION(readv);

    // https://www.man7.org/linux/man-pages/man0/sys_uio.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/writev.2.html
    // https://www.man7.org/linux/man-pages/man3/writev.3p.html
    // ASSERT_IS_FUNCTION(writev);

    // https://www.man7.org/linux/man-pages/man0/netinet_tcp.h.0p.html
    // Can be "a socket option at the IPPROTO_TCP level"
    DECLARE_CONSTANT_ALIAS(TCP_NODELAY);

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
    // uint8_t
    // uint32_t

    // https://www.man7.org/linux/man-pages/man0/netinet_in.h.0p.html
    ASSERT_IS_CLASS(in_addr);
    ASSERT_IS_CLASS(sockaddr_in);
    ASSERT_IS_CLASS(in6_addr);
    ASSERT_IS_CLASS(sockaddr_in6);
    const in6_addr c_in6addr_any = IN6ADDR_ANY_INIT;
    const in6_addr c_in6addr_loopback = IN6ADDR_LOOPBACK_INIT;
    ASSERT_IS_CLASS(ipv6_mreq);

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
        const struct in6_addr c_in6_addr = IN6ADDR_LOOPBACK_INIT;

        IN6_IS_ADDR_UNSPECIFIED(&c_in6_addr);

        IN6_IS_ADDR_LOOPBACK(&c_in6_addr);

        IN6_IS_ADDR_MULTICAST(&c_in6_addr);

        IN6_IS_ADDR_LINKLOCAL(&c_in6_addr);

        IN6_IS_ADDR_SITELOCAL(&c_in6_addr);

        IN6_IS_ADDR_V4MAPPED(&c_in6_addr);

        IN6_IS_ADDR_V4COMPAT(&c_in6_addr);

        IN6_IS_ADDR_MC_NODELOCAL(&c_in6_addr);

        IN6_IS_ADDR_MC_LINKLOCAL(&c_in6_addr);

        IN6_IS_ADDR_MC_SITELOCAL(&c_in6_addr);

        IN6_IS_ADDR_MC_ORGLOCAL(&c_in6_addr);

        IN6_IS_ADDR_MC_GLOBAL(&c_in6_addr);
    }

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

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // Can be the `option_name` value for `getsockopt()` and `setsockopt()`
    DECLARE_CONSTANT_ALIAS(SO_ACCEPTCONN);
    DECLARE_CONSTANT_ALIAS(SO_BROADCAST);
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

    void s_msghdr_cmsghdr()
    {
        // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
        // https://en.cppreference.com/w/cpp/language/zero_initialization
        // https://en.cppreference.com/w/cpp/language/default_initialization
        // https://www.man7.org/linux/man-pages/man3/cmsg.3.html
        msghdr mhdr = {0};
        cmsghdr cmsg = {0};

        // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
        // https://www.man7.org/linux/man-pages/man3/CMSG_DATA.3.html
        // https://www.man7.org/linux/man-pages/man3/cmsg_data.3.html
        auto cmsg_data = CMSG_DATA(&cmsg);

        // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
        // https://www.man7.org/linux/man-pages/man3/CMSG_NXTHDR.3.html
        // https://www.man7.org/linux/man-pages/man3/cmsg_nxthdr.3.html
        auto cmsg_nxthdr = CMSG_NXTHDR(&mhdr, &cmsg);

        // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
        // https://www.man7.org/linux/man-pages/man3/CMSG_FIRSTHDR.3.html
        // https://www.man7.org/linux/man-pages/man3/cmsg_firsthdr.3.html
        auto cmsg_firsthdr = CMSG_FIRSTHDR(&mhdr);
    }

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
    // https://www.man7.org/linux/man-pages/man7/socket.7.html
    ASSERT_IS_FUNCTION(socket);

    // https://www.man7.org/linux/man-pages/man0/sys_socket.h.0p.html
    // https://www.man7.org/linux/man-pages/man2/socketpair.2.html
    // https://www.man7.org/linux/man-pages/man3/socketpair.3p.html
    ASSERT_IS_FUNCTION(socketpair);

} // namespace cpp_sockets

#endif // SANDBOX_CPP_SOCKETS
