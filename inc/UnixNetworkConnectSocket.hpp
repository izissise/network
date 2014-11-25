#ifndef UNIXNETWORKCONNECTSOCKET_H
# define UNIXNETWORKCONNECTSOCKET_H

# include "INetworkSocket.hpp"
# include "UnixNetworkBasicSocket.hpp"

namespace Network {
namespace UnixNetwork {

class UnixNetworkConnectSocket : public UnixNetworkBasicSocket
{
public:
  UnixNetworkConnectSocket(const std::string& ip,
                           INetworkSocket::SockType socktype = INetworkSocket::SockType::TCP,
                           bool nonBlock = true,
                           const std::string& port = "0");
  virtual ~UnixNetworkConnectSocket() = default;

protected:
  static void connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
  static void connectNonBlock(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
};

};
};

#endif // UNIXNETWORKCONNECTSOCKET_H
