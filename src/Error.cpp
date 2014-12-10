#include "Error.hpp"

namespace Network {
Error::Error(const std::string& err) throw()
 : runtime_error(err)
{
}

};
