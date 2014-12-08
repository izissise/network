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

bool ANetwork::dispatchUdpEvent(const Network::Identity& idCheck, const Network::Buffer& data)
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

void ANetwork::dispatchListenerReadEv(const std::shared_ptr<AListenSocket>& listener)
{
  if (listener->getSockType() == ASocket::SockType::TCP)
    listener->getAcceptorCallback()();
  else if (listener->getSockType() == ASocket::SockType::UDP)
    {
      Network::Buffer data;
      Identity identity(listener->recvFrom(data, _recvfSize));
      if (!dispatchUdpEvent(identity, data))
        {
          std::shared_ptr<Identity> nid(new Identity(identity));
          listener->getNewConnectionCallback()(nid, data);
        }
    }
}

};
