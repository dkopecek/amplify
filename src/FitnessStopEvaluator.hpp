#ifndef AMPUSH_FITNESSSTOPEVALUATOR_HPP
#define AMPUSH_FITNESSSTOPEVALUATOR_HPP

#include <build-config.h>

namespace Amplify
{
  class FitnessStopEvaluator
  {
  public:
    FitnessStopEvaluator();
    virtual bool evaluate() const = 0;
  private:
  };
} /* namespace Amplify */

#endif /* AMPUSH_FITNESSSTOPEVALUATOR_HPP */
