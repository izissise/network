#ifndef UNIXNETWORKCONNECTSOCKET_H
# define UNIXNETWORKCONNECTSOCKET_H

# include "ASocket.hpp"
# include "Unix/UnixNetworkBasicSocket.hpp"

namespace Network {
namespace Unix {

class ConnectSocket : public BasicSocket
{
public:
  ConnectSocket(const std::string& ip, const std::string& port,
                           ASocket::SockType socktype = ASocket::SockType::TCP,
                           bool nonBlock = false);
  virtual ~ConnectSocket() = default;

protected:
  static void connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
  static void connectNonBlock(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
};

};
};

#endif // UNIXNETWORKCONNECTSOCKET_H
