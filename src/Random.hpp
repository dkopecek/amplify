#ifndef AMPUSH_RANDOM_HPP
#define AMPUSH_RANDOM_HPP

#include <build-config.h>
#include <random>

namespace Amplify
{
  class Random
  {
  public:
    Random()
      : _normal_name(6, 2)
    {
      std::random_device rng;
      _engine.seed(rng());
    }

    int64_t randomInteger64()
    {
      return _uniform_int64(_engine);
    }

    double randomFloat()
    {
      return _uniform_float(_engine);
    }

    bool randomBoolean()
    {
      return _bernoulli(_engine);
    }

    size_t randomNameLength()
    {
      size_t randval = (size_t)_normal_name(_engine);
      if (randval < 1) {
	return 1;
      }
      if (randval > 64) {
	return 64;
      }
      return randval;
    }

    int64_t randomInteger64Uniform()
    {
      return _uniform_int64_nontype(_engine);
    }

    uint8_t randomType()
    {
      return _discrete_type(_engine);
    }

  private:
    std::default_random_engine _engine;
    std::uniform_int_distribution<int64_t> _uniform_int64;
    std::uniform_real_distribution<double> _uniform_float;
    std::bernoulli_distribution _bernoulli;
    std::normal_distribution<double> _normal_name;
    std::uniform_int_distribution<int64_t> _uniform_int64_nontype;
    std::discrete_distribution<uint8_t> _discrete_type;
  };

} /* namespace Amplify */
#endif /* AMPUSH_RANDOM_HPP */
