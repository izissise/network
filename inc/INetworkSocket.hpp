#ifndef INETWORKSOCKET_H
# define INETWORKSOCKET_H

# include "NetworkError.hpp"

#include <iostream>

namespace Network {

class INetworkSocket
{
public:
  static int MAX_QUEUE;

  enum class SockType
  {
    TCP,
    UDP,
    RAW
  };

public:
  virtual ~INetworkSocket() {std::cout << "INetworkSocket::~" << std::endl;};

  virtual void closeSocket() = 0;

};

int INetworkSocket::MAX_QUEUE = 50;

};

#endif // INETWORKSOCKET_H
