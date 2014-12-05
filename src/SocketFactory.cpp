#include "SocketFactory.hpp"

namespace Network
{

std::unique_ptr<Network::IListenSocket> SocketFactory::createListenSocket(const std::string& ip, const std::string& port,
    Network::ISocket::SockType socktype, bool reuse)
{
#ifdef UNIX
  return std::unique_ptr<Network::IListenSocket>(new Unix::ListenSocket(ip, port, socktype, reuse));
#endif
#ifdef WIN32
  return std::unique_ptr<Network::IListenSocket>(new Win::ListenSocket(ip, port, socktype, reuse));
#endif
}

std::unique_ptr<Network::IBasicSocket> SocketFactory::createConnectSocket(const std::string& ip, const std::string& port,
    Network::ISocket::SockType socktype, bool nonBlock)
{
#ifdef UNIX
  return std::unique_ptr<Network::IBasicSocket>(new Unix::ConnectSocket(ip, port, socktype, nonBlock));
#endif
#ifdef WIN32
  return std::unique_ptr<Network::IBasicSocket>(new Win::ConnectSocket(ip, port, socktype, nonBlock));
#endif
}

};
