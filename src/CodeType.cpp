#include <build-config.h>
#include "CodeTypeImpl.hpp"
#include "IntegerTypeImpl.hpp"
#include "Program.hpp"
#include "Interpreter.hpp"
#include "Logging.hpp"

namespace Amplify
{
  const std::string CodeType::type_string = ".code";

  AMPUSH_INSTRUCTION(CODE_RANDOM);

  static const InstructionTableEntry instruction_table[] = {
    { "CODE.RANDOM", &CODE_RANDOM,     {}},
  };

  CodeType::CodeType()
    : Type(type_number, type_string)
  {
  }

  void CodeType::interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const
  {
    const CodeStorage& storage = program.getStorage<CodeStorage>(type_number);
    const Value value = storage.getValue(value_base, value_slot);
    CodeStack& code_stack = interpreter.getDataStack<CodeStack>(type_number);
    CodeStack& exec_stack = interpreter.getExecStack<CodeStack>();

    code_stack.push(value);
    exec_stack.push(value);

    return;
  }

  void CodeType::allocateStack(Interpreter& interpreter) const
  {
    interpreter.setExecStack(std::unique_ptr<StackBase>(new CodeStack()));
    interpreter.setDataStack(type_number,
			     std::unique_ptr<StackBase>(new CodeStack()));
    return;
  }

  void CodeType::allocateStorage(Program& program) const
  {
    program.setStorage(type_number, std::unique_ptr<ValueStorage>(new CodeStorage()));
    return;
  }

  void CodeType::registerInstructions(Interpreter& interpreter) const
  {
    for (size_t i = 0; i < sizeof(instruction_table)/sizeof(InstructionTableEntry); ++i) {
      auto const& entry = instruction_table[i];
      interpreter.addInstruction(entry.name, entry.function, nullptr);
    }
    return;
  }

  size_t CodeType::printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const
  {
    const CodeStorage& storage = program.getStorage<CodeStorage>(CodeType::type_number);
    CodeType::Value value = storage.getValue(base, slot);

    if (!value) {
      return 0;
    }

    const Type* type = interpreter.getType(value.type);

    return type->printValue(interpreter, program, stream, value.base, value.slot);
  }

  size_t CodeType::randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const
  {
    CodeStorage& storage = program.getVolatileStorage<CodeStorage>(type_number);

    /*
     * Randomly select one of the supported types:
     *  list, int, name, bool, float, string
     * TODO: split to terminal / non-terminal (list)
     */
    const std::vector<uint8_t>& types_num = interpreter.getTypeNumbers();
    size_t i;

    /*
     * Select a random type number. If the selected
     * type matches the code type, select again.
     */
    do {
      i = ((size_t)interpreter.random().randomInteger64Uniform()) % types_num.size();
    } while(types_num[i] == CodeType::type_number);

    /*
     * Create a random value of the selected type.
     */
    CodeType::Value randval;
    const Type* type = interpreter.getType(types_num[i]);
    size_t points = type->randomValue(interpreter, program, randval, max_points);

    value = randval;

    /*
     * Return the number of points in the generated value.
     */
    return points;
  }

  AMPUSH_INSTRUCTION(CODE_RANDOM)
  {
    CodeStack& code_stack = interpreter.getDataStack<CodeStack>(CodeType::type_number);
    IntegerStack& int_stack = interpreter.getDataStack<IntegerStack>(IntegerType::type_number);
    const Type* code_type = interpreter.getType(CodeType::type_number);
    CodeType::Value code;
    IntegerType::Value points;

    if (!int_stack.pop(points)) {
      points = 64;
    }

    code_type->randomValue(interpreter, const_cast<Program&>(program), code, 100);
    code_stack.push(code);

    return;
  }

} /* namespace Amplify */
