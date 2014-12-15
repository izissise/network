#ifndef CLIENTHELPER_H
# define CLIENTHELPER_H

# include <memory>
# include <functional>

# include "RingBuffer.hpp"
# include "ABasicSocket.hpp"
# include "AListenSocket.hpp"
# include "Identity.hpp"

namespace Network {

class SocketClientHelper
{
public:
  SocketClientHelper(size_t readSize = 10);
  SocketClientHelper(const std::shared_ptr<Network::ABasicSocket>& sock, size_t readSize = 10);
  virtual ~SocketClientHelper() = default;

  const std::shared_ptr<Network::ABasicSocket>& getSocket() const {return _socket;};
  void setSocket(const std::shared_ptr<Network::ABasicSocket>& sock);

  bool isConnected() const {return _connected;};

private:
  void onReadeable();
  void onWritable();

protected:
  virtual void onRead(size_t sizeRead) = 0;
  virtual void onWrite(size_t sizeWrite) = 0;
  virtual void onDisconnet() = 0;

protected:
  size_t								 _readSize;
  Network::RingBuffer 					 _readBuff;
  Network::RingBuffer 					 _writeBuff;
  bool									 _connected;

private:
  std::shared_ptr<Network::ABasicSocket> _socket;
};

class IdentityClientHelper
{
public:
  IdentityClientHelper(const std::shared_ptr<Network::Identity>& id,
                       const std::weak_ptr<Network::AListenSocket>& listener);
  virtual ~IdentityClientHelper() = default;

  const std::shared_ptr<Network::Identity>& getId() const {return _id;};
  void setId(const std::shared_ptr<Network::Identity>& id);

protected:
  void readData(const Network::Buffer& data);
  void writeData();
  virtual void onRead() = 0;

protected:
  std::weak_ptr<Network::AListenSocket>  _listener;
  Network::RingBuffer 					 _readBuff;
  Network::RingBuffer 					 _writeBuff;

private:
  std::shared_ptr<Network::Identity>	 _id;

};

};

#endif // CLIENTHELPER_H
