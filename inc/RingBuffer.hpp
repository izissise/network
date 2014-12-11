#ifndef RINGBUFFER_H
# define RINGBUFFER_H

# include <cstdint>
# include <memory>
# include <cstring>

# include "ASocket.hpp"

namespace Network {

class RingBuffer
{
public:
  RingBuffer(size_t size = 4096);
  virtual ~RingBuffer() = default;

  void writeBuffer(const Network::Buffer& data);
  void readBuffer(Network::Buffer& data, size_t rSize);

  size_t getBuffSize() const {return _buffSize;};

  void extendRingBuffer(size_t addSize);
  void rollbackReadBuffer(size_t rbsize);

  inline size_t getLeftRead() const {
    return (((_idxW + _buffSize) - _idxR) % _buffSize);
  };
  inline size_t getLeftWrite() const {
    return (((_idxR + _buffSize - 1) - _idxW) % _buffSize);
  };

  //Iterators

protected:
  size_t					_buffSize;
  size_t					_idxR;
  size_t					_idxW;
  std::unique_ptr<uint8_t>	_buffer;
};

};

#endif // RINGBUFFER_H
