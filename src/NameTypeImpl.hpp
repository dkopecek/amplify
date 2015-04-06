#ifndef AMPUSH_NAMETYPEIMPL_HPP
#define AMPUSH_NAMETYPEIMPL_HPP

#include <build-config.h>
#include "NameType.hpp"
#include "StackBase.hpp"
#include "ValueStorage.hpp"
#include "Utility.hpp"
#include "Logging.hpp"
#include "CustomTypes.hpp"
#include <memory>
#include <cstring>

namespace Amplify
{
  class NameStack : public StackBase
  {
  public:
    NameStack()
      : StackBase(NameType::type_number)
    {
    }

    void print(std::ostream& stream) const
    {
      for (size_t i = 0; i < size(); ++i) {
	const NameType::Value* value = _stack.at(i);
	stream << std::string(reinterpret_cast<const char *>(value->bytes), value->size);
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

    bool pop(std::unique_ptr<NameType::Value>& value_ptr)
    {
      NameType::Value* value = nullptr;
      if (!_stack.pop(value)) {
	return false;
      }
      value_ptr = std::unique_ptr<NameType::Value>(value);
      return true;
    }

    void push(std::unique_ptr<NameType::Value> value_ptr)
    {
      NameType::Value* value = value_ptr.release();
      _stack.push(value);
      return;
    }

    const NameType::Value* at(size_t pos) const
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
    static void stackValueDeleter(NameType::Value*& value)
    {
      LOG(DEBUG) << "Freeing " << value;
      free(value);
    }

  private:
    DynamicStack<NameType::Value*, 64, stackValueDeleter> _stack;
  };

  class NameStorage : public ValueStorage
  {
  public:
    NameStorage()
      : ValueStorage(NameType::type_number)
    {
    }

    ~NameStorage()
    {
      for (auto value : _values) {
	delete [] value;
      }      
    }

    CodeType::Value create(const std::string& string_value)
    {
      NameType::Value *value = \
	reinterpret_cast<NameType::Value*>(new uint8_t[sizeof(NameType::Value)
						       + string_value.size()]);
      value->size = string_value.size();
      memcpy(value->bytes, string_value.c_str(), value->size);
      /*
       * TODO: If uintptr_t is large enough to hold a int64_t, store it directly
       * in the slot field.
       */
      const uintptr_t slot = _values.size();
      _values.push_back(value);

      return CodeType::Value(NameType::type_number, AMPUSH_BASE_STORAGE, slot);
    }

    CodeType::Value create(const std::vector<CodeType::Value>& list)
    {
      throw std::runtime_error("BUG: Don't know how to create an Name type from a list");
    }

    const NameType::Value* getValue(uint8_t base, uintptr_t slot) const
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
	NameType::Value value_header;
	stream.read(reinterpret_cast<char *>(&value_header), sizeof(NameType::Value));
	if (!stream.good()) {
	  throw std::runtime_error("Input stream error");
	}
	if (stream.gcount() != sizeof(NameType::Value)) {
	  throw std::runtime_error("NameType::Value load failed");
	}
	if (value_header.size > size) {
	  throw std::runtime_error("NamType::Value invalid value: too big");
	}
	NameType::Value *value = \
	  reinterpret_cast<NameType::Value*>(new uint8_t[sizeof(NameType::Value)
							 + value_header.size]);
	value->size = value_header.size;
	stream.read(reinterpret_cast<char *>(value->bytes), value->size);
	if (stream.gcount() != value->size) {
	  throw std::runtime_error("NameType::Value value read failed");
	}
	_values.push_back(value);
	size_read += sizeof(NameType::Value) + value_header.size;
      }
      return;
    }

    size_t saveBinary(std::ostream& stream)
    {
      size_t size_saved = 0;
      for (auto const& value : _values) {
	stream.write(reinterpret_cast<const char *>(value), sizeof(NameType::Value) + value->size);
	size_saved += sizeof(NameType::Value) + value->size;
      }
      return size_saved;
    }

    uint32_t binarySize() const
    {
      uint32_t size = 0;
      for (auto const& value : _values) {
	size += sizeof(NameType::Value) + value->size;
      }
      return size;
    }

  private:
    std::vector<NameType::Value*> _values;
  };

} /* namespace Amplify */

#endif /* AMPUSH_NAMETYPEIMPL_HPP */
