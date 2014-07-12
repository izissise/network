#ifndef NETWORK_H_INCLUDED
# define NETWORK_H_INCLUDED

# ifdef __cplusplus
extern "C" {
# endif

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <fcntl.h>

# define MAX_QUEUE 50

typedef struct			s_net
{
  struct sockaddr_storage	addr;
  socklen_t			addrlen;
  int				socket;
  int				socktype;
  struct s_net			*peer;
}				t_net;

t_net		*create_connection(const char *host, const char *port,
                           int socktype, int	(*f)(int sockfd,
                               const struct sockaddr *addr,
                               socklen_t addrlen));
void	close_connection(t_net *net);
t_net	*accept_connection(t_net *sock);
char		*get_ip_addr(t_net *net);
t_net	*peer(t_net *net);
const char	*listening(int domain);
int		port_number(t_net *net);

void	write_sock(const char *str, int socket, int strlen);

int	connect_nb(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int	check_socket_error(t_net *net);
int	is_connected(t_net *net);

int	bind_reuse(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

# ifdef __cplusplus
} // closing brace for extern "C"
# endif

#endif /* !NETWORK_H_INCLUDED */
