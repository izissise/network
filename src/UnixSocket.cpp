#include "UnixSocket.hpp"

#include <cstring>
#include <algorithm>

namespace Network {
namespace UnixNetwork {

const std::map<INetworkSocket::SockType, int> UnixSocket::_socktypes = {
  {INetworkSocket::SockType::TCP, SOCK_STREAM},
  {INetworkSocket::SockType::UDP, SOCK_DGRAM},
  {INetworkSocket::SockType::RAW, SOCK_RAW}
};

UnixSocket::UnixSocket(const std::string& ip,
                       INetworkSocket::SockType socktype,
                       const std::string& port,
                       const std::function<void(int sockfd, const struct sockaddr *addr, socklen_t addrlen)>& func)
  : _socket(-1), _socktype(socktype)
{
  struct addrinfo			req;
  struct addrinfo			*res;
  struct addrinfo			*tmp;
  int						ret;

  std::memset(&req, 0, sizeof(decltype(req)));
  req.ai_family = AF_UNSPEC;
  req.ai_socktype = sockTypeToInt(_socktype);
  req.ai_flags = AI_PASSIVE | AI_NUMERICSERV | AI_ADDRCONFIG;
  if ((ret = getaddrinfo((ip == "") ? nullptr : ip.c_str(), (port == "") ? nullptr : port.c_str(), &req, &res)))
    throw NetworkError(gai_strerror(ret));
  tmp = res;
  while (tmp)
    {
      _socket = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
      if (_socket == -1)
        throw NetworkError(strerror(errno));

      ret = 0;
      std::memset(&_addr, 0, sizeof(decltype(_addr)));
      std::memcpy(&_addr, tmp->ai_addr, tmp->ai_addrlen);
      _addr.ss_family = tmp->ai_addr->sa_family;
      _socktype = intToSockType(tmp->ai_socktype);
      _addrlen = tmp->ai_addrlen;

      try {
          func(_socket, reinterpret_cast<const struct sockaddr*>(&_addr), _addrlen);
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
}

UnixSocket::UnixSocket(int sockfd, INetworkSocket::SockType socktype)
  : _socket(sockfd), _socktype(socktype)
{
}

UnixSocket::~UnixSocket()
{
  closeSocket();
}

void UnixSocket::closeSocket()
{
  if (_socket != -1)
    {
      close(_socket);
      _socket = -1;
    }
}

std::string UnixSocket::ipAddr(const struct sockaddr_storage& addr)
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

uint16_t UnixSocket::portNumber(const struct sockaddr_storage& addr)
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

INetworkSocket::SockType UnixSocket::intToSockType(int t)
{
  auto it = std::find_if(_socktypes.begin(), _socktypes.end(), [t](const std::pair<INetworkSocket::SockType, int>& p) {
    return p.second == t;
  });
  if (it == _socktypes.end())
    throw std::runtime_error("Not in map");
  return it->first;
}

int UnixSocket::sockTypeToInt(INetworkSocket::SockType t)
{
  return _socktypes.at(t);
}

void UnixSocket::updateInfo()
{
  _addrlen = sizeof(decltype(_addr));
  int ret = getsockname(_socket, reinterpret_cast<struct sockaddr*>(&_addr), &_addrlen);
  if (ret)
    throw NetworkError(strerror(errno));
}

};
};
