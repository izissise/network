#ifndef UNIXNETWORK_H
# define UNIXNETWORK_H

# include <sys/epoll.h>

# include "ANetwork.hpp"

namespace Network {
namespace Unix {

class UnixNetwork : public ANetwork
{
public:
  UnixNetwork(size_t recvFromSize = 10, size_t maxEvents = 50);
  virtual ~UnixNetwork();

  void registerClient(const std::weak_ptr<Network::ABasicSocket>& cli) override;
  void registerListener(const std::weak_ptr<Network::AListenSocket>& listener) override;

  void poll(bool block) override;

protected:
  void updateRequest();
  void dispatchEvent(struct epoll_event* ev);

protected:
  int _pollFd;
  size_t _maxEvents;
  struct epoll_event* _events;
};
};
};

#endif // UNIXNETWORK_H
