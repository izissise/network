#include "UnixNetwork.hpp"

#include <sys/epoll.h>

#include "Error.hpp"

namespace Network {
namespace Unix {

UnixNetwork::UnixNetwork()
{
  _pollFd = epoll_create1(0);
  if (_pollFd == -1)
    throw Network::Error("epoll" + strerror(errno));
}

UnixNetwork::~UnixNetwork()
{
}

void UnixNetwork::poll()
{

}

};
};
