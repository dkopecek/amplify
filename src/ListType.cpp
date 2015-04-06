#include <build-config.h>
#include "ListTypeImpl.hpp"
#include "CodeTypeImpl.hpp"
#include "Program.hpp"
#include "Interpreter.hpp"
#include "Logging.hpp"

namespace Amplify
{
  const std::string ListType::type_string = ".list";

  ListType::ListType()
    : Type(type_number, type_string)
  {
  }

  void ListType::interpretValue(Interpreter& interpreter, const Program& program, uint8_t value_base, uintptr_t value_slot) const
  {
    /* 
     * List is a built-in type and a building block in
     * the Push language. When interpreting a list, the
     * list items have to be pushed to the exec stack
     * in reverse order (the first item in the list will
     * be executed first).
     */
    const ListStorage& storage = program.getStorage<ListStorage>(type_number);
    const Value* list = storage.getValue(value_base, value_slot);

    LOG(TRACE) << "Interpreting ListType value";

    if (list->cont != 0) {
      LOG(TRACE) << "Last block is list continuation";
      /*
       * The last item must be of type List. It's interpreted as the
       * continuation of the current list until a block with last == 0
       * is found.
       */
      const CodeType::Value& last = list->items[list->size - 1];
      interpretValue(interpreter, program, last.base, last.slot);
    }

    CodeStack& stack = interpreter.getExecStack<CodeStack>();

    LOG(TRACE) << "Pushing list items to Exec stack in reverse order";

    for (uint16_t n = (list->cont == 0 ? list->size : list->size - 1); n > 0; --n) {
      stack.push(list->items[n - 1]);
    }

    return;
  }

  void ListType::allocateStack(Interpreter& interpreter) const
  {
    /* There's no stack for the List type */
  }

  void ListType::allocateStorage(Program& program) const
  {
    program.setStorage(type_number, std::unique_ptr<ValueStorage>(new ListStorage()));
  }

  size_t ListType::printValue(const Interpreter& interpreter, const Program& program, std::ostream& stream, uint8_t base, uintptr_t slot) const
  {
    const ListStorage& storage = program.getStorage<ListStorage>(ListType::type_number);
    size_t size = 2;
    stream << '(';
    for (;;) {
      const ListType::Value * const value = storage.getValue(base, slot);
      const uint16_t n_max = (value->cont == 0 ? value->size : value->size - 1);
      
      for (uint16_t n = 0; n < n_max; ++n) {
	const Type* type = interpreter.getType(value->items[n].type);
	size += type->printValue(interpreter, program, stream,
				 value->items[n].base, value->items[n].slot);
	if (n < n_max - 1) {
	  stream << ' ';
	  ++size;
	}
      }
      if (value->cont != 0) {
	base = value->items[value->size - 1].base;
	slot = value->items[value->size - 1].slot;
      } else {
	break;
      }
    }

    stream << ')';
    return size;
  }

  size_t ListType::randomValue(Interpreter& interpreter, Program& program, Type::Value& value, size_t max_points) const
  {
    ListStorage& storage = program.getVolatileStorage<ListStorage>(type_number);
    size_t points = 0;
    const Type* code = interpreter.getType(CodeType::type_number);
    size_t entry_max_points = max_points;
    size_t numentries = ((size_t)interpreter.random().randomInteger64Uniform()) % (max_points + 1);
    std::vector<CodeType::Value> entries(numentries);

    entry_max_points -= numentries;

    for (size_t i = 0; i < numentries; ++i) {
      size_t entry_points = code->randomValue(interpreter, program, entries[i], entry_max_points);
      if (entry_points <= entry_max_points) {
	entry_max_points -= entry_points;
      } else {
	entry_max_points = 0;
      }
      points += entry_points;
    }

    CodeType::Value randval = storage.create(entries);
    value = randval;

    return points;
  }

} /* namespace Amplify */
