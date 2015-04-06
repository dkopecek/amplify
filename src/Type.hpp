#ifndef AMPUSH_TYPE_HPP
#define AMPUSH_TYPE_HPP

#include <build-config.h>
#include <cinttypes>
#include <memory>
#include <string>

namespace Amplify
{
  const uint8_t AMPUSH_BASE_POINTER = 255; /**< value is stored at a memory location pointed to by the value of the slot field */
  const uint8_t AMPUSH_BASE_SLOT    = 254; /**< value is stored in the slot field */
  const uint8_t AMPUSH_BASE_STORAGE = 253; /**< value is stored in the value storage */ 
  const uint8_t AMPUSH_BASE_MAX     = 253; /**< maximum index value usable in the value storage */

  class Interpreter;
  class Program;
  
  class Type
  {
  public:
    /*
     * A value of a Amplify type is uniquely identified
     * by this triple. The structure contains the type
     * number itself to allow the type to generate values
     * of others types too.
     */
    struct Value
    {
      Value()
      {
	slot = 0;
	type = 255;
	base = 0;
      }

      Value(uint8_t _type, uint8_t _base, uintptr_t _slot)
      {
	slot = _slot;
	type = _type;
	base = _base;
      }

      operator bool() const
      {
	return !(slot == 0 && type == 255 && base == 0);
      }

      uintptr_t slot;/**< slot specifier relative to a base or a memory pointer */
      uint8_t  type; /**< 8-bit type signature */
      uint8_t  base; /**< 8-bit slot base specifier; 0 => slot contains an absolute address */
    } __attribute__((packed));


    Type(uint8_t type_number, const std::string& type_string)
      : _type_number(type_number),
	_type_string(type_string)
    {
    }

    const uint8_t getTypeNumber() const
    {
      return _type_number;
    }

    const std::string& getTypeString() const
    {
      return _type_string;
    }

    virtual void interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const = 0;
    virtual void allocateStack(Interpreter& interpreter) const = 0;
    virtual void allocateStorage(Program& program) const = 0;
    virtual void registerInstructions(Interpreter& interpreter) const {}
    virtual size_t printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t value_base, uintptr_t value_slot) const = 0;
    virtual size_t randomValue(Interpreter& interpreter, Program& program, Value& value, size_t max_points) const = 0;

  private:
    const uint8_t _type_number;
    const std::string _type_string;
  };
} /* namespace Amplify */

#endif /* AMPUSH_TYPE_HPP */
