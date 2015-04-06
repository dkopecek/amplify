#ifndef AMPUSH_INTEGERTYPEIMPL_HPP
#define AMPUSH_INTEGERTYPEIMPL_HPP

#include <build-config.h>
#include "IntegerType.hpp"
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
  class IntegerStack : public StackBase
  {
  public:
    IntegerStack()
      : StackBase(IntegerType::type_number)
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

    bool pop(IntegerType::Value& value)
    {
      return _stack.pop(value);
    }

    void push(const IntegerType::Value& value)
    {
      _stack.push(value);
    }

    const IntegerType::Value& at(size_t pos) const
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
    DynamicStack<IntegerType::Value> _stack;
  };

  class IntegerStorage : public ValueStorage
  {
  public:
    IntegerStorage()
      : ValueStorage(IntegerType::type_number)
    {
    }

    CodeType::Value create(const std::string& string_value)
    {
      IntegerType::Value value = stringToNumber<IntegerType::Value>(string_value);
      return create(value);
    }

    CodeType::Value create(IntegerType::Value value)
    {
      /*
       * TODO: If uintptr_t is large enough to hold a int64_t, store it directly
       * in the slot field.
       */
      const uintptr_t slot = _values.size();
      _values.push_back(value);

      return CodeType::Value(IntegerType::type_number, AMPUSH_BASE_STORAGE, slot);
    }

    CodeType::Value create(const std::vector<CodeType::Value>& list)
    {
      throw std::runtime_error("BUG: Don't know how to create an Integer type from a list");
    }

    IntegerType::Value getValue(uint8_t base, uintptr_t slot) const
    {
      switch (base) {
      case AMPUSH_BASE_STORAGE:
	return _values.at(slot);
      case AMPUSH_BASE_SLOT:
	return static_cast<IntegerType::Value>(slot);
      case AMPUSH_BASE_POINTER:
	return *reinterpret_cast<const IntegerType::Value*>(slot);
      }
      throw std::runtime_error("BUG: Unknown base");
    }

    void loadBinary(std::istream& stream, const size_t size)
    {
      size_t size_read = 0;
      while (size_read < size) {
	IntegerType::Value value;
	stream.read(reinterpret_cast<char *>(&value), sizeof(value));
	if (!stream.good()) {
	  throw std::runtime_error("Input stream error");
	}
	if (stream.gcount() != sizeof(value)) {
	  throw std::runtime_error("IntegerType::Value load failed");
	}
	_values.push_back(value);
	size_read += sizeof(value);
      }
      if (size_read != size) {
	throw std::runtime_error("size_read and size don't match");
      }
      return;
    }

    size_t saveBinary(std::ostream& stream)
    {
      size_t size_saved = 0;
      for (IntegerType::Value value : _values) {
	stream.write(reinterpret_cast<char *>(&value), sizeof(value));
	if (!stream.good()) {
	  throw std::runtime_error("Output stream error");
	}
	size_saved += sizeof(value);
      }
      return size_saved;
    }

    uint32_t binarySize() const
    {
      return _values.size() * sizeof(IntegerType::Value);
    }

  private:
    std::vector<IntegerType::Value> _values;
  };
} /* namespace Amplify */

#endif /* AMPUSH_INTEGERTYPEIMPL_HPP */
