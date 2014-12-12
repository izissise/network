#ifndef WINNETWORK_H
# define WINNETWORK_H

# include <atomic>
# include <memory>

# include "winsock2.h"

# include "ANetwork.hpp"

namespace Network {
namespace Win {

class WinNetwork : public ANetwork
{
private:
	static std::atomic<bool>   _init;
	static std::atomic<size_t> _nbInstance;

public:
  WinNetwork(size_t recvFromSize = 10);
  virtual ~WinNetwork();

  void poll(bool block) override;

protected:
	void setFdSet();
	void pollFdsets();

protected:
  bool		_bool;
  fd_set	_setr;
  fd_set	_setw;
  int		_maxFd;
};
};
};

#endif // WINNETWORK_H
