#ifndef INETWORKCONNECTSOCKET_H
# define INETWORKCONNECTSOCKET_H

# include <vector>
# include <string>
# include <cstdint>

# include "INetworkSocket.hpp"

class INetworkConnectSocket : public INetworkSocket
{
public:
  virtual ~INetworkConnectSocket() = default;

  /**
   * Can throw
  **/
  virtual bool isConnected() = 0;

  /**
   * Close connection
  **/
  virtual void closeConnection() = 0;

  /**
   * Return the current local ip, port, type
  **/
  virtual const std::string& getIpAddr() const = 0;
  virtual uint16_t getPort() const = 0;
  virtual const std::string& getRemoteIpAddr() const = 0;
  virtual uint16_t getRemotePort() const = 0;
  virtual INetworkSocket::SockType getSockType() const = 0;

  virtual size_t write(const std::vector<uint8_t>& data) = 0;
  virtual void read(std::vector<uint8_t>& data, size_t size) = 0;
};

#endif // INETWORKCONNECTSOCKET_H
