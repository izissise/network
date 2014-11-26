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

# include "ISocket.hpp"

namespace Network {
namespace UnixNetwork {

class Socket : virtual public ISocket
{
public:
  static const std::map<ISocket::SockType, int> _socktypes;

public:
  Socket(const std::string& ip,
             ISocket::SockType socktype,
             const std::string& port,
             const std::function<void(int sockfd, const struct sockaddr *addr, socklen_t addrlen)>& func);

  Socket(int sockFd, ISocket::SockType socktype);

  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;

  virtual ~Socket();

  void closeSocket() override;

  int getSockFd() const {return _socket;};
  ISocket::SockType getSockType() const {return _socktype;};

protected:
  static std::string ipAddr(const struct sockaddr_storage& addr);
  static uint16_t portNumber(const struct sockaddr_storage& addr);
  static ISocket::SockType intToSockType(int t);
  static int sockTypeToInt(ISocket::SockType t);
  virtual void updateInfo();

protected:
  struct sockaddr_storage	_addr;
  socklen_t				    _addrlen;
  int						_socket;
  ISocket::SockType			_socktype;
};

};
};

#endif // UNIXSOCKET_H
