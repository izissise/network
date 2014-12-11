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
  SocketClientHelper(const std::shared_ptr<Network::ABasicSocket>& sock, size_t readSize = 10);
  virtual ~SocketClientHelper() = default;

  bool getConnected() const {return _connected;};

private:
  void onReadeable();
  void onWritable();

protected:
  virtual void onRead(size_t sizeRead) = 0;
  virtual void onWrite(size_t sizeWrite) = 0;

protected:
  std::shared_ptr<Network::ABasicSocket> _socket;
  size_t								 _readSize;
  bool									 _connected;
  Network::RingBuffer 					 _readBuff;
  Network::RingBuffer 					 _writeBuff;
};

class IdentityClientHelper
{
public:
  IdentityClientHelper(const std::shared_ptr<Network::Identity>& id,
                  const std::weak_ptr<Network::AListenSocket>& listener);
  virtual ~IdentityClientHelper() = default;


protected:
  void readData(const Network::Buffer& data);
  void writeData();
  virtual void onRead() = 0;

protected:
  std::shared_ptr<Network::Identity>	 _id;
  std::weak_ptr<Network::AListenSocket>  _listener;
  Network::RingBuffer 					 _readBuff;
  Network::RingBuffer 					 _writeBuff;
};

};

#endif // CLIENTHELPER_H
