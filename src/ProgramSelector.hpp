#ifndef AMPUSH_PROGRAMSELECTOR_HPP
#define AMPUSH_PROGRAMSELECTOR_HPP

#include <build-config.h>
#include <memory>
#include <string>
#include <vector>

namespace Amplify
{
  class Program;
  class ProgramSelector
  {
  public:
    ProgramSelector(const std::string& name)
      : _name(name)
    {
    }

    const std::string& name()
    {
      return _name;
    }

    virtual std::shared_ptr<Program> select(const std::vector<std::shared_ptr<Program> >& population) = 0;

  private:
    const std::string _name;
  };

} /* namespace Amplify */

#endif /* AMPUSH_PROGRAMSELECTOR_HPP */
