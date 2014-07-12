#include "select.h"

static inline int	max_fd_plusone(t_list *fds)
{
  int			max;
  t_list		*tmp;
  t_selfd		*fd;

  max = -1;
  tmp = fds;
  while (tmp)
    {
      fd = (t_selfd*)tmp->data;
      max = fd->fd > max ? fd->fd : max;
      tmp = tmp->next;
    }
  return (max + 1);
}

static inline void	set_fdset(t_list *fds, fd_set *setr, fd_set *setw)
{
  t_list		*tmp;
  t_selfd		*fd;

  FD_ZERO(setr);
  FD_ZERO(setw);
  tmp = fds;
  while (tmp)
    {
      fd = (t_selfd*)tmp->data;
      if ((fd->checktype & FDREAD) == FDREAD)
        FD_SET(fd->fd, setr);
      if ((fd->checktype & FDWRITE) == FDWRITE)
        FD_SET(fd->fd, setw);
      tmp = tmp->next;
    }
}

t_selfd		*create_fd(int fd, void *data, int (*call)())
{
  t_selfd	*res;

  if ((res = malloc(1 * sizeof(t_selfd))) == NULL)
    return (NULL);
  res->fd = fd;
  res->etype = 0;
  res->checktype = FDREAD;
  res->data = data;
  res->callback = call;
  res->to_close = 0;
  res->rbuff = create_ring_buffer(BUFSIZ * 2);
  res->wbuff = create_ring_buffer(BUFSIZ * 2);
  if (!res->rbuff || !res->wbuff)
    {
      destroy_ring_buffer(res->rbuff);
      destroy_ring_buffer(res->wbuff);
      free(res);
      return (NULL);
    }
  return (res);
}

void		destroy_fd(void *fd)
{
  t_selfd	*ptr;

  ptr = (t_selfd*)fd;
  if (ptr)
    {
      destroy_ring_buffer(ptr->rbuff);
      destroy_ring_buffer(ptr->wbuff);
      free(ptr);
    }
}

void		do_select(t_list *fds, struct timeval *tv, void *global_arg)
{
  fd_set	setr;
  fd_set	setw;
  t_list	*tmp;
  t_list	*nexttmp;
  t_selfd	*fd;

  nexttmp = NULL;
  set_fdset(fds, &setr, &setw);
  if ((select(max_fd_plusone(fds), &setr, &setw, NULL, tv) == -1))
    return ;
  tmp = fds;
  nexttmp = tmp->next;
  while (tmp)
    {
      fd = (t_selfd*)tmp->data;
      fd->etype = (FD_ISSET(fd->fd, &setr) ? FDREAD : 0)
                  + (FD_ISSET(fd->fd, &setw) ? FDWRITE : 0);
      fd->checktype = 0;
      fd->callback(fd, global_arg);
      tmp = nexttmp;
      nexttmp = tmp ? tmp->next : NULL;
    }
}
