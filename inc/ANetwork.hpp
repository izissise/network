#ifndef ANETWORK_H
# define ANETWORK_H

# include <memory>
# include <map>
# include <deque>
# include <algorithm>

# include "Identity.hpp"
# include "AListenSocket.hpp"

namespace Network {

class ANetwork
{
public:
  ANetwork() = default;
  virtual ~ANetwork() = default;

  //UDP clients
  virtual void registerIdentity(const std::weak_ptr<Network::Identity>& id);

  //TCP clients
  virtual void registerClient(const std::weak_ptr<Network::ABasicSocket>& cli);

  //Listening sockets
  virtual void registerListener(const std::weak_ptr<Network::AListenSocket>& listener);

  virtual void poll(bool block = false) = 0;

protected:
  void dispatchListenerReadEv(const std::shared_ptr<AListenSocket>& listener);
  bool dispatchUdpEvent(const Network::Identity& idCheck, const Network::Buffer& data);

protected:
  std::deque<std::weak_ptr<Network::AListenSocket>> _listener;
  std::deque<std::weak_ptr<Network::Identity>> _identities;
  std::deque<std::weak_ptr<Network::ABasicSocket>> _clients;
};
};

#endif // ANETWORK_H
