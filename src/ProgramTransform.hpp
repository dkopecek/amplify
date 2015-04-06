#ifndef AMPUSH_PROGRAMTRANSFORM_HPP
#define AMPUSH_PROGRAMTRANSFORM_HPP

#include <build-config.h>
#include <memory>
#include <string>
#include <vector>

namespace Amplify
{
  class Program;
  class ProgramTransform
  {
  public:
    ProgramTransform(const std::string& name)
      : _name(name)
    {
    }

    const std::string& name()
    {
      return _name;
    }

    virtual std::vector<std::shared_ptr<Program> > transform(const std::vector<std::shared_ptr<Program> >& program_set) = 0;

  private:
    const std::string _name;
  };

} /* namespace Amplify */

#endif /* AMPUSH_PROGRAMTRANSFORM_HPP */
