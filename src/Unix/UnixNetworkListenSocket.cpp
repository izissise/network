#include "Unix/UnixNetworkListenSocket.hpp"

#include <sstream>

#include "Unix/UnixNetworkBasicSocket.hpp"

namespace Network {
namespace Unix {

ListenSocket::ListenSocket(const std::string& listeningIp, const std::string& port,
                           ISocket::SockType socktype,
                           bool reuse)
  : Socket::Socket(listeningIp, socktype, port, reuse ? &ListenSocket::bindReuse : &ListenSocket::bind)
{
  if (socktype == ISocket::SockType::TCP)
    ::listen(_socket, 50);
  updateInfo();
}

void ListenSocket::updateInfo()
{
  Socket::updateInfo();

  _ip = ipAddr(_addr);
  _port = portNumber(_addr);
}

std::unique_ptr<ABasicSocket> ListenSocket::acceptClient()
{
  if (_socktype != ISocket::SockType::TCP)
    throw std::runtime_error("acceptClient not implemented for non TCP socket.");

  int newfd = ::accept(_socket, nullptr, nullptr);
  if (newfd == -1)
    throw Error(strerror(errno));
  return std::unique_ptr<ABasicSocket>(new BasicSocket(newfd, _socktype));
}

Network::Identity ListenSocket::recvFrom(Network::Buffer& data, size_t size)
{
  if (_socktype != ISocket::SockType::UDP)
    throw std::runtime_error("recvFrom not implemented for non UDP socket.");

  int 						ret;
  struct sockaddr_storage	raddr;
  socklen_t				    raddrlen = sizeof(decltype(raddr));
  std::unique_ptr<char[]> 	buff(new char[size]);

  std::memset(&raddr, 0, raddrlen);
  ret = recvfrom(_socket, buff.get(), size, 0,
                 reinterpret_cast<struct sockaddr*>(&raddr), &raddrlen);
  if (ret == -1)
    throw Error(strerror(errno));
  data.resize(ret);
  data.assign(buff.get(), ret);
  return Network::Identity(ipAddr(raddr), portNumber(raddr));
}

size_t ListenSocket::sendTo(const Network::Identity &cli, const Network::Buffer& data)
{
  if (_socktype != ISocket::SockType::UDP)
    throw std::runtime_error("sendTo not implemented for non UDP socket.");

  int 						ret;
  struct sockaddr_storage	raddr = _addr;
  struct addrinfo			req;
  struct addrinfo 			*res;
  struct addrinfo 			*tmp;

  std::memset(&req, 0, sizeof(decltype(req)));
  req.ai_family = raddr.ss_family;
  req.ai_socktype = sockTypeToInt(_socktype);
  ret = getaddrinfo(cli.ip.c_str(), [](uint16_t port) {
    std::stringstream ss;
    ss << port;
    return ss.str();
  }(cli.port).c_str(), &req, &res);
  if (ret != 0)
    throw Error(gai_strerror(ret));
  tmp = res;
  while (tmp)
    {
      ret = sendto(_socket, data.data(), data.size(), 0, tmp->ai_addr, tmp->ai_addrlen);
      if (ret != -1)
        break;
      tmp = tmp->ai_next;
    }
  freeaddrinfo(res);
  if (ret == -1)
    throw Error(strerror(errno));
  return ret;
}

void ListenSocket::bind(int sockfd, const struct sockaddr * addr, socklen_t addrlen)
{
  int ret = ::bind(sockfd, addr, addrlen);
  if (ret)
    throw Error(strerror(errno));
}

void ListenSocket::bindReuse(int sockfd, const struct sockaddr * addr, socklen_t addrlen)
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
