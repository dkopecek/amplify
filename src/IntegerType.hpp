#ifndef AMPUSH_INTEGERTYPE_HPP
#define AMPUSH_INTEGERTYPE_HPP

#include <build-config.h>
#include "Type.hpp"

namespace Amplify
{
  class Interpreter;
  class Program;

  class IntegerType : public Type
  {
  public:
    static const uint8_t type_number = 2;
    static const std::string type_string;

    typedef int32_t Value;

    IntegerType();
    void interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const;
    void allocateStack(Interpreter& interpreter) const;
    void allocateStorage(Program& program) const;
    void registerInstructions(Interpreter& interpreter) const;
    size_t printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const;
    size_t randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const;
  };
} /* namespace Amplify */

#endif /* AMPUSH_INTEGERTYPE_HPP */
