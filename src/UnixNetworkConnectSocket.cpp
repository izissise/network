#include "UnixNetworkConnectSocket.hpp"

#include <map>
#include <cstring>

UnixNetworkConnectSocket::UnixNetworkConnectSocket(const std::string& ip,
    INetworkSocket::SockType socktype,
    bool nonBlock,
    const std::string& port)
  : _socket(-1), _socktype(socktype), _connected(false)
{
  struct addrinfo			req;
  struct addrinfo			*res;
  struct addrinfo			*tmp;
  int						ret;
  std::map<INetworkSocket::SockType, int> types = {
    {INetworkSocket::SockType::TCP, SOCK_STREAM},
    {INetworkSocket::SockType::UDP, SOCK_DGRAM},
    {INetworkSocket::SockType::RAW, SOCK_RAW}
  };

  std::memset(&req, 0, sizeof(struct addrinfo));
  req.ai_family = AF_UNSPEC;
  req.ai_socktype = types.at(socktype);
  req.ai_flags = AI_PASSIVE | AI_NUMERICSERV | AI_ADDRCONFIG;
  if ((ret = getaddrinfo(ip.c_str(), port.c_str(), &req, &res)))
    throw NetworkError(gai_strerror(ret));
  tmp = res;
  while (tmp)
    {
      _socket = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
      if (_socket == -1)
        throw NetworkError(strerror(errno));

      ret = 0;
      std::memcpy(&_addr, tmp->ai_addr, tmp->ai_addrlen);
      _addr.ss_family = tmp->ai_addr->sa_family;
      // _socktype = tmp->ai_socktype;
      _addrlen = tmp->ai_addrlen;

      try {
          if (nonBlock)
            connectNonBlock();
          else
            connect();
        }
      catch (NetworkError& e)
        {
          close(_socket);
          _socket = -1;
          ret = -1;
        }
      if ((!ret) && (getsockname(_socket, reinterpret_cast<struct sockaddr*>(&_addr),
                                 &_addrlen)) == -1)
        {
          close(_socket);
          _socket = -1;
          ret = -1;
        }

      tmp = tmp->ai_next;
    }
  freeaddrinfo(res);
  if (ret == -1)
    throw NetworkError(strerror(errno));

  struct sockaddr_storage peerAddr;
  socklen_t peerAddrlen = sizeof(struct sockaddr_storage);
  ret = getpeername(_socket, reinterpret_cast<struct sockaddr*>(&peerAddr), &peerAddrlen);
  if (ret)
    throw NetworkError(strerror(errno));

  _ip = ipAddr(_addr);
  _port = portNumber(_addr);
  _remoteIp = ipAddr(peerAddr);
  _remotePort = portNumber(peerAddr);
}

UnixNetworkConnectSocket::~UnixNetworkConnectSocket()
{
  closeConnection();
}

void UnixNetworkConnectSocket::connect()
{
  int ret;

  ret = ::connect(_socket, reinterpret_cast<struct sockaddr*>(&_addr), _addrlen);
  if (ret)
    throw NetworkError(strerror(errno));
  _connected = true;
}

void UnixNetworkConnectSocket::connectNonBlock()
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

  if (((ret = setFdFlag(_socket, O_NONBLOCK, 0)) == -1)
      || (((ret = ::connect(_socket, reinterpret_cast<struct sockaddr*>(&_addr), _addrlen)) == -1)
          && (errno != EINPROGRESS))
      || ((ret = setFdFlag(_socket, O_NONBLOCK, 1)) == -1))
    throw NetworkError(strerror(errno));
}

bool UnixNetworkConnectSocket::isConnected()
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
        throw NetworkError(strerror(errno));
      return false;
    }

  socklen_t	len = sizeof(int);
  if ((getsockopt(_socket, SOL_SOCKET, SO_ERROR, &ret, &len) == -1)
      || (ret > 0) || getsockname(_socket,
                                  reinterpret_cast<struct sockaddr*>(&_addr),
                                  &_addrlen) == -1)
    {
      errno = (ret > 0) ? ret : errno;
      throw NetworkError(strerror(errno));
    }
  _connected = true;
  return _connected;
}

void UnixNetworkConnectSocket::closeConnection()
{
  if (_socket != -1)
    close(_socket);
  _connected = false;
}

size_t UnixNetworkConnectSocket::write(const std::vector<uint8_t>& data)
{
  int ret;

  ret = ::write(_socket, data.data(), data.size());
  if (ret == -1)
    throw NetworkError(strerror(errno));
  return ret;
}

void UnixNetworkConnectSocket::read(std::vector<uint8_t>& data, size_t size)
{
  int ret;
  uint8_t *buff = new uint8_t[size];

  ret = ::read(_socket, buff, size);
  if (ret == -1)
    {
      delete[] buff;
      throw NetworkError(strerror(errno));
    }
  data.resize(ret);
  for (size_t i = 0; i < size; ++i)
    data[i] = buff[i];
  delete[] buff;
}

std::string UnixNetworkConnectSocket::ipAddr(const struct sockaddr_storage& addr)
{
  const struct sockaddr	*sa;
  const void			*res;
  char					buff[BUFSIZ];

  res = NULL;
  sa = reinterpret_cast<const struct sockaddr*>(&addr);
  if (sa->sa_family == AF_INET)
    res = reinterpret_cast<const void*>(&((reinterpret_cast<const struct sockaddr_in*>(sa))->sin_addr));
  else if (sa->sa_family == AF_INET6)
    res = reinterpret_cast<const void*>(&((reinterpret_cast<const struct sockaddr_in6*>(sa))->sin6_addr));
  else
    throw NetworkError("Unknown socket family");
  inet_ntop((reinterpret_cast<const struct sockaddr*>(&addr))->sa_family,
            res, buff, sizeof(buff));
  return std::string(buff);
}

uint16_t UnixNetworkConnectSocket::portNumber(const struct sockaddr_storage& addr)
{
  in_port_t		port;
  const struct sockaddr	*tmp;

  tmp = reinterpret_cast<const struct sockaddr*>(&addr);
  port = 0;
  if (tmp->sa_family == AF_INET)
    port = ((reinterpret_cast<const struct sockaddr_in*>(tmp))->sin_port);
  else if (tmp->sa_family == AF_INET6)
    port = ((reinterpret_cast<const struct sockaddr_in6*>(tmp))->sin6_port);
  else
    throw NetworkError("Unknown socket family");
  return ntohs(port);
}
