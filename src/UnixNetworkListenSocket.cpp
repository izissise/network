#include "UnixNetworkListenSocket.hpp"

#include "UnixNetworkBasicSocket.hpp"

namespace Network {
namespace UnixNetwork {

UnixNetworkListenSocket::UnixNetworkListenSocket(const std::string& listeningIp, const std::string& port,
    INetworkSocket::SockType socktype, bool reuse)
  : UnixSocket::UnixSocket(listeningIp, socktype, port, reuse ? &UnixNetworkListenSocket::bindReuse : &UnixNetworkListenSocket::bind)
{
  ::listen(_socket, 50);
}

void UnixNetworkListenSocket::updateInfo()
{
  UnixSocket::updateInfo();

  _ip = ipAddr(_addr);
  _port = portNumber(_addr);
}

std::unique_ptr<INetworkBasicSocket> UnixNetworkListenSocket::acceptClient()
{
  int newfd = ::accept(_socket, nullptr, nullptr);
  if (newfd == -1)
    throw NetworkError(strerror(errno));
  return std::unique_ptr<INetworkBasicSocket>(new UnixNetworkBasicSocket(newfd, _socktype));
}

void UnixNetworkListenSocket::bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int ret = ::bind(sockfd, addr, addrlen);
  if (ret)
    throw NetworkError(strerror(errno));
}

void UnixNetworkListenSocket::bindReuse(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int	yes;

  yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    throw NetworkError(strerror(errno));
  int ret = ::bind(sockfd, addr, addrlen);
  if (ret)
    throw NetworkError(strerror(errno));
}

};
};
