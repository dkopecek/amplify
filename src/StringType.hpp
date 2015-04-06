#ifndef AMPUSH_STRINGTYPE_HPP
#define AMPUSH_STRINGTYPE_HPP

#include <build-config.h>
#include "Type.hpp"
#include <memory>
#include <cstring>
#include <cstdlib>

namespace Amplify
{
  class Interpreter;
  class Program;

  class StringType : public Type
  {
  public:
    static const uint8_t type_number = 6;
    static const std::string type_string;

    struct Value {
      uint32_t size;
      uint8_t bytes[0];

      std::unique_ptr<Value> copy() const
      {
	Value* value_copy = reinterpret_cast<Value*>(malloc(sizeof(Value) + size));

	value_copy->size = size;
	memcpy(value_copy->bytes, bytes, size);

	return std::move(std::unique_ptr<Value>(value_copy));
      }
    } __attribute__((packed));

    StringType();
    void interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const;
    void allocateStack(Interpreter& interpreter) const;
    void allocateStorage(Program& program) const;
    size_t printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const;
    size_t randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const;
  };
} /* namespace Amplify */

#endif /* AMPUSH_STRINGTYPE_HPP */
