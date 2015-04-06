#include <build-config.h>
#include "StringTypeImpl.hpp"
#include "Program.hpp"
#include "Interpreter.hpp"

namespace Amplify
{
  const std::string StringType::type_string = ".string";

  StringType::StringType()
    : Type(type_number, type_string)
  {
  }

  void StringType::interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const
  {
    LOG(TRACE) << "Interpreting StringType value";
    const StringStorage& storage = program.getStorage<StringStorage>(type_number);
    const StringType::Value* value = storage.getValue(value_base, value_slot);
    const std::string name(reinterpret_cast<const char *>(value->bytes), value->size);
    LOG(DEBUG) << "String stack push";
    StringStack& stack = interpreter.getDataStack<StringStack>(type_number);
    stack.push(std::move(value->copy()));
    return;
  }

  void StringType::allocateStack(Interpreter& interpreter) const
  {
    interpreter.setDataStack(type_number,
			     std::unique_ptr<StackBase>(new StringStack()));
  }

  void StringType::allocateStorage(Program& program) const
  {
    program.setStorage(type_number, std::unique_ptr<ValueStorage>(new StringStorage()));
  }

  size_t StringType::printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const
  {
    const StringStorage& storage = program.getStorage<StringStorage>(StringType::type_number);
    const StringType::Value* value = storage.getValue(base, slot);
    const std::string value_string(reinterpret_cast<const char *>(value->bytes), value->size);
    stream << '"' << value_string << '"';
    /* TODO: Handle escaped characters in lexer and when printing the value */
    return value_string.size() + 2;
  }

  size_t StringType::randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const
  {
    StringStorage& storage = program.getVolatileStorage<StringStorage>(type_number);
    std::string randval;
    size_t randlen = interpreter.random().randomNameLength();

    randval.resize(randlen);
    for (size_t i = 0; i < randlen; ++i) {
      const char *allowed = "012345679_- abcdefghijklmnopqrstuvwxyz";
      size_t allowed_size = strlen(allowed);
      size_t ri = interpreter.random().randomInteger64Uniform() % allowed_size;
      char randchar = allowed[ri];
      randval.push_back(randchar);
    }

    CodeType::Value code = storage.create(randval);
    value = code;
    return 1;
  }

} /* namespace Amplify */
