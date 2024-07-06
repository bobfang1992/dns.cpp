#pragma once
#include "ipv4.h"

#include <array>
#include <cstddef>
#include <optional>
#include <span>
#include <string>

#include <vector>

namespace dnscpp {
namespace {
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
} // namespace

struct UDPResult {
  NetAddress address;
  std::vector<std::byte> data;
};

class UDPSocket {
public:
  UDPSocket(const IPV4 &addr, int32_t port,
            std::optional<int32_t> local_port = std::nullopt)
      : fd(-1), server_addr() {
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
      throw std::runtime_error("Failed to create socket");
    }
    if (local_port) {
      sockaddr_in local_addr{};
      local_addr.sin_family = AF_INET;
      local_addr.sin_port = htons(local_port.value());
      local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
      if (bind(fd, reinterpret_cast<const sockaddr *>(&local_addr),
               sizeof(local_addr)) == -1) {
        throw std::runtime_error("Failed to bind socket");
      }
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(addr.to_int());
  }

  ~UDPSocket() {
    if (fd != -1) {
      close(fd);
    }
  }

  [[nodiscard]] bool send(const std::span<std::byte> &data) const {
    ssize_t sent_bytes = sendto(
        fd, data.data(), data.size(), 0,
        reinterpret_cast<const sockaddr *>(&server_addr), sizeof(server_addr));
    if (sent_bytes == -1) {
      return false;
    }
    return true;
  }

  [[nodiscard]] std::optional<UDPResult> receive() const {
    std::array<std::byte, 4096> buffer{};
    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    ssize_t received_bytes =
        recvfrom(fd, buffer.data(), buffer.size(), 0,
                 reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);
    if (received_bytes == -1) {
      return std::nullopt;
    }
    return {UDPResult{from_sockaddr(client_addr),
                      std::vector<std::byte>(buffer.begin(),
                                             buffer.begin() + received_bytes)}};
  }

private:
  int fd;
  sockaddr_in server_addr;
};

/**
 * Send data to the specified address and port
 * @param addr The address to send the data to
 * @param port The port to send the data to
 * @param data The data to send
 * @return The response from the server
 */
std::optional<std::vector<std::byte>>
send_udp(IPV4 addr, int32_t port, const std::span<std::byte> &data);

} // namespace dnscpp
