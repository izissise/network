#include "UnixNetworkListenSocket.hpp"

#include "UnixNetworkBasicSocket.hpp"

namespace Network {
namespace UnixNetwork {

ListenSocket::ListenSocket(const std::string& listeningIp, const std::string& port,
								ISocket::SockType socktype,
								bool reuse)
  : Socket::Socket(listeningIp, socktype, port, reuse ? &ListenSocket::bindReuse : &ListenSocket::bind)
{
  ::listen(_socket, 50);
  updateInfo();
}

void ListenSocket::updateInfo()
{
  Socket::updateInfo();

  _ip = ipAddr(_addr);
  _port = portNumber(_addr);
}

std::unique_ptr<IBasicSocket> ListenSocket::acceptClient()
{
  if (_socktype == ISocket::SockType::TCP)
    {
      int newfd = ::accept(_socket, nullptr, nullptr);
      if (newfd == -1)
        throw Error(strerror(errno));
      return std::unique_ptr<IBasicSocket>(new BasicSocket(newfd, _socktype));
    }
  else
    {
      throw std::runtime_error("Accept not implemented for something else than TCP.");
    }
}

void ListenSocket::bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int ret = ::bind(sockfd, addr, addrlen);
  if (ret)
    throw Error(strerror(errno));
}

void ListenSocket::bindReuse(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int	yes;

  yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    throw Error(strerror(errno));
  int ret = ::bind(sockfd, addr, addrlen);
  if (ret)
    throw Error(strerror(errno));
}

};
};
