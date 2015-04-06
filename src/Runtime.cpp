#include <build-config.h>
#include "Runtime.hpp"
#include "Utility.hpp"
#include "Lexer.hpp"
#include "CodeType.hpp"
#include "ListType.hpp"
#include "IntegerType.hpp"
#include "NameType.hpp"
#include "BooleanType.hpp"
#include "FloatType.hpp"
#include "StringType.hpp"
#include "Logging.hpp"

#include <fstream>
#include <cstring>

namespace Amplify
{
  #include "Parser.c"

  Runtime::Runtime()
  {
    addType(&_code_type);
    addType(&_list_type);
    addType(&_integer_type);
    addType(&_name_type);
    addType(&_boolean_type);
    addType(&_float_type);
    addType(&_string_type);
    _interpreter.registerInstructions();
  }

  void Runtime::loadFromFile(const std::string& path)
  {
    std::ifstream program_file_stream;

    program_file_stream.open(path, std::ios::binary);

    if (!program_file_stream.is_open()) {
      throw std::runtime_error("Can't open file " + path);
    }

    loadFromStream(program_file_stream);
  }

  void Runtime::loadFromStream(std::istream& stream)
  {
    FileHeader header;

    stream.read(reinterpret_cast<char *>(&header), sizeof(FileHeader));

    if (!stream) {
      /*
       * Not a compiled AmPush program. Might still parseable.
       */
      stream.clear();
      stream.seekg(0);
      parseFromStream(stream);
      return;
    }

    if (memcmp(header.magic, AmPush_file_magic,
	       sizeof header.magic) != 0) {
      /*
       * Definitely not a compiled AmPush program.
       */
      stream.clear();
      stream.seekg(0);
      parseFromStream(stream);
      return;
    }

    if (header.version != 0x00) {
      /*
       * Unknown version. Might be parseable.
       */
      stream.clear();
      stream.seekg(0);
      parseFromStream(stream);
      return;
    }

    size_t type_count = header.type_count;
    size_t program_size = header.size;
    std::vector<uint8_t> type_numbers;

    loadTypeMetadata(stream, type_count, type_numbers);
    loadTypeData(stream, type_count, type_numbers);

    _program.setCode(CodeType::Value(CodeType::type_number, AMPUSH_BASE_STORAGE, 0));

    return;
  }

  static void AmPushParserDeleter(void *p)
  {
    AmPushParserFree(p, &free);    
  }

  void Runtime::parseFromStream(std::istream& stream)
  {
    quex::Lexer lexer(&stream);
    quex::Token *token_ptr = NULL;
    std::unique_ptr<void,void(*)(void *)> parser_data(AmPushParserAlloc(&malloc),
						      AmPushParserDeleter);
    for (;;) {
      lexer.receive(&token_ptr);

      LOG(DEBUG) << "Token: " << token_ptr->type_id_name();

      if (token_ptr->type_id() != AMPUSH_TOKEN_TERMINATION) {
        AmPushParser(parser_data.get(),
		     token_ptr->type_id(), new QUEX_TYPE_TOKEN(*token_ptr), &_program);
      } else {
        AmPushParser(parser_data.get(), 0, NULL, &_program);
        break;
      }
    } /* token loop */
    return;
  }

  void Runtime::compileToFile(const std::string& path)
  {
    std::ofstream program_file_stream;

    program_file_stream.open(path, std::ios::binary|std::ios::trunc);

    if (!program_file_stream.is_open()) {
      throw std::runtime_error("Can't open file " + path);
    }

    compileToStream(program_file_stream);
  }

  void Runtime::compileToStream(std::ostream& stream)
  {
    FileHeader header;

    memset(&header, 0, sizeof header);
    memcpy(header.magic, AmPush_file_magic, sizeof header.magic);
    header.version = 0;
    header.type_count = _program.typeCount();
    header.size = 0;

    // Write header (without size)
    stream.write(reinterpret_cast<const char *>(&header), sizeof header);
    LOG(DEBUG) << "Wrote program header: size=" << sizeof(header);

    size_t program_size = 0;

    // Write typemetadata
    for (size_t t = 0; t < header.type_count; ++t) {
      if (!_program.typeExists(t)) {
	continue;
      }

      TypeMetadata type_metadata;
      const Type* type = _interpreter.getType(t);

      memset(&type_metadata, 0, sizeof type_metadata);
      type_metadata.type_number = t;

      const std::string& type_string = type->getTypeString();

      type_metadata.type_string_size = type_string.size();
      memcpy(type_metadata.type_string, type_string.c_str(), type_string.size());
      stream.write(reinterpret_cast<const char *>(&type_metadata), sizeof type_metadata);

      program_size += sizeof(TypeMetadata);
      LOG(DEBUG) << "Wrote type metadata: ts=" << type_string << ", tn=" << (int)t;
    }
    // Write type data
    for (size_t t = 0; t < header.type_count; ++t) {
      if (!_program.typeExists(t)) {
	continue;
      }

      TypeData type_data;
      memset(&type_data, 0, sizeof type_data);
      type_data.type_number = t;
      type_data.size = _program.binaryValuesSize(t);

      /* FIXME: Doesn't work because loadTypeData expects the sections to be there
      if (type_data.size == 0) {
	continue;
      }
      */

      stream.write(reinterpret_cast<const char *>(&type_data), sizeof type_data);
      _program.saveBinaryValues(stream, t);
      program_size += sizeof(TypeData) + type_data.size;
      LOG(DEBUG) << "Wrote type data: tn=" << (int)t << ", size=" << type_data.size << "B";
    }
    // Write header (with size)
    stream.seekp(0);
    header.size = program_size;
    stream.write(reinterpret_cast<const char *>(&header), sizeof header);
    LOG(DEBUG) << "Program size: " << program_size + sizeof(header);
    return;
  }

  const Program& Runtime::program() const
  {
    return _program;
  }

  Interpreter& Runtime::interpreter()
  {
    return _interpreter;
  }

  void Runtime::addType(const Type* type)
  {
    _program.addType(type);
    _interpreter.addType(type);
  }

  void Runtime::run()
  {
    _interpreter.run(_program);
    return;
  }

  void Runtime::printStacks()
  {
    _interpreter.printStacks(_program);
    return;
  }

  void Runtime::loadTypeMetadata(std::istream& stream, size_t type_count, std::vector<uint8_t>& type_numbers)
  {
    TypeMetadata metadata;

    for (size_t n = 0; n < type_count; ++n) {
      stream.read(reinterpret_cast<char *>(&metadata), sizeof(TypeMetadata));
      if (!stream) {
	throw std::runtime_error("stream error while reading type metadata");
      }
      /* TODO: Check type_string/type_number agains supported types */
      type_numbers.push_back(metadata.type_number);
    }
    return;
  }

  void Runtime::loadTypeData(std::istream& stream, size_t type_count, const std::vector<uint8_t>& type_numbers)
  {
    TypeData header;

    for (size_t n = 0; n < type_count; ++n) {
      stream.read(reinterpret_cast<char *>(&header), sizeof(TypeData));

      if (!stream) {
	throw std::runtime_error("stream error while reading type data");
      }

      if (!contains(type_numbers, header.type_number)) {
	throw std::runtime_error("invalid type data section, unknown type number");
      }

      /* TODO: check that data size is sane */
      /* NOTE: Consider using mmap so as not to allocate too much memory */
      //std::unique_ptr<uint8_t> data(new uint8_t[size]);
      //stream.read(reinterpret_cast<char *>(data.get()), size);
      const size_t size = header.size;
      _program.loadBinaryValues(stream, header.type_number, size);
    }

    return;
  }

} /* namespace Amplify */
