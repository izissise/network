#include "RingBuffer.hpp"

namespace Network {

RingBuffer::RingBuffer(size_t size)
  : _buffSize(size), _idxR(0), _idxW(0),
    _buffer(new uint8_t[_buffSize])
{
}

void RingBuffer::writeBuffer(const Network::Buffer& data)
{
  uint8_t	*buffer = _buffer.get();
  size_t    size = data.size();
  size_t	tmpsize;

  tmpsize = (2 * _buffSize - _idxW) % _buffSize;
  std::memcpy(&(buffer[_idxW]), data.data(), size < tmpsize ? size : tmpsize);
  if (size - tmpsize > 0)
    std::memcpy(buffer, &((data.data())[tmpsize]), size - tmpsize);
  _idxW += size;
  if (_idxW >= _idxR + _buffSize)
    _idxR = _idxW + 1;
  _idxW %= _buffSize;
}

void RingBuffer::readBuffer(Network::Buffer& data, size_t size)
{
  uint8_t	*buffer = _buffer.get();
  size_t	left;
  size_t	tmpsize;

  left = getLeftRead();
  size = size < left ? size : left;
  tmpsize = (2 * _buffSize - _idxR) % _buffSize;
  data.resize(size);
  std::memcpy(&(*(data.begin())), &(buffer[_idxR]), size < tmpsize ? size : tmpsize);
  if (size - tmpsize > 0)
    std::memcpy(&((&(*(data.begin())))[tmpsize]), buffer, size - tmpsize);
  _idxR = (_idxR + size) % _buffSize;
}

void RingBuffer::extendRingBuffer(size_t addSize)
{
  size_t	fsize = _buffSize + addSize;
  std::unique_ptr<uint8_t> tmp(new uint8_t[fsize]);

  std::memcpy(tmp.get(), _buffer.get(), _buffSize);
  std::swap(_buffer, tmp);
  if (_idxR > _idxW)
    {
      Network::Buffer tmpBuff;
      addSize = getLeftRead();
      readBuffer(tmpBuff, addSize);
      _buffSize = fsize;
      _idxR = fsize;
      _idxW = 0;
      writeBuffer(tmpBuff);
      _idxR = 0;
    }
  _buffSize = fsize;
}

void RingBuffer::rollbackReadBuffer(size_t rbsize)
{
  _idxR = (_idxR - rbsize + 2 * _buffSize) % _buffSize;
}

};
