#ifndef AMPUSH_CUSTOMTYPES_HPP
#define AMPUSH_CUSTOMTYPES_HPP

#include <build-config.h>
#include <cstddef>
#include <stdexcept>

namespace Amplify
{
  template<typename T, const size_t block_size = 64, void(*Deleter)(T&) = nullptr>
  class DynamicStack
  {
  public:
    DynamicStack()
    {
      _next_free_slot = 0;
      _blocks.push_back(new T[block_size]);
    }

    ~DynamicStack()
    {
      clear();
    }

    size_t size() const
    {
      return _next_free_slot;
    }

    void clear()
    {
      /* Delete values if a deleter is defined */
      if (Deleter != nullptr) {
	while (_next_free_slot > 0) {
	  --_next_free_slot;
	  const size_t block_index = _next_free_slot / block_size;
	  const size_t value_index = _next_free_slot % block_size;
	  Deleter(_blocks[block_index][value_index]);
	}
      } else {
	_next_free_slot = 0;
      }
      /* Delete value blocks */
      for (auto block : _blocks) {
	delete [] block;
      }
      _blocks.clear();
      return;
    }

    const T& at(size_t pos) const
    {
      if (pos >= _next_free_slot) {
	throw std::out_of_range("DynamicStack value position out of range");
      }

      const size_t block_index = pos / block_size;
      const size_t value_index = pos % block_size;
      return _blocks[block_index][value_index];
    }

    void push(const T& val)
    {
      const size_t block_index = _next_free_slot / block_size;
      const size_t value_index = _next_free_slot % block_size;
      if (block_index >= _blocks.size()) {
	_blocks.push_back(new T[block_size]);
      }
      _blocks[block_index][value_index] = val;
      ++_next_free_slot;
      return;
    }

    bool pop(T& val)
    {
      if (_next_free_slot < 1) {
	return false;
      } else {
	--_next_free_slot;
      }

      const size_t block_index = _next_free_slot / block_size;
      const size_t value_index = _next_free_slot % block_size;

      val = _blocks[block_index][value_index];

      if (block_index < (_blocks.size() - 1)) {
	delete [] _blocks[_blocks.size() - 1];
	_blocks.erase(_blocks.end() - 1);
      }

      return true;
    }

  private:
    size_t _next_free_slot;
    std::vector<T*> _blocks;
  };

} /* namespace Amplify */

#endif /* AMPUSH_CUSTOMTYPES_HPP */
