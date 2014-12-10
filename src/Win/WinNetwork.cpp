#include "Win/WinNetwork.hpp"

#include <exception>
#include <stdexcept>

#include "Error.hpp"
#include "Win/WinNetworkBasicSocket.hpp"
#include "Win/WinNetworkListenSocket.hpp"

namespace Network {
namespace Win {

WinNetwork::WinNetwork(size_t recvFromSize)
  : ANetwork(recvFromSize)
{
  _maxFd = 0;
  // Initialize Winsock
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0)
    throw Network::Error("WSAStartup failed");

}

WinNetwork::~WinNetwork()
{
  WSACleanup();
}

void WinNetwork::setFdSet()
{
  _maxFd = 0;
  FD_ZERO(&_setr);
  FD_ZERO(&_setw);

  auto setupdate = [this](int fd, Network::ASocket::Event req) {
    bool monitor = false;
    if ((req == ASocket::Event::READ) || (req == ASocket::Event::RDWR))
      {
        FD_SET(fd, &_setr);
        monitor = true;
      }
    if ((req == ASocket::Event::WRITE) || (req == ASocket::Event::RDWR))
      {
        FD_SET(fd, &_setw);
        monitor = true;
      }
    if (monitor && fd > _maxFd)
      _maxFd = fd;
  };

  _listener.erase(std::remove_if(_listener.begin(), _listener.end(),
  [&setupdate](std::weak_ptr<AListenSocket>& li) -> bool {
    std::shared_ptr<ListenSocket> sock = std::dynamic_pointer_cast<ListenSocket>(li.lock());
    if (!sock)
      return true;
    Network::ASocket::Event req = sock->getEventRequest();
    setupdate(sock->getSockFd(), req);
    return false;
  }), _listener.end());

  _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
  [&setupdate](std::weak_ptr<ABasicSocket>& cl) -> bool {
    std::shared_ptr<BasicSocket> sock = std::dynamic_pointer_cast<BasicSocket>(cl.lock());
    if (!sock)
      return true;
    Network::ASocket::Event req = sock->getEventRequest();
    setupdate(sock->getSockFd(), req);
    return false;
  }), _clients.end());

}

void WinNetwork::pollFdsets()
{
  for (auto& li : _listener)
    {
      std::shared_ptr<ListenSocket> sock = std::dynamic_pointer_cast<ListenSocket>(li.lock());
      if (sock)
        {
          if (FD_ISSET(sock->getSockFd(), &_setr))
            dispatchListenerReadEv(li.lock());
        }
    }

  for (auto& cli : _clients)
    {
      std::shared_ptr<BasicSocket> sock = std::dynamic_pointer_cast<BasicSocket>(cli.lock());
      if (sock)
        {
          if (FD_ISSET(sock->getSockFd(), &_setr))
            sock->getReadeableCallback()();
          else if (FD_ISSET(sock->getSockFd(), &_setw))
            sock->getWritableCallback()();
        }
    }
}

void WinNetwork::poll(bool block)
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 1;

  setFdSet();
  if ((select(_maxFd + 1, &_setr, &_setw, nullptr, block ? nullptr : &tv) == -1))
    throw Network::Error(std::string("select") + strerror(errno));
  pollFdsets();
}

};
};
