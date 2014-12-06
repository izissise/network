#ifndef INETWORKLISTENSOCKET_HPP
# define INETWORKLISTENSOCKET_HPP

# include <string>
# include <memory>
# include <functional>

# include "Identity.hpp"
# include "ISocket.hpp"
# include "ABasicSocket.hpp"

namespace Network {

class AListenSocket : public std::enable_shared_from_this<AListenSocket>, virtual public ISocket
{
public:
  virtual ~AListenSocket() = default;

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
  virtual std::unique_ptr<ABasicSocket> acceptClient() = 0;

  /**
   * Callback when a new client connect through UDP
   * it's callback will info returned by recvFrom
  **/
  void setNewConnectionCallback(const std::function<void(const std::weak_ptr<Network::AListenSocket>& that,
                                const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data)>& cb)
  {
    _newConnectionCb = std::bind(cb, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
  };

  const std::function<void(const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data)>& getNewConnectionCallback() const {return _newConnectionCb;};

  /**
   * Udp clients helpers
  **/
  virtual Network::Identity recvFrom(Network::Buffer& data, size_t size) = 0;
  virtual size_t sendTo(const Network::Identity& cli, const Network::Buffer& data) = 0;

protected:
  std::function<void(const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data)> _newConnectionCb;
};

};

#endif // INETWORKLISTENSOCKET_HPP