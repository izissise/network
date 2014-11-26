#include "UnixNetworkConnectSocket.hpp"

namespace Network {
namespace UnixNetwork {

UnixNetworkConnectSocket::UnixNetworkConnectSocket(const std::string& ip, const std::string& port,
    INetworkSocket::SockType socktype,
    bool nonBlock)
  : UnixNetworkBasicSocket::UnixNetworkBasicSocket(ip, socktype, port,
      nonBlock ? &UnixNetworkConnectSocket::connectNonBlock : &UnixNetworkConnectSocket::connect)
{
  if (!nonBlock)
    {
      _connected = true;
      updateInfo();
    }
}


void UnixNetworkConnectSocket::connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int ret;

  ret = ::connect(sockfd, addr, addrlen);
  if (ret)
    throw NetworkError(strerror(errno));
}

void UnixNetworkConnectSocket::connectNonBlock(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int	ret;

  auto setFdFlag = [](int fd, int flag, int unset) -> bool {
    int	flags;

    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
      return false;
    flags = unset ? (flags & ~flag) : (flags | flag);
    if (fcntl(fd, F_SETFL, flags) == -1)
      return false;
    return true;
  };

  if (((ret = setFdFlag(sockfd, O_NONBLOCK, 0)) == -1)
      || (((ret = ::connect(sockfd, addr, addrlen)) == -1)
          && (errno != EINPROGRESS))
      || ((ret = setFdFlag(sockfd, O_NONBLOCK, 1)) == -1))
    throw NetworkError(strerror(errno));
}

};
};
