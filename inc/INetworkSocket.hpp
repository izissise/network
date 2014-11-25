#ifndef INETWORKSOCKET_H
# define INETWORKSOCKET_H

# include "NetworkError.hpp"

namespace Network {

class INetworkSocket
{
public:
  enum class SockType
  {
    TCP,
    UDP,
    RAW
  };

public:
  virtual ~INetworkSocket() = default;

  virtual void closeSocket() = 0;

};

};

#endif // INETWORKSOCKET_H
