#ifndef IDENTITY_HPP_INCLUDED
# define IDENTITY_HPP_INCLUDED

# include <string>
# include <cstring>
# include <functional>

namespace Network {

struct Identity
{
  Identity(const std::string& Ip = "", uint16_t Port = 0)
    : ip(Ip), port(Port)
  {
  };

  Identity(const Identity&) = default;
  Identity& operator=(const Identity&) = default;

  ~Identity() = default;

  //bool operator==(const Identity&);

  std::string ip;
  uint16_t    port;


  std::function<void()> onReadeable;
  std::function<void()> onWritable;
};
};

#endif // IDENTITY_HPP_INCLUDED
