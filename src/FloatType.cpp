#include <build-config.h>
#include <cmath>
#include "FloatTypeImpl.hpp"
#include "Interpreter.hpp"
#include "Logging.hpp"

namespace Amplify
{
  const std::string FloatType::type_string = ".float";

  AMPUSH_INSTRUCTION(FLOAT_SUM);
  AMPUSH_INSTRUCTION(FLOAT_MODULO);
  AMPUSH_INSTRUCTION(FLOAT_AVERAGE);

  static const InstructionTableEntry instruction_table[] = {
    { "FLOAT.SUM", &FLOAT_SUM,     {}},
    { "FLOAT.%",   &FLOAT_MODULO,  {}},
    { "FLOAT.AVG", &FLOAT_AVERAGE, {}}
  };

  FloatType::FloatType()
    : Type(type_number, type_string)
  {
  }

  void FloatType::interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const
  {
    LOG(TRACE) << "Interpreting FloatType value";
    const FloatStorage& storage = program.getStorage<FloatStorage>(type_number);
    const Value value = storage.getValue(value_base, value_slot);
    FloatStack& stack = interpreter.getDataStack<FloatStack>(type_number);
    stack.push(value);
    return;
  }

  void FloatType::allocateStack(Interpreter& interpreter) const
  {
    interpreter.setDataStack(type_number, std::unique_ptr<StackBase>(new FloatStack()));
  }

  void FloatType::allocateStorage(Program& program) const
  {
    program.setStorage(type_number, std::unique_ptr<ValueStorage>(new FloatStorage()));
  }

  void FloatType::registerInstructions(Interpreter& interpreter) const
  {
    for (size_t i = 0; i < sizeof(instruction_table)/sizeof(InstructionTableEntry); ++i) {
      auto const& entry = instruction_table[i];
      interpreter.addInstruction(entry.name, entry.function, nullptr);
    }
    return;
  }

  size_t FloatType::printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const
  {
    const FloatStorage& storage = program.getStorage<FloatStorage>(FloatType::type_number);
    FloatType::Value value = storage.getValue(base, slot);
    const std::string value_string = std::to_string(value);
    stream << value_string;
    return value_string.size();
  }

  size_t FloatType::randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const
  {
    FloatStorage& storage = program.getVolatileStorage<FloatStorage>(type_number);
    FloatType::Value randval = interpreter.random().randomFloat();
    CodeType::Value code = storage.create(randval);
    value = code;
    return 1;
  }

  /*
   * Float type instructions
   */
  AMPUSH_INSTRUCTION(FLOAT_SUM)
  {
    FloatStack& stack = interpreter.getDataStack<FloatStack>(FloatType::type_number);
    FloatType::Value sum = 0;

    for (size_t i = 0; i < stack.size(); ++i) {
      sum += stack.at(i);
    }

    stack.clear();
    stack.push(sum);

    return;
  }

  AMPUSH_INSTRUCTION(FLOAT_MODULO)
  {
    FloatStack& stack = interpreter.getDataStack<FloatStack>(FloatType::type_number);
    FloatType::Value mod = 0, value = 0, divisor = 0;

    if (stack.size() < 2) {
      return;
    }

    stack.pop(value);
    stack.pop(divisor);
    mod = fmod(value, divisor);
    stack.push(mod);

    return;
  }

  AMPUSH_INSTRUCTION(FLOAT_AVERAGE)
  {
    FloatStack& stack = interpreter.getDataStack<FloatStack>(FloatType::type_number);

    if (stack.size() < 1) {
      return;
    }

    FloatType::Value sum = 0;
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
