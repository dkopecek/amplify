#ifndef AMPUSH_INSTRUCTION_HPP
#define AMPUSH_INSTRUCTION_HPP

#include <build-config.h>

namespace Amplify
{
  class Interpreter;
  class Program;

  typedef void(*InstructionFn)(Interpreter&, const Program&, void *);

#define AMPUSH_INSTRUCTION(name) static void name(Interpreter& interpreter, const Program& program, void *user_ptr)

  struct InstructionTableEntry
  {
    const char * const name;
    InstructionFn function;
    const std::vector<uint8_t> required_types;
  };

} /* namespace Amplify */

#endif /* AMPUSH_INSTRUCTION_HPP */
