#include "UnixNetworkBasicSocket.hpp"

namespace Network {
namespace UnixNetwork {

BasicSocket::BasicSocket(const std::string& ip,
    ISocket::SockType socktype,
    const std::string& port,
    const std::function<void(int sockfd, const struct sockaddr *addr, socklen_t addrlen)>& func)
  : Socket::Socket(ip, socktype, port, func), _connected(false)
{
  updateInfo();
}

BasicSocket::BasicSocket(int sockfd, ISocket::SockType socktype)
  : Socket::Socket(sockfd, socktype), _connected(true)
{
  updateInfo();
}

BasicSocket::~BasicSocket()
{
}

bool BasicSocket::isConnected()
{
  if (_connected)
    return true;

  int			ret;
  fd_set		fdset;
  struct timeval	tv;

  tv.tv_sec = 0;
  tv.tv_usec = 1;
  FD_ZERO(&fdset);
  FD_SET(_socket, &fdset);
  if ((ret = (select(_socket + 1, NULL, &fdset, NULL, &tv)) <= 0))
    {
      if (ret == -1)
        throw Error(strerror(errno));
      return false;
    }

  socklen_t	len = sizeof(int);
  if ((getsockopt(_socket, SOL_SOCKET, SO_ERROR, &ret, &len) == -1)
      || (ret > 0) || getsockname(_socket,
                                  reinterpret_cast<struct sockaddr*>(&_addr),
                                  &_addrlen) == -1)
    {
      errno = (ret > 0) ? ret : errno;
      throw Error(strerror(errno));
    }
  updateInfo();
  _connected = true;
  return _connected;
}

void BasicSocket::closeSocket()
{
  Socket::closeSocket();
  _connected = false;
}

size_t BasicSocket::write(const std::vector<uint8_t>& data)
{
  int ret;

  ret = ::write(_socket, data.data(), data.size());
  if (ret == -1)
    throw Error(strerror(errno));
  return ret;
}

void BasicSocket::read(std::vector<uint8_t>& data, size_t size)
{
  int ret;
  uint8_t *buff = new uint8_t[size];

  ret = ::read(_socket, buff, size);
  if (ret == -1)
    {
      delete[] buff;
      throw Error(strerror(errno));
    }
  data.resize(ret);
  for (size_t i = 0; i < size; ++i)
    data[i] = buff[i];
  delete[] buff;
}

void BasicSocket::updateInfo()
{
  struct sockaddr_storage peerAddr;

  Socket::updateInfo();

  socklen_t peerAddrlen = sizeof(decltype(peerAddr));
  int ret = getpeername(_socket, reinterpret_cast<struct sockaddr*>(&peerAddr), &peerAddrlen);
  if (ret)
    throw Error(strerror(errno));

  _ip = ipAddr(_addr);
  _port = portNumber(_addr);
  _remoteIp = ipAddr(peerAddr);
  _remotePort = portNumber(peerAddr);
}

};
};
