#ifndef INETWORKLISTENSOCKET_HPP
# define INETWORKLISTENSOCKET_HPP

# include <string>
# include <memory>

# include "INetworkSocket.hpp"
# include "INetworkBasicSocket.hpp"

namespace Network {

class INetworkListenSocket : virtual public INetworkSocket
{
public:
  virtual ~INetworkListenSocket() = default;

  /**
   * Close connection
  **/
  virtual void closeSocket() = 0;

  /**
   * Return the current local ip, port, type
  **/
  virtual const std::string& getListeningIpAddr() const = 0;
  virtual uint16_t getListeningPort() const = 0;
  virtual INetworkSocket::SockType getSockType() const = 0;

  /**
   * Accept a client
  **/
  virtual std::unique_ptr<INetworkBasicSocket> acceptClient() = 0;

};

};

#endif // INETWORKLISTENSOCKET_HPP
