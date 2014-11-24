#ifndef INETWORKSOCKET_H
# define INETWORKSOCKET_H

# include "NetworkError.hpp"

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

  virtual ~INetworkSocket() = default;
};

#endif // INETWORKSOCKET_H
