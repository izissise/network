#ifndef UNIXNETWORKSOCKET_H
# define UNIXNETWORKSOCKET_H

# include "INetworkConnectSocket.hpp"

# include <string>

# include <errno.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <fcntl.h>

class UnixNetworkConnectSocket : public INetworkConnectSocket
{
public:
public:
  UnixNetworkConnectSocket(const std::string& ip,
                           INetworkSocket::SockType socktype = INetworkSocket::SockType::TCP,
                           bool nonBlock = true,
                           const std::string& port = "0");
  virtual ~UnixNetworkConnectSocket();

  bool isConnected() override;

  void closeConnection() override;

  const std::string& getIpAddr() const override {return _ip;};
  uint16_t getPort() const override {return _port;};
  const std::string& getRemoteIpAddr() const override {return _remoteIp;};
  uint16_t getRemotePort() const override {return _remotePort;};
  INetworkSocket::SockType getSockType() const override {return _socktype;};

  size_t write(const std::vector<uint8_t>& data) override;
  void read(std::vector<uint8_t>& data, size_t size) override;

private:
  void connect();
  void connectNonBlock();

private:
	static std::string ipAddr(const struct sockaddr_storage& addr);
	static uint16_t portNumber(const struct sockaddr_storage& addr);

private:
  struct sockaddr_storage	_addr;
  socklen_t				    _addrlen;
  int						_socket;

  INetworkSocket::SockType	_socktype;
  std::string				_ip;
  uint16_t					_port;
  std::string				_remoteIp;
  uint16_t					_remotePort;
  bool						_connected;
};

#endif // UNIXNETWORKSOCKET_H
