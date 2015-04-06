#ifndef AMPUSH_CODESTACKIMPL_HPP
#define AMPUSH_CODESTACKIMPL_HPP

#include <build-config.h>
#include "CodeType.hpp"
#include "ValueStorage.hpp"
#include "StackBase.hpp"
#include "Logging.hpp"
#include "CustomTypes.hpp"
#include "Interpreter.hpp"
#include <stack>

namespace Amplify
{
  class CodeStack : public StackBase
  {
  public:
    CodeStack()
      : StackBase(CodeType::type_number)
    {
    }

    void print(std::ostream& stream) const
    {
      for (size_t i = 0; i < size(); ++i) {
	const CodeType::Value& value = _stack.at(i);
	stream << '[' << (int)value.type
	       << ';' << (int)value.base
	       << ';' << value.slot
	       << ']';
	if ((i + 1) < size()) {
	  stream << ", ";
	}
      }
      return;
    }

    void print(const Interpreter& interpreter, const Program& program, std::ostream& stream) const
    {
      for (size_t i = 0; i < size(); ++i) {
	const CodeType::Value& value = _stack.at(i);
	const Type* const type = interpreter.getType(value.type);
	type->printValue(interpreter, program, stream, value.base, value.slot);
	if ((i + 1) < size()) {
	  stream << ", ";
	}
      }
      return;
    }

    bool pop(CodeType::Value& value)
    {
      return _stack.pop(value);
    }

    void push(const CodeType::Value& value)
    {
      _stack.push(value);
    }

    size_t size() const
    {
      return _stack.size();
    }

    const CodeType::Value& at(size_t pos)
    {
      return _stack.at(pos);
    }

    void clear()
    {
      _stack.clear();
    }

  private:
    DynamicStack<CodeType::Value> _stack;
  };

  class CodeStorage : public ValueStorage
  {
  public:
    CodeStorage()
      : ValueStorage(CodeType::type_number)
    {
    }

    CodeType::Value create(const std::string& string_value)
    {
      throw std::runtime_error("BUG: creating a CodeType value from string doesn't make sense");
    }

    CodeType::Value create(const std::vector<CodeType::Value>& list)
    {
      if (list.size() != 1) {
	throw std::runtime_error("BUG: Cannot CodeType::Value from more than one CodeType::Values");
      }
      uintptr_t slot = _values.size();
      _values.push_back(list[0]);
      return CodeType::Value(CodeType::type_number, AMPUSH_BASE_STORAGE, slot);
    }

    CodeType::Value getValue(uint8_t value_base, uintptr_t slot) const
    {
      if (value_base != AMPUSH_BASE_STORAGE) {
	throw std::runtime_error("Invalid value base");
      }
      return _values.at(slot);
    }

    void loadBinary(std::istream& stream, const size_t size)
    {
      size_t size_read = 0;
      while (size_read < size) {
	CodeType::Value value;
	stream.read(reinterpret_cast<char *>(&value), sizeof(value));
	if (!stream.good()) {
	  throw std::runtime_error("Input stream error");
	}
	if (stream.gcount() != sizeof(value)) {
	  throw std::runtime_error("CodeType::Value load failed");
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
      for (CodeType::Value value : _values) {
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
      return _values.size() * sizeof(CodeType::Value);
    }

  private:
    std::vector<CodeType::Value> _values;
  };
} /* namespace Amplify */

#endif /* AMPUSH_CODESTACKIMPL_HPP */
