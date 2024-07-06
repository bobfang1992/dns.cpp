//
// Created by Bob Fang on 7/6/24.
//

#include "networking.h"

namespace dnscpp {
std::optional<std::vector<std::byte>>
send_udp(IPV4 addr, int32_t port, const std::span<std::byte> &data) {
  UDPSocket socket(addr, port);
  auto success = socket.send(data);
  if (!success) {
    return std::nullopt;
  }
  auto received = socket.receive();
  return received.has_value() ? std::make_optional(received.value().data)
                              : std::nullopt;
}
} // namespace dnscpp