#include <csignal>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <uvw.hpp>

void on_resolved(std::shared_ptr<uvw::TCPHandle> client,
                 const std::string &domain) {
  std::cout << "Resolving domain: " << domain << std::endl;
  auto resolver = client->loop().resource<uvw::GetAddrInfoReq>();

  resolver->on<uvw::ErrorEvent>(
      [client](const uvw::ErrorEvent &event, uvw::GetAddrInfoReq &) {
        std::cerr << "Error: " << event.what() << std::endl;
        std::string error_msg_str = "Error resolving domain: " + std::string(event.what()) + "\n";
        auto error_msg = std::make_unique<char[]>(error_msg_str.size() + 1);
        std::strcpy(error_msg.get(), error_msg_str.c_str());
        client->write(std::move(error_msg), error_msg_str.size() + 1);
      });

  resolver->on<uvw::AddrInfoEvent>(
      [client](const uvw::AddrInfoEvent &event, uvw::GetAddrInfoReq &) {
        char addr[17] = {'\0'};
        uv_ip4_name(reinterpret_cast<struct sockaddr_in *>(event.data->ai_addr),
                    addr, 16);
        client->write(addr, std::strlen(addr));
        client->write("\n", 1);
      });

  addrinfo hints;
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  resolver->nodeAddrInfo(domain, &hints);
}

void signal_handler(int signal) {
  if (signal == SIGINT) {
    std::cerr << "Server shutting down..." << std::endl;
    exit(0);
  }
}

int main() {
  signal(SIGINT, signal_handler);

  auto loop = uvw::Loop::getDefault();
  auto server = loop->resource<uvw::TCPHandle>();

  server->on<uvw::ErrorEvent>(
      [](const uvw::ErrorEvent &event, uvw::TCPHandle &) {
        std::cerr << "Error: " << event.what() << std::endl;
      });

  server->on<uvw::ListenEvent>([&](const uvw::ListenEvent &,
                                   uvw::TCPHandle &srv) {
    auto client = srv.loop().resource<uvw::TCPHandle>();
    srv.accept(*client);

    client->on<uvw::EndEvent>(
        [client](const uvw::EndEvent &, uvw::TCPHandle &) { client->close(); });

    client->on<uvw::DataEvent>([client](const uvw::DataEvent &event,
                                        uvw::TCPHandle &) {
      std::string domain(event.data.get(), event.length);
      domain.erase(domain.find_last_not_of(" \n\r\t") + 1); // Trim whitespace
      on_resolved(client, domain);
    });

    client->read();
  });

  server->bind("0.0.0.0", 8888);
  server->listen();

  loop->run();
  return 0;
}
