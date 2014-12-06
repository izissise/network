#ifndef INETWORKBASICSOCKET_H
# define INETWORKBASICSOCKET_H

# include <string>
# include <functional>
# include <memory>
# include <cstdint>

# include "ISocket.hpp"

namespace Network {

class ABasicSocket : virtual public ISocket
{
public:
  virtual ~ABasicSocket() = default;

  /**
   * Can throw
  **/
  virtual bool isConnected() = 0;

  /**
   * Close connection
  **/
  virtual void closeSocket() = 0;

  /**
   * Read/Write on socket
  **/
  virtual size_t write(const Network::Buffer& data) = 0;
  virtual void read(Network::Buffer& data, size_t size) = 0;

  /**
   * Return the current local ip, port, type
  **/
  virtual const std::string& getIpAddr() const = 0;
  virtual uint16_t getPort() const = 0;
  virtual const std::string& getRemoteIpAddr() const = 0;
  virtual uint16_t getRemotePort() const = 0;
  virtual ISocket::SockType getSockType() const = 0;

  /**
   * Callback when readable or writable
  **/
  void setReadeableCallback(const std::function<void()>& cb) {_readeableCb = cb;};
  void setWritableCallback(const std::function<void()>& cb) {_writableCb = cb;};
  const std::function<void()>& getReadeableCallback() const {return _readeableCb;};
  const std::function<void()>& setWritableCallback() const {return _writableCb;};

protected:
	std::function<void()> _readeableCb;
	std::function<void()> _writableCb;
};

};

#endif // INETWORKCONNECTSOCKET_H
