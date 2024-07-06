//
// Created by Bob Fang on 7/6/24.
//
#pragma once
#ifndef DNS_CPP_DNS_H
#define DNS_CPP_DNS_H
#include <array>
#include <cstdint>
#include <string>
#include <utility>

namespace dnscpp {
static size_t global_query_id = 0;
struct DNSQueryHeader {
  uint16_t id;
  uint16_t flags;
  uint16_t qdcount; // question count
  uint16_t ancount; // answer count
  uint16_t nscount; // name server count
  uint16_t arcount; // additional record count
};

struct DNSQuerySerialised {
  std::array<std::byte, 512> buffer;
  size_t size;
};

struct DNSQuery {
  DNSQueryHeader header{};
  std::string domain;
  uint16_t qtype;
  uint16_t qclass;

  explicit DNSQuery(std::string domain) : domain(std::move(domain)) {
    header.id = global_query_id++;
    header.flags = 0x0100; // standard query
    header.qdcount = 1;
    header.ancount = 0;
    header.nscount = 0;
    header.arcount = 0;

    qtype = 1;  // A record
    qclass = 1; // IN class
  }

  [[nodiscard]] DNSQuerySerialised serialize() const;
};

} // namespace dnscpp
#endif // DNS_CPP_DNS_H
