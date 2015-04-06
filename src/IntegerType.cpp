#include <build-config.h>
#include "IntegerTypeImpl.hpp"
#include "Interpreter.hpp"
#include "Logging.hpp"

namespace Amplify
{
  const std::string IntegerType::type_string = ".integer";

  AMPUSH_INSTRUCTION(INTEGER_SUM);
  AMPUSH_INSTRUCTION(INTEGER_MODULO);
  AMPUSH_INSTRUCTION(INTEGER_AVERAGE);

  static const InstructionTableEntry instruction_table[] = {
    { "INTEGER.SUM", &INTEGER_SUM,     {}},
    { "INTEGER.%",   &INTEGER_MODULO,  {}},
    { "INTEGER.AVG", &INTEGER_AVERAGE, {}}
  };

  IntegerType::IntegerType()
    : Type(type_number, type_string)
  {
  }

  void IntegerType::interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const
  {
    LOG(TRACE) << "Interpreting IntegerType value";
    const IntegerStorage& storage = program.getStorage<IntegerStorage>(type_number);
    const Value value = storage.getValue(value_base, value_slot);
    IntegerStack& stack = interpreter.getDataStack<IntegerStack>(type_number);
    stack.push(value);
    return;
  }

  void IntegerType::allocateStack(Interpreter& interpreter) const
  {
    interpreter.setDataStack(type_number, std::unique_ptr<StackBase>(new IntegerStack()));
  }

  void IntegerType::allocateStorage(Program& program) const
  {
    program.setStorage(type_number, std::unique_ptr<ValueStorage>(new IntegerStorage()));
  }

  void IntegerType::registerInstructions(Interpreter& interpreter) const
  {
    for (size_t i = 0; i < sizeof(instruction_table)/sizeof(InstructionTableEntry); ++i) {
      auto const& entry = instruction_table[i];
      interpreter.addInstruction(entry.name, entry.function, nullptr);
    }
    return;
  }

  size_t IntegerType::printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const
  {
    const IntegerStorage& storage = program.getStorage<IntegerStorage>(IntegerType::type_number);
    IntegerType::Value value = storage.getValue(base, slot);
    const std::string value_string = std::to_string(value);
    stream << value_string;
    return value_string.size();
  }

  size_t IntegerType::randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const
  {
    IntegerStorage& storage = program.getVolatileStorage<IntegerStorage>(type_number);
    IntegerType::Value randval = interpreter.random().randomInteger64();
    CodeType::Value code = storage.create(randval);
    value = code;
    return 1;
  }

  /*
   * Integer type instructions
   */
  AMPUSH_INSTRUCTION(INTEGER_SUM)
  {
    IntegerStack& stack = interpreter.getDataStack<IntegerStack>(IntegerType::type_number);
    IntegerType::Value sum = 0;

    for (size_t i = 0; i < stack.size(); ++i) {
      sum += stack.at(i);
    }

    stack.clear();
    stack.push(sum);

    return;
  }

  AMPUSH_INSTRUCTION(INTEGER_MODULO)
  {
    IntegerStack& stack = interpreter.getDataStack<IntegerStack>(IntegerType::type_number);
    IntegerType::Value mod = 0, value = 0, divisor = 0;

    if (stack.size() < 2) {
      return;
    }

    stack.pop(value);
    stack.pop(divisor);
    mod = value % divisor;
    stack.push(mod);

    return;
  }

  AMPUSH_INSTRUCTION(INTEGER_AVERAGE)
  {
    IntegerStack& stack = interpreter.getDataStack<IntegerStack>(IntegerType::type_number);

    if (stack.size() < 1) {
      return;
    }

    IntegerType::Value sum = 0;
    size_t count = 0;

    for (size_t i = 0; i < stack.size(); ++i) {
      sum += stack.at(i);
      ++count;
    }

    stack.clear();
    stack.push(sum / count);

    return;
  }

} /* namespace Amplify */
