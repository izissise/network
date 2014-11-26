#ifndef INETWORKSOCKET_H
# define INETWORKSOCKET_H

# include "Error.hpp"

namespace Network {

class ISocket
{
public:
  enum class SockType
  {
    TCP,
    UDP,
    RAW
  };

public:
  virtual ~ISocket() = default;

  virtual void closeSocket() = 0;

};

};

#endif // INETWORKSOCKET_H
