#ifndef UNIXNETWORKBASICSOCKET_H
# define UNIXNETWORKBASICSOCKET_H

# include <string>
# include <cstring>

# include <errno.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <fcntl.h>

# include "INetworkBasicSocket.hpp"
# include "UnixSocket.hpp"

namespace Network {
namespace UnixNetwork {

class UnixNetworkBasicSocket : public UnixSocket, public virtual INetworkBasicSocket
{
public:
public:
  UnixNetworkBasicSocket(const std::string& ip,
                           INetworkSocket::SockType socktype = INetworkSocket::SockType::TCP,
                           const std::string& port = "0",
                           const std::function<void(int sockfd, const struct sockaddr *addr, socklen_t addrlen)>& func = [](int sockfd, const struct sockaddr *addr, socklen_t addrlen){(void)sockfd; (void)addr; (void)addrlen;});

  UnixNetworkBasicSocket(int sockfd, INetworkSocket::SockType socktype);

  virtual ~UnixNetworkBasicSocket();

  bool isConnected() override;

  void closeSocket() override;

  const std::string& getIpAddr() const override {return _ip;};
  uint16_t getPort() const override {return _port;};
  const std::string& getRemoteIpAddr() const override {return _remoteIp;};
  uint16_t getRemotePort() const override {return _remotePort;};
  INetworkSocket::SockType getSockType() const override {return UnixSocket::getSockType();};

  size_t write(const std::vector<uint8_t>& data) override;
  void read(std::vector<uint8_t>& data, size_t size) override;

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

#endif // UNIXNETWORKSOCKET_H
