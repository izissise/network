#ifndef NETWORKERROR_H
# define NETWORKERROR_H

# include <string>
# include <exception>
# include <stdexcept>

namespace Network {

class Error : public std::runtime_error
{
	public:
		Error(const std::string& err) throw();
		virtual ~Error() throw() {};
};

};

#endif // NETWORKERROR_H
