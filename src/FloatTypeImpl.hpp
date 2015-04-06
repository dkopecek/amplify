#ifndef AMPUSH_FLOATTYPEIMPL_HPP
#define AMPUSH_FLOATTYPEIMPL_HPP

#include <build-config.h>
#include "FloatType.hpp"
#include "StackBase.hpp"
#include "ValueStorage.hpp"
#include "Utility.hpp"
#include "Logging.hpp"
#include "CustomTypes.hpp"

#include <cstdio>
#include <cinttypes>
#include <stack>
#include <vector>

namespace Amplify
{
  class FloatStack : public StackBase
  {
  public:
    FloatStack()
      : StackBase(FloatType::type_number)
    {
    }

    void print(std::ostream& stream) const
    {
      char string_buffer[128];

      for (size_t i = 0; i < size(); ++i) {
	snprintf(string_buffer, sizeof string_buffer, "%la", _stack.at(i));
	stream << string_buffer;
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

    bool pop(FloatType::Value& value)
    {
      return _stack.pop(value);
    }

    void push(const FloatType::Value& value)
    {
      _stack.push(value);
    }

    const FloatType::Value& at(size_t pos) const
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
    DynamicStack<FloatType::Value> _stack;
  };

  class FloatStorage : public ValueStorage
  {
  public:
    FloatStorage()
      : ValueStorage(FloatType::type_number)
    {
    }

    CodeType::Value create(const std::string& string_value)
    {
      double float_value;

      if (sscanf(string_value.c_str(), "%la", &float_value) != 1) {
	if (sscanf(string_value.c_str(), "%lA", &float_value) != 1) {
	  float_value = stringToNumber<double>(string_value);
	}
      }
      
      return create(float_value);
    }

    CodeType::Value create(FloatType::Value value)
    {
     /*
       * TODO: If uintptr_t is large enough to hold a int64_t, store it directly
       * in the slot field.
       */
      const uintptr_t slot = _values.size();
      _values.push_back(value);

      return CodeType::Value(FloatType::type_number, AMPUSH_BASE_STORAGE, slot);
    }

    CodeType::Value create(const std::vector<CodeType::Value>& list)
    {
      throw std::runtime_error("BUG: Don't know how to create an Float type from a list");
    }

    FloatType::Value getValue(uint8_t base, uintptr_t slot) const
    {
      switch (base) {
      case AMPUSH_BASE_STORAGE:
	return _values.at(slot);
      case AMPUSH_BASE_SLOT:
	return static_cast<FloatType::Value>(slot);
      case AMPUSH_BASE_POINTER:
	return *reinterpret_cast<const FloatType::Value*>(slot);
      }
      throw std::runtime_error("BUG: Unknown base");
    }

    void loadBinary(std::istream& stream, const size_t size)
    {
      size_t size_read = 0;
      while (size_read < size) {
	FloatType::Value value;
	stream.read(reinterpret_cast<char *>(&value), sizeof(value));
	if (!stream.good()) {
	  throw std::runtime_error("Input stream error");
	}
	if (stream.gcount() != sizeof(value)) {
	  throw std::runtime_error("FloatType::Value load failed");
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
      for (FloatType::Value value : _values) {
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
      return _values.size() * sizeof(FloatType::Value);
    }

  private:
    std::vector<FloatType::Value> _values;
  };
} /* namespace Amplify */

#endif /* AMPUSH_FLOATTYPEIMPL_HPP */
