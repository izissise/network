#include "ring_buffer.h"

void		write_buffer(t_rbuf *buf, const char *data, size_t size)
{
  size_t	born;
  char		*buffer;
  size_t	tmpsize;

  born = buf->size;
  buffer = buf->buf;
  tmpsize = (2 * born - buf->idx_w) % born;
  memcpy(&(buffer[buf->idx_w]), data, size < tmpsize ? size : tmpsize);
  if ((int)size - (int)tmpsize > 0)
    memcpy(buffer, &(data[tmpsize]), size - tmpsize);
  buf->idx_w += size;
  if (buf->idx_w >= buf->idx_r + born)
    buf->idx_r = buf->idx_w + 1;
  buf->idx_w %= born;
}

size_t		read_buffer(t_rbuf *buf, char *data, size_t size)
{
  size_t	born;
  size_t	left;
  char		*buffer;
  size_t	tmpsize;

  left = ring_buffer_left_read(buf);
  born = buf->size;
  buffer = buf->buf;
  size = size < left ? size : left;
  tmpsize = (2 * born - buf->idx_r) % born;
  memcpy(data, &(buffer[buf->idx_r]), size < tmpsize ? size : tmpsize);
  if ((int)size - (int)tmpsize > 0)
    memcpy(&(data[tmpsize]), buffer, size - tmpsize);
  buf->idx_r = (buf->idx_r + size) % born;
  return (size);
}

void	rollback_read_buffer(t_rbuf *buf, size_t bysize)
{
  buf->idx_r = (buf->idx_r - bysize + 2 * buf->size) % buf->size;
}
