#include <uvw.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <cstring>

void on_resolved(std::shared_ptr<uvw::Loop> loop, const std::string &domain) {
  auto resolver = loop->resource<uvw::GetAddrInfoReq>();

  resolver->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &event, uvw::GetAddrInfoReq &) {
    std::cerr << "Error: " << event.what() << std::endl;
  });

  resolver->on<uvw::AddrInfoEvent>([](const uvw::AddrInfoEvent &event, uvw::GetAddrInfoReq &req) {
    char addr[17] = {'\0'};
    uv_ip4_name(reinterpret_cast<struct sockaddr_in *>(event.data->ai_addr), addr, 16);
    std::cout << "Resolved IP: " << addr << std::endl;
  });

  addrinfo hints;
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  resolver->nodeAddrInfo(domain, &hints);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " [domain]" << std::endl;
    return 1;
  }

  std::string domain = argv[1];

  auto loop = uvw::Loop::getDefault();
  on_resolved(loop, domain);
  loop->run();

  return 0;
}