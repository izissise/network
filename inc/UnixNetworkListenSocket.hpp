#ifndef UNIXNETWORKLISTENSOCKET_HPP
# define UNIXNETWORKLISTENSOCKET_HPP

# include <string>
# include <cstdint>

# include "UnixSocket.hpp"
# include "IListenSocket.hpp"

namespace Network {
namespace UnixNetwork {

class ListenSocket : public Socket, virtual public IListenSocket
{
	public:
		ListenSocket(const std::string& listeningIp, const std::string& port,
								ISocket::SockType socktype = ISocket::SockType::TCP,
								bool reuse = true);
		virtual ~ListenSocket() = default;

		void closeSocket() override {Socket::closeSocket();};

		std::unique_ptr<IBasicSocket> acceptClient() override;

		const std::string& getListeningIpAddr() const override {return _ip;};
		uint16_t getListeningPort() const override {return _port;};
		ISocket::SockType getSockType() const override {return Socket::getSockType();};

	protected:
		static void bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
		static void bindReuse(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

		void updateInfo() override;

	protected:
		std::string _ip;
		uint16_t	_port;
};

};
};

#endif // UNIXNETWORKLISTENSOCKET_HPP
