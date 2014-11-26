#include "Error.hpp"

namespace Network {
Error::Error(const std::string& err) throw()
 : std::runtime_error::runtime_error(err)
{
}

};
