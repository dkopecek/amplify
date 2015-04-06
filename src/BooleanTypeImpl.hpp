#ifndef AMPUSH_BOOLEANTYPEIMPL_HPP
#define AMPUSH_BOOLEANTYPEIMPL_HPP

#include <build-config.h>
#include "BooleanType.hpp"
#include "StackBase.hpp"
#include "ValueStorage.hpp"
#include "Utility.hpp"
#include "Logging.hpp"
#include "CustomTypes.hpp"

#include <cinttypes>
#include <stack>
#include <vector>

namespace Amplify
{
  class BooleanStack : public StackBase
  {
  public:
    BooleanStack()
      : StackBase(BooleanType::type_number)
    {
    }

    void print(std::ostream& stream) const
    {
      for (size_t i = 0; i < size(); ++i) {
	stream << _stack.at(i);
	if ((i + 1) < size()) {
	  stream << ", ";
	}
      }
      return;
    }

    void print(const Interpreter& interpreter, const Program& program, std::ostream& stream) const
    {
      (void)interpreter;
      (void)program;
      print(stream);
      return;
    }

    bool pop(BooleanType::Value& value)
    {
      return _stack.pop(value);
    }

    void push(const BooleanType::Value& value)
    {
      _stack.push(value);
    }

    const BooleanType::Value& at(size_t pos) const
    {
      return _stack.at(pos);
    }

    void clear()
    {
      _stack.clear();
    }

    size_t size() const
    {
      return _stack.size();
    }

  private:
    DynamicStack<BooleanType::Value> _stack;
  };

  class BooleanStorage : public ValueStorage
  {
  public:
    BooleanStorage()
      : ValueStorage(BooleanType::type_number)
    {
    }

    CodeType::Value create(const std::string& string_value)
    {
      bool value;

      if (string_value == "#T") {
	value = true;
      }
      else if (string_value == "#F") {
	value = false;
      }
      else {
	throw std::runtime_error("BUG: Invalid string representation of a boolean value");
      }

      return create(value);
    }

    CodeType::Value create(bool value)
    {
      return CodeType::Value(BooleanType::type_number, AMPUSH_BASE_SLOT, (uintptr_t)value);
    }

    CodeType::Value create(const std::vector<CodeType::Value>& list)
    {
      throw std::runtime_error("BUG: Don't know how to create an Boolean type from a list");
    }

    BooleanType::Value getValue(uint8_t base, uintptr_t slot) const
    {
      if (base == AMPUSH_BASE_SLOT) {
	return static_cast<BooleanType::Value>(slot);
      }
      throw std::runtime_error("BUG: Unknown base");
    }
  };
} /* namespace Amplify */

#endif /* AMPUSH_BOOLEANTYPEIMPL_HPP */
