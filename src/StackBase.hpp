#ifndef AMPUSH_STACKBASE_HPP
#define AMPUSH_STACKBASE_HPP

#include <build-config.h>
#include "CodeType.hpp"
#include <cinttypes>
#include <ostream>

namespace Amplify
{
  class Program;
  class Interpreter;

  class StackBase
  {
  public:
    StackBase(uint8_t type_number) : _type_number(type_number) {}
    virtual ~StackBase() {}

    uint8_t getTypeNumber() const
    {
      return _type_number;
    }

    virtual void print(std::ostream& stream) const = 0;
    virtual void print(const Interpreter& interpreter, const Program& program, std::ostream& stream) const = 0;

  private:
    uint8_t _type_number;
  };

} /* namespace Amplify */

#endif /* AMPUSH_STACKBASE_HPP */
