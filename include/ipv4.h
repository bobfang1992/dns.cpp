#pragma once
#include <netinet/in.h>
#include <string>

namespace dnscpp {

class IPV4 {
public:
  constexpr IPV4() : a(0), b(0), c(0), d(0) {}
  constexpr IPV4(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
      : a(a), b(b), c(c), d(d) {}

  friend std::string to_string(const IPV4 &ip);

  [[nodiscard]] constexpr uint32_t to_int() const {
    return (a << 24) | (b << 16) | (c << 8) | d;
  }

  bool operator==(const IPV4 &other) const {
    return a == other.a && b == other.b && c == other.c && d == other.d;
  }

private:
  const uint8_t a;
  const uint8_t b;
  const uint8_t c;
  const uint8_t d;
};

struct NetAddress {
  IPV4 ip;
  int32_t port{};

  bool operator==(const NetAddress &other) const {
    return ip == other.ip && port == other.port;
  }
};

inline std::string to_string(const IPV4 &ip) {
  return std::to_string(ip.a) + "." + std::to_string(ip.b) + "." +
         std::to_string(ip.c) + "." + std::to_string(ip.d);
}

inline NetAddress from_sockaddr(const sockaddr_in &addr) {
  uint32_t ip = ntohl(addr.sin_addr.s_addr);
  return {IPV4(ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF),
          ntohs(addr.sin_port)};
}

} // namespace dnscpp

namespace std {

template <> struct hash<dnscpp::IPV4> {
  std::size_t operator()(const dnscpp::IPV4 &ip) const {
    return std::hash<uint32_t>{}(ip.to_int());
  }
};

template <> struct hash<dnscpp::NetAddress> {
  std::size_t operator()(const dnscpp::NetAddress &addr) const {
    return std::hash<dnscpp::IPV4>{}(addr.ip) ^ std::hash<int32_t>{}(addr.port);
  }
};

} // namespace std
