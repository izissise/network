#ifndef INETWORKSOCKET_H
# define INETWORKSOCKET_H

# include <string>
# include <cstring>

# include "Error.hpp"

namespace Network {

typedef std::string Buffer;

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

struct Identity
{
  Identity(const std::string& Ip, uint16_t Port)
    : ip(Ip), port(Port)
  {
  };

  Identity(const Identity&) = default;
  Identity& operator=(const Identity&) = default;

  ~Identity() = default;

  bool operator==(const Identity&);

  std::string ip;
  uint16_t    port;
};

};

#endif // INETWORKSOCKET_H
