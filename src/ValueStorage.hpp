#ifndef AMPUSH_VALUESTORAGE_HPP
#define AMPUSH_VALUESTORAGE_HPP

#include <build-config.h>
#include "CodeType.hpp"
#include <cinttypes>
#include <vector>
#include <string>
#include <ostream>
#include <istream>

namespace Amplify
{
  class ValueStorage
  {
  public:
    ValueStorage(uint8_t type_number)
      : _type_number(type_number)
    {
    }

    virtual ~ValueStorage()
    {
    }

    const uint8_t typeNumber() const
    {
      return _type_number;
    }

    virtual void loadBinary(std::istream& stream, const size_t size)
    {
    }

    virtual size_t saveBinary(std::ostream& stream)
    {
      return 0;
    }

    virtual uint32_t binarySize() const
    {
      return 0;
    }

    virtual CodeType::Value create()
    {
      return CodeType::Value();
    }

    virtual CodeType::Value create(const std::string& string_value) = 0;
    virtual CodeType::Value create(const std::vector<CodeType::Value>& list) = 0;

  private:
    const uint8_t _type_number;
  };

} /* namespace Amplify */

#endif /* AMPUSH_VALUESTORAGE_HPP */
