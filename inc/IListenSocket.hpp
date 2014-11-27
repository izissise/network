#ifndef INETWORKLISTENSOCKET_HPP
# define INETWORKLISTENSOCKET_HPP

# include <string>
# include <memory>

# include "ISocket.hpp"
# include "IBasicSocket.hpp"

namespace Network {

class IListenSocket : virtual public ISocket
{
public:
  virtual ~IListenSocket() = default;

  /**
   * Close connection
  **/
  virtual void closeSocket() = 0;

  /**
   * Return the current local ip, port, type
  **/
  virtual const std::string& getListeningIpAddr() const = 0;
  virtual uint16_t getListeningPort() const = 0;
  virtual ISocket::SockType getSockType() const = 0;

  /**
   * Accept a client
  **/
  virtual std::unique_ptr<IBasicSocket> acceptClient() = 0;

  /**
   * Udp clients helpers
  **/
  virtual Network::Identity recvFrom(Network::Buffer& data, size_t size) = 0;
  virtual size_t sendTo(const Network::Identity& cli, const Network::Buffer& data) = 0;
};

};

#endif // INETWORKLISTENSOCKET_HPP
