#ifndef AMPUSH_FITNESSCASEEVALUATOR_HPP
#define AMPUSH_FITNESSCASEEVALUATOR_HPP

#include <build-config.h>

namespace Amplify
{
  class FitnessCaseEvaluator
  {
  public:
    FitnessCaseEvaluator();
    virtual double evaluate() const = 0;
  private:
  };
} /* namespace Amplify */

#endif /* AMPUSH_FITNESSCASEEVALUATOR_HPP */
