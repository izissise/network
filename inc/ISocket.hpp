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

  enum class Event
  {
    READ,
    WRITE,
    RDWR
  };

public:
  virtual ~ISocket() = default;

  virtual void closeSocket() = 0;

  void setEventRequest(Network::ISocket::Event ev) {_eventRequest = ev;};
  Network::ISocket::Event getEventRequest() const {return _eventRequest;};

protected:
  Event _eventRequest;
};

};

#endif // INETWORKSOCKET_H
