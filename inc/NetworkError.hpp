#ifndef NETWORKERROR_H
# define NETWORKERROR_H

# include <string>
# include <exception>
# include <stdexcept>

namespace Network {

class NetworkError : public std::runtime_error
{
	public:
		NetworkError(const std::string& err) throw();
		virtual ~NetworkError() throw() = default;
};

};

#endif // NETWORKERROR_H
