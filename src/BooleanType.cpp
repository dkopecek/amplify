#include <build-config.h>
#include "BooleanTypeImpl.hpp"
#include "Interpreter.hpp"
#include "Logging.hpp"

namespace Amplify
{
  const std::string BooleanType::type_string = ".boolean";

  BooleanType::BooleanType()
    : Type(type_number, type_string)
  {
  }

  void BooleanType::interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const
  {
    LOG(TRACE) << "Interpreting BooleanType value";
    const BooleanStorage& storage = program.getStorage<BooleanStorage>(type_number);
    const Value value = storage.getValue(value_base, value_slot);
    BooleanStack& stack = interpreter.getDataStack<BooleanStack>(type_number);
    stack.push(value);
    return;
  }

  void BooleanType::allocateStack(Interpreter& interpreter) const
  {
    interpreter.setDataStack(type_number, std::unique_ptr<StackBase>(new BooleanStack()));
  }

  void BooleanType::allocateStorage(Program& program) const
  {
    program.setStorage(type_number, std::unique_ptr<ValueStorage>(new BooleanStorage()));
  }

  void BooleanType::registerInstructions(Interpreter& interpreter) const
  {
    return;
  }

  size_t BooleanType::printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const
  {
    (void)interpreter;
    (void)program;
    (void)base;
    bool value = static_cast<bool>(slot);
    stream << (value ? "#T" : "#F");
    return 2;
  }

  size_t BooleanType::randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const
  {
    BooleanStorage& storage = program.getVolatileStorage<BooleanStorage>(type_number);
    BooleanType::Value randval = interpreter.random().randomBoolean();
    CodeType::Value code = storage.create(randval);
    value = code;
    return 1;
  }

} /* namespace Amplify */
