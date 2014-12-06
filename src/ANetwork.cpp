#include "ANetwork.hpp"

namespace Network {

void ANetwork::registerIdentity(const std::weak_ptr<Network::Identity>& id)
{
  _identities.push_front(id);
}

void ANetwork::registerClient(const std::weak_ptr<Network::ABasicSocket>& cli)
{
  _clients.push_front(cli);
}

void ANetwork::registerListener(const std::weak_ptr<Network::AListenSocket>& listener)
{
  _listener.push_front(listener);
}

};
