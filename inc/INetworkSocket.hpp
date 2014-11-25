#ifndef INETWORKSOCKET_H
# define INETWORKSOCKET_H

# include "NetworkError.hpp"

#include <iostream>

namespace Network {

class INetworkSocket
{
public:
  const int MAX_QUEUE = 50;

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

};

#endif // INETWORKSOCKET_H
