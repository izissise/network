#include "Unix/LinuxNetwork.hpp"

#include <exception>
#include <stdexcept>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <csignal>

#include "Error.hpp"
#include "Unix/UnixNetworkBasicSocket.hpp"
#include "Unix/UnixNetworkListenSocket.hpp"

namespace Network {
namespace Unix {

std::atomic<bool> LinuxNetwork::_init(true);

LinuxNetwork::LinuxNetwork(size_t recvFromSize, size_t maxEvents)
  : ANetwork::ANetwork(recvFromSize), _maxEvents(maxEvents)
{
  if (_init.exchange(false))
    signal(SIGPIPE, SIG_IGN);

  _events = new struct epoll_event[_maxEvents];
  _pollFd = epoll_create1(0);
  if (_pollFd == -1)
    throw Network::Error(std::string("epoll") + strerror(errno));
}

LinuxNetwork::~LinuxNetwork()
{
  delete[] _events;
  close(_pollFd);
}

void LinuxNetwork::registerClient(const std::weak_ptr<Network::ABasicSocket>& cli)
{
  std::shared_ptr<BasicSocket> uBasicSock = std::dynamic_pointer_cast<BasicSocket>(cli.lock());
  struct epoll_event ev;

  if (!uBasicSock)
    throw std::runtime_error("LinuxNetwork::registerClient");

  std::memset(&ev, 0, sizeof(decltype(ev)));
  ev.data.ptr = (cli.lock()).get();
  int ret = epoll_ctl(_pollFd, EPOLL_CTL_ADD, uBasicSock->getSockFd(), &ev);
  if (ret == -1)
    throw Network::Error(std::string("epoll_ctl") + strerror(errno));
  ANetwork::registerClient(cli);
}

void LinuxNetwork::registerListener(const std::weak_ptr<Network::AListenSocket>& listener)
{
  std::shared_ptr<ListenSocket> uListSock = std::dynamic_pointer_cast<ListenSocket>(listener.lock());
  struct epoll_event ev;

  if (!uListSock)
    throw std::runtime_error("LinuxNetwork::registerListener");

  std::memset(&ev, 0, sizeof(decltype(ev)));
  ev.events = EPOLLIN;
  ev.data.ptr = (listener.lock()).get();
  int ret = epoll_ctl(_pollFd, EPOLL_CTL_ADD, uListSock->getSockFd(), &ev);
  if (ret == -1)
    throw Network::Error(std::string("epoll_ctl") + strerror(errno));
  ANetwork::registerListener(listener);
}
void LinuxNetwork::updateRequest()
{
  auto epupdate = [this](int fd, Network::ASocket::Event req, void* data) {
    struct epoll_event ev;
    std::memset(&ev, 0, sizeof(decltype(ev)));
    ev.data.ptr = data;
    ev.events = 0;
    if ((req == ASocket::Event::READ) || (req == ASocket::Event::RDWR))
      ev.events |= EPOLLIN;
    if ((req == ASocket::Event::WRITE) || (req == ASocket::Event::RDWR))
      ev.events |= EPOLLOUT;
    epoll_ctl(_pollFd, EPOLL_CTL_MOD, fd, &ev);
  };

  _listener.erase(std::remove_if(_listener.begin(), _listener.end(),
  [&epupdate](std::weak_ptr<AListenSocket>& li) -> bool {
    std::shared_ptr<ListenSocket> sock = std::dynamic_pointer_cast<ListenSocket>(li.lock());
    if (!sock || sock->getSockFd() == -1)
      return true;
    Network::ASocket::Event req = sock->getEventRequest();
    epupdate(sock->getSockFd(), req, (li.lock()).get());
    return false;
  }), _listener.end());


  _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
  [&epupdate](std::weak_ptr<ABasicSocket>& cl) -> bool {
    std::shared_ptr<BasicSocket> sock = std::dynamic_pointer_cast<BasicSocket>(cl.lock());
    if (!sock || sock->getSockFd() == -1)
      return true;
    sock->getPollUpdateCallback()();
    Network::ASocket::Event req = sock->getEventRequest();
    epupdate(sock->getSockFd(), req, (cl.lock()).get());
    return false;
  }), _clients.end());
}

void LinuxNetwork::dispatchEvent(struct epoll_event* ev)
{
  void* ptr = ev->data.ptr;
  bool handled = false;

  for (auto& li : _listener)
    {
      std::shared_ptr<AListenSocket> sock(li.lock());
      if (sock && (sock.get() == ptr))
        {
          handled = true;
          if (ev->events & EPOLLIN)
            dispatchListenerReadEv(li.lock());
        }
    }

  if (handled)
    return;

  for (auto& cli : _clients)
    {
      std::shared_ptr<ABasicSocket> sock(cli.lock());
      if (sock && (sock.get() == ptr))
        {
          handled = true;
          if (ev->events & EPOLLIN)
            sock->getReadeableCallback()();
          else if (ev->events & EPOLLOUT)
            sock->getWritableCallback()();
        }
    }
}

void LinuxNetwork::poll(bool block)
{
  updateRequest();
  if (_listener.size() + _clients.size() == 0)
    return;
  int ret = epoll_wait(_pollFd, _events, _maxEvents, block ? -1 : 0);
  if (ret == -1 && errno != EINTR)
    throw std::runtime_error("epoll_wait");
  for (int i = 0; i < ret; ++i)
    dispatchEvent(&(_events[i]));
}

};
};
