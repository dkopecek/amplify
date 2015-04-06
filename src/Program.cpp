#include <build-config.h>
#include "Program.hpp"

namespace Amplify
{
  Program::Program()
  {
  }

  CodeType::Value Program::getCode() const
  {
    return _code;
  }

  void Program::setCode(const CodeType::Value& code)
  {
    _code = code;
  }

  void Program::loadBinaryValues(std::istream& stream, uint8_t type, const size_t size)
  {
    _values.at(type)->loadBinary(stream, size);
  }

  size_t Program::saveBinaryValues(std::ostream& stream, uint8_t type)
  {
    _values.at(type)->saveBinary(stream);
  }

  uint32_t Program::binaryValuesSize(uint8_t type)
  {
    return _values.at(type)->binarySize();
  }
  
  CodeType::Value Program::createValue(uint8_t type, const std::string& value_string)
  {
    return _values.at(type)->create(value_string);
  }

  CodeType::Value Program::createValue(uint8_t type, const std::vector<CodeType::Value>& value_array)
  {
    return _values.at(type)->create(value_array);
  }

  void Program::setStorage(uint8_t type_number, std::unique_ptr<ValueStorage> storage)
  {
    if (_values.size() < (type_number + 1)) {
      _values.resize(type_number + 1);
    }
    if (_values[type_number] != nullptr) {
      throw std::runtime_error("Storage already set");
    }
    _values[type_number] = std::move(storage);
    return;
  }

} /* namespace Amplify */
