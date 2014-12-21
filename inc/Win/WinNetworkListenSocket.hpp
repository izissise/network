#ifndef WINNETWORKLISTENSOCKET_HPP
# define WINNETWORKLISTENSOCKET_HPP

# include <string>
# include <cstdint>

# include "Win/WinSocket.hpp"
# include "AListenSocket.hpp"

namespace Network {
namespace Win {

class ListenSocket : public Socket, virtual public AListenSocket
{
public:
  ListenSocket(const std::string& listeningIp, const std::string& port,
               ASocket::SockType socktype = ASocket::SockType::TCP,
               bool reuse = true);
  virtual ~ListenSocket() = default;

  void closeSocket() override {Socket::closeSocket();};

  std::unique_ptr<ABasicSocket> acceptClient() override;

  Network::Identity recvFrom(Network::Buffer& data) override;

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

#endif // WinNETWORKLISTENSOCKET_HPP
