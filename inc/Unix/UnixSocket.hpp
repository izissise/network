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

# include "ASocket.hpp"

namespace Network {
namespace Unix {

class Socket : virtual public ASocket
{
public:
  static const std::map<ASocket::SockType, int> _socktypes;

public:
  Socket(const std::string& ip,
             ASocket::SockType socktype,
             const std::string& port,
             const std::function<void(int sockfd, const struct sockaddr *addr, socklen_t addrlen)>& func);

  Socket(int sockFd, ASocket::SockType socktype);

  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;

  virtual ~Socket();

  void closeSocket() override;

  int getSockFd() const {return _socket;};
  ASocket::SockType getSockType() const {return _socktype;};

protected:
  static std::string ipAddr(const struct sockaddr_storage& addr);
  static uint16_t portNumber(const struct sockaddr_storage& addr);
  static ASocket::SockType intToSockType(int t);
  static int sockTypeToInt(ASocket::SockType t);
  virtual void updateInfo();

protected:
  struct sockaddr_storage	_addr;
  socklen_t				    _addrlen;
  int						_socket;
  ASocket::SockType			_socktype;
};

};
};

#endif // UNIXSOCKET_H
