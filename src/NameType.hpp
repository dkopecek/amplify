#ifndef AMPUSH_NAMETYPE_HPP
#define AMPUSH_NAMETYPE_HPP

#include <build-config.h>
#include "Type.hpp"
#include <memory>
#include <cstring>
#include <cstdlib>

namespace Amplify
{
  class Interpreter;
  class Program;

  class NameType : public Type
  {
  public:
    static const uint8_t type_number = 3;
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

    NameType();
    void interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const;
    void allocateStack(Interpreter& interpreter) const;
    void allocateStorage(Program& program) const;
    void registerInstructions(Interpreter& interpreter) const;
    size_t printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const;
    size_t randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const;
    static void getAllowedChars(size_t pos, const char **allowed_ptr, size_t *allowed_size_ptr);
  };
} /* namespace Amplify */

#endif /* AMPUSH_INTEGERTYPE_HPP */
