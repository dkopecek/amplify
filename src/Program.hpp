#ifndef AMPUSH_PROGRAM_HPP
#define AMPUSH_PROGRAM_HPP

#include <build-config.h>
#include "CodeType.hpp"
#include "ValueStorage.hpp"
#include <vector>
#include <memory>
#include <ostream>
#include <istream>

namespace Amplify
{
  class Program
  {
  public:
    Program();

    CodeType::Value getCode() const;
    void setCode(const CodeType::Value& code);

    template<typename T>
    T getValue(uint8_t type, uint8_t base, uintptr_t slot) const
    {
      //return static_cast<T>(_values[type]->getValue(base, slot));
      return (T)0;
    }

    void loadBinaryValues(std::istream& stream, uint8_t type, const size_t size);
    size_t saveBinaryValues(std::ostream& stream, uint8_t type);
    uint32_t binaryValuesSize(uint8_t type);

    CodeType::Value createValue(uint8_t type, const std::string& value_string);
    CodeType::Value createValue(uint8_t type, const std::vector<CodeType::Value>& value_array);
    void setStorage(uint8_t type_number, std::unique_ptr<ValueStorage> storage);

    template<typename T>
    const T& getStorage(uint8_t type) const
    {
      return static_cast<const T&>(*_values.at(type));
    }

    template<typename T>
    T& getVolatileStorage(uint8_t type)
    {
      return static_cast<T&>(*_values.at(type));
    }

    void addType(const Type* type)
    {
      type->allocateStorage(*this);
    }

    bool typeExists(uint8_t type) const
    {
      if (_values.size() > type) {
	return _values[type] != nullptr;
      }
      return false;
    }

    size_t typeCount() const
    {
      return _values.size();
    }

  private:
    /*
     * Value storages indexed by 8bit type signature
     * The entry point for any program is at type 0,
     * base 0, slot 0.
     */
    CodeType::Value _code;
    std::vector<std::unique_ptr<ValueStorage> > _values;
  };

} /* namespace Amplify */

#endif /* AMPUSH_PROGRAM_HPP */
