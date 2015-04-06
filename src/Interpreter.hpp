#ifndef AMPUSH_INTERPRETER_HPP
#define AMPUSH_INTERPRETER_HPP

#include <build-config.h>
#include "Program.hpp"
#include "Type.hpp"
#include "StackBase.hpp"
#include "Instruction.hpp"
#include "Random.hpp"

#include <memory>
#include <vector>
#include <cinttypes>
#include <unordered_map>
#include <stack>

namespace Amplify
{
  class Interpreter
  {
  public:
    struct Instruction
    {
      InstructionFn function_pointer;
      void *function_argument;

      Instruction()
      {
	function_pointer = nullptr;
	function_argument = nullptr;
      }

      operator bool() const
      {
	return function_pointer != nullptr;
      }
    };

    struct NameBinding
    {
      Instruction instruction;
      Type::Value value;

      operator bool() const
      {
	return !!instruction ^ !!value;
      }
    };

    Interpreter();
    ~Interpreter();

    void run(const Program& program);
    void addType(const Type* type);
    const Type* getType(uint8_t type_number) const;
    const std::vector<uint8_t>& getTypeNumbers() const;

    void setDataStack(uint8_t type_number, std::unique_ptr<StackBase> stack);

    template<typename T>
    T& getDataStack(uint8_t type_number)
    {
      return static_cast<T&>(*_stacks.at(type_number));
    }

    void setExecStack(std::unique_ptr<StackBase> stack);

    template<typename T>
    T& getExecStack()
    {
      return static_cast<T&>(*_exec_stack);
    }

    void printStacks() const;
    void printStacks(const Program& program) const;

    void addInstruction(const std::string& name, InstructionFn function, void *user_ptr = NULL);
    const Instruction getInstruction(const std::string& name) const;
    void registerInstructions();

    size_t addNameBinding(const std::string& name, InstructionFn function, void *user_ptr = NULL);
    size_t addNameBinding(const std::string& name, const Type::Value& value);
    const NameBinding getNameBinding(const std::string& name) const;
    size_t getNameBindingCount() const;
    const std::vector<const std::string *>& getNameBindingNames() const;

    Random& random()
    {
      return _random;
    }
  protected:
    size_t addNameBinding(const std::string& name, const NameBinding& binding);

  private:
    std::vector<uint8_t> _types_num;
    std::vector<const Type *> _types;
    std::unique_ptr<StackBase> _exec_stack;
    std::vector<std::unique_ptr<StackBase> > _stacks;
    std::unordered_map<std::string, std::pair<size_t, std::stack<NameBinding> > > _name_bindings;
    std::vector<const std::string *> _names;

    Random _random;
  };

} /* namespace Amplify */

#endif /* AMPUSH_INTERPRETER_HPP */
