//
// Created by Bob Fang on 7/6/24.
//
#include "dns.h"
#include <ranges>
#include <vector>
namespace dnscpp {

std::vector<uint8_t> encode_dns_name(const std::string &domain_name) {
  std::vector<uint8_t> encoded;

  for (auto part : domain_name | std::views::split('.')) {
    std::string part_str(part.begin(), part.end());
    encoded.push_back(static_cast<uint8_t>(part_str.size()));
    encoded.insert(encoded.end(), part_str.begin(), part_str.end());
  }

  // Add the final null byte to terminate the DNS name
  encoded.push_back(0);

  return encoded;
}

DNSQuerySerialised DNSQuery::serialize() const {
  std::array<std::byte, 512> buffer{};
  size_t current_pointer = 0;

  // encode header
  auto id = htons(header.id);
  std::memcpy(buffer.data(), &id, sizeof(id));
  current_pointer += sizeof(id);

  auto flags = htons(header.flags);
  std::memcpy(buffer.data() + current_pointer, &flags, sizeof(flags));
  current_pointer += sizeof(flags);

  auto qdcount = htons(header.qdcount);
  std::memcpy(buffer.data() + current_pointer, &qdcount, sizeof(qdcount));
  current_pointer += sizeof(qdcount);

  auto ancount = htons(header.ancount);
  std::memcpy(buffer.data() + current_pointer, &ancount, sizeof(ancount));
  current_pointer += sizeof(ancount);

  auto nscount = htons(header.nscount);
  std::memcpy(buffer.data() + current_pointer, &nscount, sizeof(nscount));
  current_pointer += sizeof(nscount);

  auto arcount = htons(header.arcount);
  std::memcpy(buffer.data() + current_pointer, &arcount, sizeof(arcount));
  current_pointer += sizeof(arcount);

  // encode query
  auto encoded_domain = encode_dns_name(domain);
  std::memcpy(buffer.data() + current_pointer, encoded_domain.data(),
              encoded_domain.size());
  current_pointer += encoded_domain.size();
  // encode type
  auto qtype_s = htons(qtype);
  std::memcpy(buffer.data() + current_pointer, &qtype_s, sizeof(qtype_s));
  current_pointer += sizeof(qtype_s);

  // encode class
  auto qclass_s = htons(qclass);
  std::memcpy(buffer.data() + current_pointer, &qclass_s, sizeof(qclass_s));
  current_pointer += sizeof(qclass_s);

  return {buffer, current_pointer};
}
} // namespace dnscpp