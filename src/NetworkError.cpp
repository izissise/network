#include "NetworkError.hpp"

NetworkError::NetworkError(const std::string& err) throw()
 : std::runtime_error::runtime_error(err)
{
}
