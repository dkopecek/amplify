#ifndef AMPUSH_LISTTYPE_HPP
#define AMPUSH_LISTTYPE_HPP

#include <build-config.h>
#include "Type.hpp"
#include "CodeType.hpp"

namespace Amplify
{
  class Interpreter;
  class Program;

  class ListType : public Type
  {
  public:
    static const uint8_t type_number = 1;
    static const std::string type_string;

    struct Value {
      Value()
      {
	size = 0; /**< number of items in this list block */
	cont = 0; /**< non-zero if the last item should be interpreter as a continuation of the list */
      }

      operator bool() const
      {
	return (size != 0 &&
		!(cont != 0 && size == 0));
      }

      uint16_t size;
      uint16_t cont;
      CodeType::Value items[0];
    } __attribute__((packed));

    ListType();
    void interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const;
    void allocateStack(Interpreter& interpreter) const;
    void allocateStorage(Program& program) const;
    size_t printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const;
    size_t randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const;
  };

} /* namespace Amplify */

#endif /* AMPUSH_LISTTYPE_HPP */
