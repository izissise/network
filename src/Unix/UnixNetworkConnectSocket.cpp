#include "Unix/UnixNetworkConnectSocket.hpp"

namespace Network {
namespace Unix {

ConnectSocket::ConnectSocket(const std::string& ip, const std::string& port,
    ASocket::SockType socktype,
    bool nonBlock)
  : BasicSocket::BasicSocket(ip, socktype, port,
      nonBlock ? &ConnectSocket::connectNonBlock : &ConnectSocket::connect)
{
  if (!nonBlock)
    {
      _connected = true;
      updateInfo();
    }
}


void ConnectSocket::connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int ret;

  ret = ::connect(sockfd, addr, addrlen);
  if (ret)
    throw Error(strerror(errno));
}

void ConnectSocket::connectNonBlock(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
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
    throw Error(strerror(errno));
}

};
};
