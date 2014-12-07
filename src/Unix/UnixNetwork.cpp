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

UnixNetwork::UnixNetwork(size_t recvFromSize, size_t maxEvents)
  : ANetwork::ANetwork(recvFromSize), _maxEvents(maxEvents)
{
  _events = new struct epoll_event[_maxEvents];
  _pollFd = epoll_create1(0);
  if (_pollFd == -1)
    throw Network::Error(std::string("epoll") + strerror(errno));
}

UnixNetwork::~UnixNetwork()
{
  delete[] _events;
  close(_pollFd);
}

void UnixNetwork::registerClient(const std::weak_ptr<Network::ABasicSocket>& cli)
{
  std::shared_ptr<BasicSocket> uBasicSock = std::dynamic_pointer_cast<BasicSocket>(cli.lock());
  struct epoll_event ev;

  if (!uBasicSock)
    throw std::runtime_error("UnixNetwork::registerClient");

  std::memset(&ev, 0, sizeof(decltype(ev)));
  ev.data.ptr = (cli.lock()).get();
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
  ev.data.ptr = (listener.lock()).get();
  int ret = epoll_ctl(_pollFd, EPOLL_CTL_ADD, uListSock->getSockFd(), &ev);
  if (ret == -1)
    throw Network::Error(std::string("epoll_ctl") + strerror(errno));
  ANetwork::registerListener(listener);
}
void UnixNetwork::updateRequest()
{
  auto epupdate = [this](int fd, Network::ASocket::Event req, void* data) {
    struct epoll_event ev;
    std::memset(&ev, 0, sizeof(decltype(ev)));
    ev.data.ptr = data;
    ev.events = ((req == ASocket::Event::READ) ? EPOLLIN :
                 ((req == ASocket::Event::WRITE) ? EPOLLOUT :
                  ((req == ASocket::Event::WRITE) ? (EPOLLIN | EPOLLOUT) : 0)));
    epoll_ctl(_pollFd, EPOLL_CTL_MOD, fd, &ev);
  };

  _listener.erase(std::remove_if(_listener.begin(), _listener.end(),
  [&epupdate](std::weak_ptr<AListenSocket>& li) -> bool {
    std::shared_ptr<ListenSocket> sock = std::dynamic_pointer_cast<ListenSocket>(li.lock());
    if (!sock)
      return true;
    Network::ASocket::Event req = sock->getEventRequest();
    epupdate(sock->getSockFd(), req, (li.lock()).get());
    return false;
  }), _listener.end());


  _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
  [&epupdate](std::weak_ptr<ABasicSocket>& cl) -> bool {
    std::shared_ptr<BasicSocket> sock = std::dynamic_pointer_cast<BasicSocket>(cl.lock());
    if (!sock)
      return true;
    Network::ASocket::Event req = sock->getEventRequest();
    epupdate(sock->getSockFd(), req, (cl.lock()).get());
    return false;
  }), _clients.end());
}

bool UnixNetwork::dispatchUdpEvent(const Network::Identity& idCheck, const Network::Buffer& data)
{
  bool handled = false;

  _identities.erase(std::remove_if(_identities.begin(), _identities.end(),
  [&idCheck, &handled, &data](std::weak_ptr<Network::Identity>& cl) -> bool {
    std::shared_ptr<Network::Identity> id = cl.lock();
    if (!id)
      return true;
    if ((*id) == idCheck)
      {
        handled = true;
        id->onRead(data);
      }
    return false;
  }), _identities.end());

  return handled;
}

void UnixNetwork::dispatchEvent(struct epoll_event* ev)
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
            {
              if (sock->getSockType() == ASocket::SockType::TCP)
                sock->getAcceptorCallback()();
              else if (sock->getSockType() == ASocket::SockType::UDP)
                {
                  Network::Buffer data;
                  Identity identity(sock->recvFrom(data, _recvfSize));
                  if (!dispatchUdpEvent(identity, data))
                    {
                      std::shared_ptr<Identity> nid(new Identity(identity));
                      sock->getNewConnectionCallback()(nid, data);
                    }
                }
            }
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

void UnixNetwork::poll(bool block)
{
  updateRequest();
  int ret = epoll_wait(_pollFd, _events, _maxEvents, block ? -1 : 0);
  if (ret == -1)
    throw std::runtime_error("epoll_wait");
  for (int i = 0; i < ret; ++i)
    dispatchEvent(&(_events[i]));
}

};
};
