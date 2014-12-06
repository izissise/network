#ifndef NETWORKFACTORY_H
# define NETWORKFACTORY_H

# include <memory>
# include <string>

# include "ASocket.hpp"
# include "AListenSocket.hpp"
# include "ABasicSocket.hpp"

# include "Config.h"
# ifdef UNIX
#  include "Unix/UnixNetworkBasicSocket.hpp"
#  include "Unix/UnixNetworkListenSocket.hpp"
#  include "Unix/UnixNetworkConnectSocket.hpp"
# endif
# ifdef WIN32
#  include "Win/WinNetworkBasicSocket.hpp"
#  include "Win/WinNetworkListenSocket.hpp"
#  include "Win/WinNetworkConnectSocket.hpp"
# endif

namespace Network
{

class NetworkFactory
{
public:
  NetworkFactory() = delete;
  ~NetworkFactory() = delete;

  static std::unique_ptr<Network::AListenSocket> createListenSocket(const std::string& ip, const std::string& port,
      Network::ASocket::SockType socktype = Network::ASocket::SockType::TCP, bool reuse = true);
  static std::unique_ptr<Network::ABasicSocket> createConnectSocket(const std::string& ip, const std::string& port,
      Network::ASocket::SockType socktype = Network::ASocket::SockType::TCP, bool nonBlock = false);
  template<typename F1>
  static std::unique_ptr<Network::ASocket> createSocket(const std::string& ip, const std::string& port,
      Network::ASocket::SockType socktype, F1 func)
  {
#ifdef UNIX
    return std::unique_ptr<Network::ASocket>(new Unix::Socket(ip, port, socktype, func));
#endif
#ifdef WIN32
    return std::unique_ptr<Network::ASocket>(new Win::Socket(ip, port, socktype, func));
#endif
  };

};
};

#endif // SOCKETFACTORY_H
