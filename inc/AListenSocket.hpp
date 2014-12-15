#ifndef INETWORKLISTENSOCKET_HPP
# define INETWORKLISTENSOCKET_HPP

# include <string>
# include <memory>
# include <functional>

# include "Identity.hpp"
# include "ASocket.hpp"
# include "ABasicSocket.hpp"

namespace Network {

class AListenSocket : public std::enable_shared_from_this<AListenSocket>, virtual public ASocket
{
public:
  AListenSocket() {setEventRequest(Network::ASocket::Event::READ);};
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
  virtual ASocket::SockType getSockType() const = 0;

  /**
   * Accept a client
  **/
  virtual std::unique_ptr<ABasicSocket> acceptClient() = 0;

  /**
   * Callback when a new client is can be accepted,
   * the callback is the one calling acceptClient
  **/
  void setAcceptorCallback(const std::function<void(const std::weak_ptr<Network::AListenSocket>& that)>& cb)
  {
    _acceptorCb = std::bind(cb, shared_from_this());
  };

  /**
   * Callback when a new client connect through UDP
   * it's called with the info returned by recvFrom
  **/
  void setNewConnectionCallback(const std::function<void(const std::weak_ptr<Network::AListenSocket>& that,
                                const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data)>& cb)
  {
    _newConnectionCb = std::bind(cb, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
  };

  const std::function<void(const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data)>& getNewConnectionCallback() const {return _newConnectionCb;};
  const std::function<void()>& getAcceptorCallback() const {return _acceptorCb;};

  /**
   * UDP clients helpers
  **/
  virtual Network::Identity recvFrom(Network::Buffer& data, size_t size) = 0;
  virtual size_t sendTo(const Network::Identity& cli, const Network::Buffer& data) = 0;

protected:
  std::function<void(const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data)> _newConnectionCb;
  std::function<void()> _acceptorCb;
};

};

#endif // INETWORKLISTENSOCKET_HPP
