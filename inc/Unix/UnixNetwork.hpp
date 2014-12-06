#ifndef UNIXNETWORK_H
# define UNIXNETWORK_H

# include "ANetwork.hpp"

namespace Network {
namespace Unix {

class Network
{
public:
  UnixNetwork();
  virtual ~UnixNetwork();

  void poll() override;
protected:
  int _pollFd;
};
};
};

#endif // UNIXNETWORK_H
