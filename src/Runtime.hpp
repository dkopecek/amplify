#ifndef AMPUSH_RUNTIME_HPP
#define AMPUSH_RUNTIME_HPP

#include <build-config.h>
#include "Program.hpp"
#include "Interpreter.hpp"
#include "CodeType.hpp"
#include "ListType.hpp"
#include "IntegerType.hpp"
#include "NameType.hpp"
#include "BooleanType.hpp"
#include "FloatType.hpp"
#include "StringType.hpp"

#include <istream>

namespace Amplify
{
  class Runtime
  {
  public:
    Runtime();

    void loadFromFile(const std::string& path);
    void loadFromStream(std::istream& stream);
    void parseFromStream(std::istream& stream);
    void compileToFile(const std::string& path);
    void compileToStream(std::ostream& stream);

    const Program& program() const;
    Interpreter& interpreter();
    void addType(const Type* type);

    void run();
    void printStacks();

  protected:
    void loadTypeMetadata(std::istream& stream, size_t type_count, std::vector<uint8_t>& type_numbers);
    void loadTypeData(std::istream& stream, size_t type_count, const std::vector<uint8_t>& type_numbers);

  private:
    const uint8_t AmPush_file_magic[8] = { 0x00, 'A', 'm', 'P', 'u', 's', 'h', 0x00 };

    struct FileHeader {
      uint8_t magic[sizeof AmPush_file_magic]; /* \x00 'A' 'm' 'P' 'u' 's' 'h' \x00 */
      uint8_t version; /* \x00 */
      uint8_t type_count; /* number of types used in the program */
      uint32_t size; /* total size of the file without the header */
    } __attribute__((packed));

    struct TypeMetadata {
      uint8_t type_number;
      uint8_t __pad0; /* \x00 */
      uint8_t __pad1; /* \x00 */
      uint8_t type_string_size;
      uint8_t type_string[32];
    } __attribute__((packed));

    struct TypeData {
      uint8_t type_number;
      uint8_t __pad0; /* \x00 */
      uint16_t __pad1; /* \x00 \x00 */
      uint32_t size;
    } __attribute__((packed));

    CodeType _code_type;
    ListType _list_type;
    IntegerType _integer_type;
    NameType _name_type;
    BooleanType _boolean_type;
    FloatType _float_type;
    StringType _string_type;

    Program _program;
    Interpreter _interpreter;
  };
} /* namespace Amplify */

#endif /* AMPUSH_HPP */
