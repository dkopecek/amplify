#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <build-config.h>
#include <sstream>

namespace Amplify
{
  template<class C, typename T>
  bool contains(C container, const T& value)
  {
    for (auto const& item : container) {
      if (item == value) {
	return true;
      }
    }
    return false;
  }

  /**
   * Convert a string representation of a number
   * to a number of type T.
   */
  template<typename T>
  T stringToNumber(const std::string& string_value)
  {
    std::istringstream string_stream(string_value);
    T value;
    string_stream >> value;
    return value;
  }

  /**
   * Tokenize a std::string compatible type using delimiters specified in a string.
   *
   * Splits the given string using delimiters found in the delimiter string (each
   * character in the delimiter string is considered as a delimiter) and stores the
   * token in a vector. If trim_empty is set to true, empty tokens won't be included
   * in the vector.
   */
  template<typename StringType>
  void tokenizeString(const StringType& str, std::vector<StringType>& tokens, const typename std::vector<StringType>::value_type delimiters, bool trim_empty = false)
  {
    typename StringType::size_type pos, last_pos = 0;
    while(true) {
      pos = str.find_first_of(delimiters, last_pos);
      if(pos == StringType::npos) {
	pos = str.length();

	if(pos != last_pos || !trim_empty) {
	  tokens.push_back(StringType(str.data() + last_pos, pos - last_pos));
	}

	break;
      } else {
	if(pos != last_pos || !trim_empty) {
	  tokens.push_back(StringType(str.data() + last_pos, pos - last_pos));
	}
      }
      last_pos = pos + 1;
    }
  }

  /**
   * Return the filename part of a path. If include_extension is set to
   * false, then any characters after the last dot character '.' will be
   * removed including the dot.
   *
   * Examples: "/foo/bar/baz" => "baz"
   *           "/foo/bar/baz.txt" (include_extension=false) => "baz"
   *           "/foo/bar/baz.txt" (include_extension=true) => "baz.txt"
   *           "/foo/bar/baz.woo.txt" (include_extension=false) => "baz.woo"
   *           "foo.txt" (include_extension=true) => "foo.txt"
   */
  static std::string filenameFromPath(const std::string& filepath, bool include_extension = false)
  {
    const std::string directory_separator = "/";
    std::vector<std::string> path_tokens;

    tokenizeString(filepath, path_tokens, directory_separator);

    if (path_tokens.size() == 0) {
      return std::string();
    }

    const std::string& filename = path_tokens.back();

    if (include_extension) {
      return std::move(filename);
    }

    const size_t substr_to = filename.find_last_of('.');

    return std::move(filename.substr(0, substr_to));
  }

};

#endif /* UTILITY_HPP */
