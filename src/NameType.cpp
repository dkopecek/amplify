#include <build-config.h>
#include "NameTypeImpl.hpp"
#include "CodeTypeImpl.hpp"
#include "Program.hpp"
#include "Interpreter.hpp"

namespace Amplify
{
  const std::string NameType::type_string = ".name";

  AMPUSH_INSTRUCTION(NAME_GENERATE);

  static const InstructionTableEntry instruction_table[] = {
    { "NAME.GENERATE", &NAME_GENERATE,     {}},
  };

  NameType::NameType()
    : Type(type_number, type_string)
  {
  }

  void NameType::interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const
  {
    LOG(TRACE) << "Interpreting NameType value";
    const NameStorage& storage = program.getStorage<NameStorage>(type_number);
    const NameType::Value* value = storage.getValue(value_base, value_slot);
    const std::string name(reinterpret_cast<const char *>(value->bytes), value->size);

    /* Look for name bindings */
    const Interpreter::NameBinding binding = interpreter.getNameBinding(name);

    if (binding) {
      if (binding.instruction) {
	LOG(DEBUG) << "Found instruction for name " << name;
	binding.instruction.function_pointer(interpreter, program,
					     binding.instruction.function_argument);
      } else {
	LOG(DEBUG) << "Found value binding for name " << name;
	CodeStack& exec_stack = interpreter.getExecStack<CodeStack>();
	exec_stack.push(binding.value);
      }
    } else {
      /* Otherwise, push to stack */
      LOG(DEBUG) << "Name stack push";
      NameStack& stack = interpreter.getDataStack<NameStack>(type_number);
      stack.push(std::move(value->copy()));
    }
    return;
  }

  void NameType::allocateStack(Interpreter& interpreter) const
  {
    interpreter.setDataStack(type_number,
			     std::unique_ptr<StackBase>(new NameStack()));
  }

  void NameType::allocateStorage(Program& program) const
  {
    program.setStorage(type_number, std::unique_ptr<ValueStorage>(new NameStorage()));
  }

  void NameType::registerInstructions(Interpreter& interpreter) const
  {
    for (size_t i = 0; i < sizeof(instruction_table)/sizeof(InstructionTableEntry); ++i) {
      auto const& entry = instruction_table[i];
      interpreter.addInstruction(entry.name, entry.function, nullptr);
    }
    return;
  }

  size_t NameType::printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const
  {
    const NameStorage& storage = program.getStorage<NameStorage>(NameType::type_number);
    const NameType::Value* value = storage.getValue(base, slot);
    const std::string value_string(reinterpret_cast<const char *>(value->bytes), value->size);
    stream << value_string;
    return value_string.size();
  }

  size_t NameType::randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const
  {
    auto const& names = interpreter.getNameBindingNames();
    size_t ri = ((size_t)interpreter.random().randomInteger64Uniform()) % names.size();
    NameStorage& storage = program.getVolatileStorage<NameStorage>(type_number);
    CodeType::Value code = storage.create(*names[ri]);
    value = code;

    return 1;
  }

  void NameType::getAllowedChars(size_t pos, const char **allowed_ptr, size_t *allowed_size_ptr)
  {
    static const char allowed[] = "0123456789-./~!@#$%^&*+=_abcdefghijklmnopqrstuvwxzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (pos < 1) {
      *allowed_ptr = allowed + 24;
      *allowed_size_ptr = strlen(*allowed_ptr);
      return;
    } else {
      *allowed_ptr = allowed;
      *allowed_size_ptr = strlen(*allowed_ptr);
      return;
    }
  }

  AMPUSH_INSTRUCTION(NAME_GENERATE)
  {
    NameStorage& storage = const_cast<Program&>(program).getVolatileStorage<NameStorage>(NameType::type_number);
    std::string randval_str;
    size_t randlen = interpreter.random().randomNameLength();
    randval_str.resize(randlen);

    for (size_t i = 0; i < randlen; ++i) {
      const char *allowed = NULL;
      size_t allowed_size = 0;
      NameType::getAllowedChars(i, &allowed, &allowed_size);
      size_t ri = interpreter.random().randomInteger64Uniform() % allowed_size;
      char randchar = allowed[ri];
      randval_str.push_back(randchar);
    }

    NameType::Value *value = \
      reinterpret_cast<NameType::Value*>(new uint8_t[sizeof(NameType::Value)
						     + randval_str.size()]);
    value->size = randval_str.size();
    memcpy(value->bytes, randval_str.c_str(), value->size);

    //std::unique_ptr<NameType::Value> randval(value);
    NameStack& stack = interpreter.getDataStack<NameStack>(NameType::type_number);
    stack.push(value->copy());
    return;
  }

} /* namespace Amplify */
