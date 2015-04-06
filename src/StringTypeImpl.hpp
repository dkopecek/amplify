#ifndef AMPUSH_STRINGTYPEIMPL_HPP
#define AMPUSH_STRINGTYPEIMPL_HPP

#include <build-config.h>
#include "StringType.hpp"
#include "StackBase.hpp"
#include "ValueStorage.hpp"
#include "Utility.hpp"
#include "Logging.hpp"
#include "CustomTypes.hpp"
#include <memory>
#include <cstring>

namespace Amplify
{
  class StringStack : public StackBase
  {
  public:
    StringStack()
      : StackBase(StringType::type_number)
    {
    }

    void print(std::ostream& stream) const
    {
      for (size_t i = 0; i < size(); ++i) {
	const StringType::Value* value = _stack.at(i);
	stream << '"'
	       << std::string(reinterpret_cast<const char *>(value->bytes), value->size)
	       << '"';
	if (i < (size() - 1)) {
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

    bool pop(std::unique_ptr<StringType::Value>& value_ptr)
    {
      StringType::Value* value = nullptr;
      if (!_stack.pop(value)) {
	return false;
      }
      value_ptr = std::unique_ptr<StringType::Value>(value);
      return true;
    }

    void push(std::unique_ptr<StringType::Value> value_ptr)
    {
      StringType::Value* value = value_ptr.release();
      _stack.push(value);
      return;
    }

    const StringType::Value* at(size_t pos) const
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

  protected:
    static void stackValueDeleter(StringType::Value*& value)
    {
      LOG(DEBUG) << "Freeing " << value;
      free(value);
    }

  private:
    DynamicStack<StringType::Value*, 64, stackValueDeleter> _stack;
  };

  class StringStorage : public ValueStorage
  {
  public:
    StringStorage()
      : ValueStorage(StringType::type_number)
    {
    }

    ~StringStorage()
    {
      for (auto value : _values) {
	delete [] value;
      }      
    }

    CodeType::Value create(const std::string& string_value)
    {
      StringType::Value *value = \
	reinterpret_cast<StringType::Value*>(new uint8_t[sizeof(StringType::Value)
						       + string_value.size()]);
      value->size = string_value.size();
      memcpy(value->bytes, string_value.c_str(), value->size);
      /*
       * TODO: If uintptr_t is large enough to hold a int64_t, store it directly
       * in the slot field.
       */
      const uintptr_t slot = _values.size();
      _values.push_back(value);

      return CodeType::Value(StringType::type_number, AMPUSH_BASE_STORAGE, slot);
    }

    CodeType::Value create(const std::vector<CodeType::Value>& list)
    {
      throw std::runtime_error("BUG: Don't know how to create an String type from a list");
    }

    const StringType::Value* getValue(uint8_t base, uintptr_t slot) const
    {
      switch (base) {
      case AMPUSH_BASE_STORAGE:
	return _values.at(slot);
      }
      throw std::runtime_error("BUG: Unknown base");
    }

    void loadBinary(std::istream& stream, const size_t size)
    {
      size_t size_read = 0;
      while (size_read < size) {
	StringType::Value value_header;
	stream.read(reinterpret_cast<char *>(&value_header), sizeof(StringType::Value));
	if (!stream.good()) {
	  throw std::runtime_error("Input stream error");
	}
	if (stream.gcount() != sizeof(StringType::Value)) {
	  throw std::runtime_error("StringType::Value load failed");
	}
	if (value_header.size > size) {
	  throw std::runtime_error("NamType::Value invalid value: too big");
	}
	StringType::Value *value = \
	  reinterpret_cast<StringType::Value*>(new uint8_t[sizeof(StringType::Value)
							 + value_header.size]);
	value->size = value_header.size;
	stream.read(reinterpret_cast<char *>(value->bytes), value->size);
	if (stream.gcount() != value->size) {
	  throw std::runtime_error("StringType::Value value read failed");
	}
	_values.push_back(value);
	size_read += sizeof(StringType::Value) + value_header.size;
      }
      return;
    }

    size_t saveBinary(std::ostream& stream)
    {
      size_t size_saved = 0;
      for (auto const& value : _values) {
	stream.write(reinterpret_cast<const char *>(value), sizeof(StringType::Value) + value->size);
	size_saved += sizeof(StringType::Value) + value->size;
      }
      return size_saved;
    }

    uint32_t binarySize() const
    {
      uint32_t size = 0;
      for (auto const& value : _values) {
	size += sizeof(StringType::Value) + value->size;
      }
      return size;
    }

  private:
    std::vector<StringType::Value*> _values;
  };

} /* namespace Amplify */

#endif /* AMPUSH_STRINGTYPEIMPL_HPP */
