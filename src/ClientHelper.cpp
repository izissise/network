#include "ClientHelper.hpp"

namespace Network {

ClientHelper::ClientHelper(const std::shared_ptr<Network::ABasicSocket>& sock)
  : _socket(sock)
{
  _socket->setReadeableCallback(std::bind(&ClientHelper::onReadeable, this));
  _socket->setWritableCallback(std::bind(&ClientHelper::onWritable, this));
}

};
