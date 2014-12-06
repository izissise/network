#include "Unix/UnixNetwork.hpp"

#include <exception>
#include <stdexcept>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "Error.hpp"
#include "Unix/UnixNetworkBasicSocket.hpp"
#include "Unix/UnixNetworkListenSocket.hpp"

namespace Network {
namespace Unix {

UnixNetwork::UnixNetwork(size_t maxEvents)
  : _maxEvents(maxEvents)
{
  _events = new struct epoll_event[_maxEvents];
  _pollFd = epoll_create1(0);
  if (_pollFd == -1)
    throw Network::Error(std::string("epoll") + strerror(errno));
}

UnixNetwork::~UnixNetwork()
{
  close(_pollFd);
}

void UnixNetwork::registerClient(const std::weak_ptr<Network::ABasicSocket>& cli)
{
  std::shared_ptr<BasicSocket> uBasicSock = std::dynamic_pointer_cast<BasicSocket>(cli.lock());
  struct epoll_event ev;

  if (!uBasicSock)
    throw std::runtime_error("UnixNetwork::registerClient");

  std::memset(&ev, 0, sizeof(decltype(ev)));
  int ret = epoll_ctl(_pollFd, EPOLL_CTL_ADD, uBasicSock->getSockFd(), &ev);
  if (ret == -1)
    throw Network::Error(std::string("epoll_ctl") + strerror(errno));
  ANetwork::registerClient(cli);
}

void UnixNetwork::registerListener(const std::weak_ptr<Network::AListenSocket>& listener)
{
  std::shared_ptr<ListenSocket> uListSock = std::dynamic_pointer_cast<ListenSocket>(listener.lock());
  struct epoll_event ev;

  if (!uListSock)
    throw std::runtime_error("UnixNetwork::registerListener");

  std::memset(&ev, 0, sizeof(decltype(ev)));
  ev.events = EPOLLIN;
  int ret = epoll_ctl(_pollFd, EPOLL_CTL_ADD, uListSock->getSockFd(), &ev);
  if (ret == -1)
    throw Network::Error(std::string("epoll_ctl") + strerror(errno));
  ANetwork::registerListener(listener);
}

void UnixNetwork::poll(bool block)
{
//check there and update epoll_event
  int ret = epoll_wait(_pollFd, _events, _maxEvents, block ? -1 : 0);
  if (ret == -1)
    throw std::runtime_error("epoll_wait");
  for (int i = 0; i < ret; ++i)
    {
    //  _events[i].events
    }
}

};
};
