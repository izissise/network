#ifndef SELECT_H_INCLUDED
# define SELECT_H_INCLUDED

# include <stdlib.h>
# include <stdio.h>
# include <sys/select.h>
# include <fcntl.h>
# include <errno.h>

# include "liste.h"
# include "ring_buffer.h"

# define FDREAD		1
# define FDWRITE	2
# define ISREADABLE(x)	(((x)->etype & FDREAD) == FDREAD)
# define ISWRITEABLE(x)	(((x)->etype & FDWRITE) == FDWRITE)
# define CHECKREAD(x)	((x)->checktype |= FDREAD)
# define CHECKWRITE(x)	((x)->checktype |= FDWRITE)

typedef struct	s_selfd
{
  int		fd;
  size_t	cli_num;
  char		to_close;
  int		etype;
  int		checktype;
  void		*data;
  int		(*callback)(struct s_selfd *_this, void *data);
  t_rbuf	*rbuff;
  t_rbuf	*wbuff;
}		t_selfd;

void	do_select(t_list *fds, struct timeval *tv, void *global_arg);
t_selfd	*create_fd(int fd, void *data, int (*call)());
void	destroy_fd(void *fd);

#endif /* !SELECT_H_INCLUDED */
