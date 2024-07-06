#pragma once
#include <string>

namespace dnscpp {

class SQL {
public:
  SQL();
  ~SQL();
  std::string version();
};

} // namespace dnscpp
