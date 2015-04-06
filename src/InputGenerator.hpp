#ifndef AMPUSH_INPUTGENERATOR_HPP
#define AMPUSH_INPUTGENERATOR_HPP

#include <build-config.h>
#include <cstdint>

namespace Amplify
{
  class InputGenerator
  {
  public:
    InputGenerator();
    virtual void generate(int64_t i) = 0;
  private:
  };
} /* namespace Amplify */

#endif /* AMPUSH_INPUTGENERATOR_HPP */
