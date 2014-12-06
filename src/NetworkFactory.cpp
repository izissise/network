#include "NetworkFactory.hpp"

namespace Network
{

std::unique_ptr<Network::AListenSocket> NetworkFactory::createListenSocket(const std::string& ip, const std::string& port,
    Network::ISocket::SockType socktype, bool reuse)
{
#ifdef UNIX
  return std::unique_ptr<Network::AListenSocket>(new Unix::ListenSocket(ip, port, socktype, reuse));
#endif
#ifdef WIN32
  return std::unique_ptr<Network::AListenSocket>(new Win::ListenSocket(ip, port, socktype, reuse));
#endif
}

std::unique_ptr<Network::ABasicSocket> NetworkFactory::createConnectSocket(const std::string& ip, const std::string& port,
    Network::ISocket::SockType socktype, bool nonBlock)
{
#ifdef UNIX
  return std::unique_ptr<Network::ABasicSocket>(new Unix::ConnectSocket(ip, port, socktype, nonBlock));
#endif
#ifdef WIN32
  return std::unique_ptr<Network::ABasicSocket>(new Win::ConnectSocket(ip, port, socktype, nonBlock));
#endif
}

};
