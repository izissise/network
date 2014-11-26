#ifndef UNIXNETWORKCONNECTSOCKET_H
# define UNIXNETWORKCONNECTSOCKET_H

# include "ISocket.hpp"
# include "UnixNetworkBasicSocket.hpp"

namespace Network {
namespace Unix {

class ConnectSocket : public BasicSocket
{
public:
  ConnectSocket(const std::string& ip, const std::string& port,
                           ISocket::SockType socktype = ISocket::SockType::TCP,
                           bool nonBlock = true);
  virtual ~ConnectSocket() = default;

protected:
  static void connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
  static void connectNonBlock(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
};

};
};

#endif // UNIXNETWORKCONNECTSOCKET_H
