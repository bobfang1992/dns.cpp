#pragma once

#include "dns.h"
#include "ipv4.h"
#include "networking.h"
#include "root-server.h"

namespace dnscpp {

struct Resolver {
  [[nodiscard]] static IPV4 resolve(std::string_view domain) {
    DNSQuery query((std::string(domain)));
    auto serialised = query.serialize();
    UDPSocket socket(root_servers[0].second, 53, 12346);
    auto success =
        socket.send(std::span(serialised.buffer.data(), serialised.size));
    if (!success) {
      throw std::runtime_error("Failed to send data");
    }
    auto data = socket.receive();
    if (!data) {
      throw std::runtime_error("Failed to receive data");
    }
    return {};
  }
};

} // namespace dnscpp