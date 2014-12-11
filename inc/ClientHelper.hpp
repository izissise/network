#ifndef CLIENTHELPER_H
# define CLIENTHELPER_H

#include "RingBuffer.hpp"
#include "ABasicSocket.hpp"

namespace Network {

class ClientHelper
{
public:
  ClientHelper(const std::shared_ptr<Network::ABasicSocket>& sock);
  virtual ~ClientHelper() = default;

protected:
  virtual void onReadeable() = 0;
  virtual void onWritable() = 0;

protected:
  std::shared_ptr<Network::ABasicSocket> _socket;
  Network::RingBuffer 					 _readBuff;
  Network::RingBuffer 					 _writeBuff;
};

};

#endif // CLIENTHELPER_H
