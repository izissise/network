#include "network.h"

const char	*listening(int domain)
{
  if (domain == AF_INET)
    return ("0.0.0.0");
  else if (domain == AF_INET6)
    return ("::1");
  return (NULL);
}

t_net	*peer(t_net *net)
{
  t_net	*res;
  int	ret;

  if ((res = malloc(1 * sizeof(t_net))) == NULL)
    return (NULL);
  res->socket = -1;
  res->addrlen = sizeof(struct sockaddr_storage);
  ret = getpeername(net->socket, (struct sockaddr*)(&(res->addr)),
                    &(res->addrlen));
  if (ret)
    {
      perror("getpeername");
      free(res);
      return (NULL);
    }
  return (res);
}

int			port_number(t_net *net)
{
  in_port_t		port;
  struct sockaddr	*addr;

  addr = (struct sockaddr*)(&(net->addr));
  port = 0;
  if (addr->sa_family == AF_INET)
    port = (((struct sockaddr_in*)addr)->sin_port);
  else if (addr->sa_family == AF_INET6)
    port = (((struct sockaddr_in6*)addr)->sin6_port);
  return ((port == 0) ? 0 : ntohs(port));
}

t_net	*accept_connection(t_net *sock)
{
  t_net	*res;

  if ((res = malloc(1 * sizeof(t_net))) == NULL)
    return (NULL);
  res->addrlen = sizeof(struct sockaddr_storage);
  res->socket = accept(sock->socket, (struct sockaddr*)(&(res->addr)),
                       &(res->addrlen));
  if (res->socket == -1)
    {
      perror("accept");
      free(res);
      return (NULL);
    }
  return (res);
}
