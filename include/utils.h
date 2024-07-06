//
// Created by Bob Fang on 7/6/24.
//
#pragma once
#ifndef DNS_CPP_UTILS_H
#define DNS_CPP_UTILS_H


#include <iostream>
#include <iomanip>
#include "sql.h"
#include "resolver.h"
#include "dns.h"

std::__1::vector<std::byte> string_to_bytes(const std::string &str) {
  return {reinterpret_cast<const std::byte *>(str.data()),
          reinterpret_cast<const std::byte *>(str.data() + str.size())};
}

std::string bytes_to_string(const std::__1::vector<std::byte> &byte_vector) {
  // Reinterpret the byte data as char data and construct the string
  return {reinterpret_cast<const char *>(byte_vector.data()),
          byte_vector.size()};
}

void print_bytes_in_hex(const std::__1::vector<std::byte> &bytes,
                        size_t bytes_per_line) {
  size_t count = 0;
  for (const auto &byte : bytes) {
    if (count > 0 && count % bytes_per_line == 0) {
      std::cout << std::endl;
    }

    int byte_value = std::to_integer<int>(byte);

    if (byte_value >= 32 && byte_value <= 126) { // Printable ASCII range
      std::cout << "\033[31m" << static_cast<char>(byte_value)
                << "\033[0m "; // Red color
    } else {
      std::cout << std::hex << std::setfill('0') << std::setw(2) << byte_value
                << " ";
    }
    count++;
  }
  std::cout << std::endl;
}

#endif //DNS_CPP_UTILS_H
