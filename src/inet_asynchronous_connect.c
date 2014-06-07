#include "network.h"

int	set_fd_flags(int fd, int flag, int unset)
{
  int	flags;

  if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
    return (-1);
  flags = unset ? (flags & ~flag) : (flags | flag);
  if (fcntl(fd, F_SETFL, flags) == -1)
    return (-1);
  return (0);
}

/*
** Connect in a non blocking way, you should not use the socket until
** is_connected returned successfully.
*/

int	connect_nb(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int	ret;

  if (((ret = set_fd_flags(sockfd, O_NONBLOCK, 0)) == -1)
      || (((ret = connect(sockfd, addr, addrlen)) == -1)
          && (errno != EINPROGRESS))
      || ((ret = set_fd_flags(sockfd, O_NONBLOCK, 1)) == -1))
    {
      perror("fcntl");
      return (ret);
    }
  return (0);
}

/*
** Check if error on socket which were in progress
** it should be called without being the corresponding fd being 'select'
** return -1 for error or 0 otherwise
*/

int		check_socket_error(t_net *net)
{
  int		ret;
  socklen_t	len;

  if (!net)
    {
      errno = EFAULT;
      return (-1);
    }
  len = sizeof(int);
  if ((getsockopt(net->socket, SOL_SOCKET, SO_ERROR, &ret, &len) == -1)
      || (ret > 0) || getsockname(net->socket,
                                  (struct sockaddr*)(&(net->addr)),
                                  &(net->addrlen)) == -1)
    {
      errno = (ret > 0) ? ret : errno;
      return (-1);
    }
  return (0);
}

/*
** Check if the socket is connected return quickly
** return 0 when connection as been established
** return 1 when it still waiting
** return -1 in case of error
*/

int			is_connected(t_net *net)
{
  int			ret;
  fd_set		fdset;
  struct timeval	tv;

  if (!net)
    {
      errno = EFAULT;
      return (-1);
    }
  tv.tv_sec = 0;
  tv.tv_usec = 20;
  FD_ZERO(&fdset);
  FD_SET(net->socket, &fdset);
  if ((ret = (select(net->socket + 1, NULL, &fdset, NULL, &tv)) <= 0))
    return ((ret == -1) ? -1 : 1);
  return (check_socket_error(net));
}
