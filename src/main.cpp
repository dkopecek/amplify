#include <build-config.h>
#include "Runtime.hpp"
#include "Logging.hpp"
#include "Utility.hpp"

#include <iostream>
#include <getopt.h>

using namespace Amplify;

AMPUSH_LOGGING_INIT;

void print_usage(std::ostream& stream, const std::string& arg0)
{
  const std::string basename = filenameFromPath(arg0, /*include_extension=*/true);
  stream << std::endl;
  stream << "Usage: " << basename << " <program>" << std::endl;
  stream << "       " << basename << " compile <program> <output>" << std::endl;
  stream << std::endl;
}

int main(int argc, char *argv[])
{
  const std::string arg0 = argv[0];
  Amplify::Runtime runtime;

  if (argc == 2) {
    runtime.loadFromFile(argv[1]);
    runtime.run();
    runtime.printStacks();
    return EXIT_SUCCESS;
  }
  else if (argc == 4 && strcmp(argv[1], "compile") == 0) {
    runtime.loadFromFile(argv[2]);
    runtime.compileToFile(argv[3]);
    return EXIT_SUCCESS;
  }
  else {
    print_usage(std::cerr, arg0);
    return EXIT_FAILURE;
  }
}
