#ifndef AMPUSH_TRANSFORMATION_HPP
#define AMPUSH_TRANSFORMATION_HPP

#include <build-config.h>
#include "Program.hpp"

#include <string>
#include <memory>

namespace Amplify
{

  class Transformation
  {
  public:
    Transformation(const std::string& name)
    {
      _name = name;
    }

  private:
    const std::string _name;
  };

} /* namespace Amplify */

#endif /* AMPUSH_TRANSFORMATION_HPP */
