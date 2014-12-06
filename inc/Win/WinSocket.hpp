#ifndef WINSOCKET_H
# define WINSOCKET_H

# include <string>
# include <map>
# include <functional>
# include <cstdint>

# include <winsock2.h>
# include <ws2tcpip.h>
# include <windows.h>

# include "ASocket.hpp"

namespace Network {
namespace Win {

typedef unsigned short in_port_t;

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
  static char* inet_ntop(int af, const void* src, char* dest, size_t length);

protected:
  struct sockaddr_storage	_addr;
  socklen_t				    _addrlen;
  int						_socket;
  ASocket::SockType			_socktype;
};

};
};

#endif // WinSOCKET_H
