#include "ClientHelper.hpp"

namespace Network {

SocketClientHelper::SocketClientHelper(const std::shared_ptr<Network::ABasicSocket>& sock, size_t readSize)
  : _readSize(readSize), _connected(true), _socket(sock)
{
  _socket->setReadeableCallback(std::bind(&SocketClientHelper::onReadeable, this));
  _socket->setWritableCallback(std::bind(&SocketClientHelper::onWritable, this));
}

SocketClientHelper::SocketClientHelper(size_t readSize)
  : _readSize(readSize), _connected(false), _socket(nullptr)
{

}

void SocketClientHelper::setSocket(const std::shared_ptr<Network::ABasicSocket>& sock)
{
  _connected = true;
  _socket = sock;
  _socket->setReadeableCallback(std::bind(&SocketClientHelper::onReadeable, this));
  _socket->setWritableCallback(std::bind(&SocketClientHelper::onWritable, this));
}

void SocketClientHelper::onReadeable()
{
  Buffer buff;
  size_t size = _readBuff.getLeftWrite();

  if (_readSize > size)
    size = _readSize;
  try {
      _socket->read(buff, size);
      size = buff.size();
      _readBuff.writeBuffer(buff);
      onRead(size);
      if (_writeBuff.getLeftRead() > 0)
        _socket->setEventRequest(Network::ASocket::Event::RDWR);
      else
        _socket->setEventRequest(Network::ASocket::Event::READ);
      if (size == 0 && _socket->getSockType() == ASocket::SockType::TCP)
        _connected = false;
    }
  catch (Network::Error& e)
    {
      _connected = false;
    }
  if (!_connected)
    {
      _socket->setEventRequest(Network::ASocket::Event::NONE);
      _socket->closeSocket();
    }
}

void SocketClientHelper::onWritable()
{
  size_t size = _writeBuff.getLeftRead();
  size_t sizeWrite;
  Buffer buff;

  _writeBuff.readBuffer(buff, size);
  try {
      sizeWrite = _socket->write(buff);
      _writeBuff.rollbackReadBuffer(size - sizeWrite);
      onWrite(size);
      if (_writeBuff.getLeftRead() > 0)
        _socket->setEventRequest(Network::ASocket::Event::RDWR);
      else
        _socket->setEventRequest(Network::ASocket::Event::READ);
    }
  catch (Network::Error& e)
    {
      _connected = false;
      _writeBuff.rollbackReadBuffer(size);
    }
  if (!_connected)
    {
      _socket->setEventRequest(Network::ASocket::Event::NONE);
      _socket->closeSocket();
    }
}

IdentityClientHelper::IdentityClientHelper(const std::shared_ptr<Network::Identity>& id,
    const std::weak_ptr<Network::AListenSocket>& listener)
  : _listener(listener), _id(id)
{
  _id->onRead = std::bind(&IdentityClientHelper::readData, this, std::placeholders::_1);
}

void IdentityClientHelper::setId(const std::shared_ptr<Network::Identity>& id)
{
  _id = id;
  _id->onRead = std::bind(&IdentityClientHelper::readData, this, std::placeholders::_1);
}

void IdentityClientHelper::readData(const Network::Buffer& data)
{
  _readBuff.writeBuffer(data);
  onRead();
  writeData();
}

void IdentityClientHelper::writeData()
{
  size_t size = _writeBuff.getLeftRead();
  size_t sizeWrite;
  Buffer buff;
  std::shared_ptr<Network::AListenSocket> listener = _listener.lock();

  if (!listener)
    throw std::runtime_error("Can't lock listener.");
  _writeBuff.readBuffer(buff, size);
  try {
      sizeWrite = listener->sendTo(*(_id.get()), buff);
      _writeBuff.rollbackReadBuffer(size - sizeWrite);
    }
  catch (Network::Error& e)
    {
      _writeBuff.rollbackReadBuffer(size);
    }
}

};
