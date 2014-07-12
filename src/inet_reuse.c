#include "network.h"

/*
** Bind a socket to a port and reuse it.
*/

int	bind_reuse(int sockfd, const struct sockaddr *addr,
               socklen_t addrlen)
{
  int	yes;

  yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
      perror("setsockopt");
      return (-1);
    }
  return (bind(sockfd, addr, addrlen));
}
