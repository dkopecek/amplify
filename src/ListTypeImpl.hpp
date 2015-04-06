#ifndef AMPUSH_LISTTYPEIMPL_HPP
#define AMPUSH_LISTTYPEIMPL_HPP

#include <build-config.h>
#include "ListType.hpp"
#include "ValueStorage.hpp"
#include "Logging.hpp"

namespace Amplify
{
  class ListStorage : public ValueStorage
  {
  public:
    ListStorage()
      : ValueStorage(ListType::type_number)
    {
    }

    ~ListStorage()
    {
      for (auto value : _values) {
	delete [] value;
      }      
    }

    CodeType::Value create(const std::string& string_value)
    {
      throw std::runtime_error("BUG: Don't know how to create a List type from string");
    }

    CodeType::Value create(const std::vector<CodeType::Value>& entries)
    {
      const uintptr_t slot = _values.size();
      uintptr_t current_slot = slot;
      size_t remaining_size = entries.size();
      size_t entries_index = 0;

      do {
	const uintptr_t current_slot = _values.size();
	const uint16_t size = remaining_size > UINT16_MAX ? UINT16_MAX : remaining_size;
	remaining_size -= size;
	const uint16_t cont = remaining_size > 0 ? 1 : 0;
	uint8_t* list_memory = new uint8_t[sizeof(ListType::Value)+sizeof(CodeType::Value)*size];
	ListType::Value* value = reinterpret_cast<ListType::Value*>(list_memory);

	value->size = size;
	value->cont = cont;

	for (uint16_t block_index = 0; block_index < size; ++block_index) {
	  value->items[block_index] = entries[entries_index];
	  ++entries_index;
	}

	if (cont) {
	  value->items[UINT16_MAX - 1] = CodeType::Value(ListType::type_number,
							 AMPUSH_BASE_STORAGE, current_slot + 1);
	}

	_values.push_back(value);
      } while (remaining_size > 0);

      return CodeType::Value(ListType::type_number, AMPUSH_BASE_STORAGE, slot);
    }

    const ListType::Value* getValue(uint8_t base, uintptr_t slot) const
    {
      return _values.at(slot);
    }

    void loadBinary(std::istream& stream, const size_t size)
    {
      size_t size_read = 0;
      while (size_read < size) {
	ListType::Value value_header;
	LOG(DEBUG) << "sizeof(ListValue)= " << sizeof(ListType::Value);
	stream.read(reinterpret_cast<char *>(&value_header), sizeof(ListType::Value));
	if (!stream.good()) {
	  throw std::runtime_error("Input stream error");
	}
	if (stream.gcount() != sizeof(ListType::Value)) {
	  throw std::runtime_error("ListType::Value load failed");
	}
	if (value_header.size > size) {
	  throw std::runtime_error("ListType::Value invalid value: too big");
	}
	ListType::Value *value = \
	  reinterpret_cast<ListType::Value*>(new uint8_t[sizeof(ListType::Value)
							 + value_header.size * sizeof(CodeType::Value)]);
	value->size = value_header.size;
	value->cont = value_header.cont;
	stream.read(reinterpret_cast<char *>(value->items), value->size * sizeof(CodeType::Value));
	if (stream.gcount() != (value->size * sizeof(CodeType::Value))) {
	  throw std::runtime_error("ListType::Value value read failed");
	}
	_values.push_back(value);
	size_read += sizeof(ListType::Value) + value_header.size * sizeof(CodeType::Value);
      }
      return;
    }

    size_t saveBinary(std::ostream& stream)
    {
      size_t size_saved = 0;
      for (auto const& value : _values) {
	stream.write(reinterpret_cast<const char *>(value),
		     sizeof(ListType::Value) + value->size * sizeof(CodeType::Value));
	size_saved += sizeof(ListType::Value) + value->size * sizeof(CodeType::Value);
      }
      return size_saved;
    }

    uint32_t binarySize() const
    {
      uint32_t size = 0;
      for (auto const& value : _values) {
	size += sizeof(ListType::Value) + value->size * sizeof(CodeType::Value);
      }
      return size;
    }

  private:
    std::vector<ListType::Value*> _values;
  };
} /* namespace Amplify */

#endif /* AMPUSH_LISTTYPEIMPL_HPP */
