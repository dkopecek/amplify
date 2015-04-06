#include <build-config.h>
#include "Interpreter.hpp"
#include "CodeTypeImpl.hpp"
#include "Logging.hpp"
#include <iostream>

namespace Amplify
{
  Interpreter::Interpreter()
  {
  }

  Interpreter::~Interpreter()
  {
    for (auto const& n : _names) {
      LOG(INFO) << "Bound name: " << *n;
    }
  }

  void Interpreter::run(const Program& program)
  {
    CodeStack& stack = getExecStack<CodeStack>();
    CodeType::Value code = program.getCode();

    do {
      const Type* type = getType(code.type);
      LOG(TRACE) << "Interpreting value of type " << type->getTypeString();
      type->interpretValue(*this, program, code.base, code.slot);
    } while(stack.pop(code));

    return;
  }

  void Interpreter::addType(const Type* type)
  {
    const uint8_t type_number = type->getTypeNumber();

    if (_types.size() < (type_number + 1)) {
      _types.resize(type_number + 1);
    }

    auto& type_slot = _types[type_number];

    if (type_slot != nullptr) {
	throw std::runtime_error("Type already registered!");
    }

    _types_num.push_back(type_number);

    type_slot = type;
    type->allocateStack(*this);

    return;
  }

  const Type* Interpreter::getType(uint8_t type_number) const
  {
    return _types.at(type_number);
  }

  const std::vector<uint8_t>& Interpreter::getTypeNumbers() const
  {
    return _types_num;
  }

  void Interpreter::setDataStack(uint8_t type_number, std::unique_ptr<StackBase> stack)
  {
    if (_stacks.size() < (type_number + 1)) {
      _stacks.resize(type_number + 1);
    }

    auto& stack_slot = _stacks[type_number];

    if (stack_slot != nullptr) {
      throw std::runtime_error("Stack already set");
    }

    stack_slot = std::move(stack);

    return;
  }

  void Interpreter::setExecStack(std::unique_ptr<StackBase> stack)
  {
    if (_exec_stack != nullptr) {
      throw std::runtime_error("Exec stack already set");
    }
    _exec_stack = std::move(stack);
    return;
  }

  void Interpreter::printStacks() const
  {
    std::cout << (int)_exec_stack->getTypeNumber() << ": ";
    _exec_stack->print(std::cout);
    std::cout << std::endl;

    for (auto const& stack : _stacks) {
      if (stack == nullptr) {
	continue;
      }
      std::cout << (int)stack->getTypeNumber() << ": ";
      stack->print(std::cout);
      std::cout << std::endl;
    }
    return;
  }

  void Interpreter::printStacks(const Program& program) const
  {
    std::cout << (int)_exec_stack->getTypeNumber() << ": ";
    _exec_stack->print(*this, program, std::cout);
    std::cout << std::endl;

    for (auto const& stack : _stacks) {
      if (stack == nullptr) {
	continue;
      }
      std::cout << (int)stack->getTypeNumber() << ": ";
      stack->print(*this, program, std::cout);
      std::cout << std::endl;
    }
    return;
  }

  void Interpreter::addInstruction(const std::string& name, InstructionFn function, void *user_ptr)
  {
    /* obsolete function */
    addNameBinding(name, function, user_ptr);
    return;
  }

  void Interpreter::registerInstructions()
  {
    for (auto const& type : _types) {
      type->registerInstructions(*this);
    }
    return;
  }

  size_t Interpreter::addNameBinding(const std::string& name, InstructionFn function, void *user_ptr)
  {
    NameBinding binding;
    binding.instruction.function_pointer = function;
    binding.instruction.function_argument = user_ptr;
    return addNameBinding(name, binding);
  }

  size_t Interpreter::addNameBinding(const std::string& name, const Type::Value& value)
  {
    NameBinding binding;
    binding.value = value;
    return addNameBinding(name, binding);
  }

  size_t Interpreter::addNameBinding(const std::string& name, const NameBinding& binding)
  {
    auto& pair = _name_bindings[name];

    if (pair.second.empty()) {
      auto const it = _name_bindings.find(name);
      pair.first = _names.size();
      _names.push_back(&it->first);
    }

    pair.second.push(binding);
    return pair.second.size();
  }

  const Interpreter::NameBinding Interpreter::getNameBinding(const std::string& name) const
  {
    try {
      auto const& stack = _name_bindings.at(name).second;
      return stack.top();
    } catch(std::out_of_range& ex) {
      return NameBinding();
    }    
  }

  size_t Interpreter::getNameBindingCount() const
  {
    return _name_bindings.size();
  }

  const std::vector<const std::string *>& Interpreter::getNameBindingNames() const
  {
    return _names;
  }

} /* namespace Amplify */
