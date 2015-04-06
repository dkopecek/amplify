#ifndef AMPUSH_GPSYSTEM_HPP
#define AMPUSH_GPSYSTEM_HPP

#include <build-config.h>

#include "ProgramSelector.hpp"
#include "ProgramTransform.hpp"
#include "InputGenerator.hpp"
#include "FitnessCaseEvaluator.hpp"
#include "FitnessStopEvaluator.hpp"
#include "Typedefs.hpp"
#include "Runtime.hpp"

#include <cstddef>

namespace Amplify
{
  class GPSystem
  {
  public:
    typedef void(*InputFunc)();
    typedef double(*FitnessFunc)();
    typedef bool(*FitnessStopFunc)();

    enum StopReason {
      SizeLimit,
      OptimalFitness,
      Interrupted,
      ResourceLimit
    };

    GPSystem();

    void setMaxPopulationSize(size_t size);
    size_t getMaxPopulationSize() const;
    size_t getGenerationNumber() const;

    void setInputGenerator(UniquePointer<InputGenerator>& generator);
    void setFitnessCaseEvaluator(UniquePointer<FitnessCaseEvaluator>& case_evaluator);
    void setFitnessStopEvaluator(UniquePointer<FitnessStopEvaluator>& stop_evaluator);

    void addGenerationOperator(PointerVector<ProgramSelector> selector,
			       double population_percentage,
			       PointerVector<ProgramTransform> transform);

    StopReason run();

  protected:
    enum State
    {
      Constructed, /* state after the constructor completes */
      Initialized, /* parameters loaded, first population generated */
      Running, /* 2nd, 3rd, ... nth generation */
      Finished /* Stopping criteria reached */
    };

    State getState() const;
    void setState(State state);

    struct GenerationOperator
    {
      PointerVector<ProgramSelector> selector;
      PointerVector<ProgramTransform> transform;
      double population_percentage;

      void initForGeneration(size_t generation_number);
      void finiForGeneration(size_t generation_number);
      void evalForGeneration(size_t generation_number,
			     const PointerVector<Program>& old_population,
			     PointerVector<Program>& new_population);
    };

    void generateFirstPopulation();
    void generateNextPopulation();

  private:
    State _state;
    Runtime _runtime;
    // Runtime
    // Input generator
    // Fitness case evaluation function
    // Fitness stop evaluation function
    // Generation function
    // \_ array of selectors + transform operators
    std::vector<GenerationOperators> _generation_operators;
    size_t _generation_number;
    size_t _max_population_size;
    PointerVector<Program> _population[2];
  };

} /* namespace Amplify */

#endif /* AMPUSH_GPSYSTEM_HPP */
