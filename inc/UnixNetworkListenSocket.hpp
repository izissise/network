#ifndef UNIXNETWORKLISTENSOCKET_HPP
# define UNIXNETWORKLISTENSOCKET_HPP

# include <string>
# include <cstdint>

# include "UnixSocket.hpp"
# include "INetworkListenSocket.hpp"

namespace Network {
namespace UnixNetwork {

class UnixNetworkListenSocket : public UnixSocket, public virtual INetworkListenSocket
{
	public:
		UnixNetworkListenSocket(const std::string& listeningIp, const std::string& port,
								INetworkSocket::SockType socktype = INetworkSocket::SockType::TCP,
								bool reuse = true);
		virtual ~UnixNetworkListenSocket() = default;

		void closeSocket() override {UnixSocket::closeSocket();};

		std::unique_ptr<INetworkBasicSocket> acceptClient() override;

		const std::string& getListeningIpAddr() const override {return _ip;};
		uint16_t getListeningPort() const override {return _port;};
		INetworkSocket::SockType getSockType() const override {return UnixSocket::getSockType();};

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
