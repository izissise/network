#ifndef WINNETWORKBASICSOCKET_H
# define WINNETWORKBASICSOCKET_H

# include <string>
# include <cstring>

# include <winsock2.h>
# include <ws2tcpip.h>
# include <windows.h>

# include "ABasicSocket.hpp"
# include "Win/WinSocket.hpp"

namespace Network {
namespace Win {

class BasicSocket : public Socket, public virtual ABasicSocket
{
public:
  BasicSocket(const std::string& ip,
                           ASocket::SockType socktype = ASocket::SockType::TCP,
                           const std::string& port = "",
                           const std::function<void(int sockfd, const struct sockaddr *addr, socklen_t addrlen)>& func = [](int sockfd, const struct sockaddr *addr, socklen_t addrlen){(void)sockfd; (void)addr; (void)addrlen;});

  BasicSocket(int sockfd, ASocket::SockType socktype);

  virtual ~BasicSocket();

  bool isConnected() override;

  void closeSocket() override;

  const std::string& getIpAddr() const override {return _ip;};
  uint16_t getPort() const override {return _port;};
  const std::string& getRemoteIpAddr() const override {return _remoteIp;};
  uint16_t getRemotePort() const override {return _remotePort;};
  ASocket::SockType getSockType() const override {return Socket::getSockType();};

  size_t write(const Network::Buffer& data) override;
  void read(Network::Buffer& data, size_t size) override;

  int getSockFd() const {return Socket::getSockFd();};

protected:
  void updateInfo() override;

protected:
  std::string				_ip;
  uint16_t					_port;
  std::string				_remoteIp;
  uint16_t					_remotePort;
  bool						_connected;
};

};
};

#endif // WinNETWORKSOCKET_H
