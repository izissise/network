#ifndef RING_BUFFER_H_INCLUDED
# define RING_BUFFER_H_INCLUDED

# ifdef __cplusplus
extern "C" {
# endif

# include <stdlib.h>
# include <string.h>

typedef struct	s_rbuf
{
  char		*buf;
  size_t		size;
  size_t		idx_r;
  size_t		idx_w;
}		t_rbuf;

t_rbuf	*create_ring_buffer(size_t size);
void		destroy_ring_buffer(void *buf);
void		extend_ring_buffer(t_rbuf *buf, size_t addsize);

size_t	ring_buffer_left_read(t_rbuf *buf);
size_t	ring_buffer_left_write(t_rbuf *buf);

void		write_buffer(t_rbuf *buf, const char *data, size_t size);
size_t		read_buffer(t_rbuf *buf, char *data, size_t size);
void		rollback_read_buffer(t_rbuf *buf, size_t bysize);

# ifdef __cplusplus
} // closing brace for extern "C"
# endif

#endif /* !RING_BUFFER_H_INCLUDED */
