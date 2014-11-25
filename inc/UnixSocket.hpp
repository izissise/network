#ifndef UNIXSOCKET_H
# define UNIXSOCKET_H

# include <string>
# include <map>
# include <functional>
# include <cstdint>

# include <errno.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <fcntl.h>

# include "INetworkSocket.hpp"

namespace Network {
namespace UnixNetwork {

class UnixSocket : public INetworkSocket
{
public:
  static const std::map<INetworkSocket::SockType, int> _socktypes;

public:
  UnixSocket(const std::string& ip,
             INetworkSocket::SockType socktype,
             const std::string& port,
             const std::function<void(int sockfd, const struct sockaddr *addr, socklen_t addrlen)>& func);

  UnixSocket(int sockFd, INetworkSocket::SockType socktype);

  UnixSocket(const UnixSocket&) = delete;
  UnixSocket& operator=(const UnixSocket&) = delete;

  virtual ~UnixSocket();

  void closeSocket() override;

  int getSockFd() const {return _socket;};
  INetworkSocket::SockType getSockType() const {return _socktype;};

protected:
  static std::string ipAddr(const struct sockaddr_storage& addr);
  static uint16_t portNumber(const struct sockaddr_storage& addr);
  static INetworkSocket::SockType intToSockType(int t);
  static int sockTypeToInt(INetworkSocket::SockType t);
  virtual void updateInfo();

protected:
  struct sockaddr_storage	_addr;
  socklen_t				    _addrlen;
  int						_socket;
  INetworkSocket::SockType	_socktype;
};

};
};

#endif // UNIXSOCKET_H
