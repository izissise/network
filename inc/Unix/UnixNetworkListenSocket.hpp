#ifndef UNIXNETWORKLISTENSOCKET_HPP
# define UNIXNETWORKLISTENSOCKET_HPP

# include <functional>
# include <memory>
# include <string>
# include <cstdint>

# include "Unix/UnixSocket.hpp"
# include "AListenSocket.hpp"

namespace Network {
namespace Unix {

class ListenSocket : public Socket, virtual public AListenSocket
{
public:
  ListenSocket(const std::string& listeningIp, const std::string& port,
               ASocket::SockType socktype = ASocket::SockType::TCP,
               bool reuse = true);
  virtual ~ListenSocket() = default;

  void closeSocket() override {Socket::closeSocket();};

  std::unique_ptr<ABasicSocket> acceptClient() override;

  Network::Identity recvFrom(Network::Buffer& data, size_t size) override;

  size_t sendTo(const Network::Identity& cli, const Network::Buffer& data) override;

  const std::string& getListeningIpAddr() const override {return _ip;};
  uint16_t getListeningPort() const override {return _port;};
  ASocket::SockType getSockType() const override {return Socket::getSockType();};

protected:
  static void bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
  static void bindReuse(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

  void updateInfo() override;

protected:
  std::string _ip;
  uint16_t	_port;
};

};
};

#endif // UNIXNETWORKLISTENSOCKET_HPP
