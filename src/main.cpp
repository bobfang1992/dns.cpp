#include "resolver.h"
#include <iostream>

int main() {
  auto ip = dnscpp::Resolver::resolve("google.com");
  std::cout << "result = " << dnscpp::to_string(ip) << std::endl;
  return 0;
}