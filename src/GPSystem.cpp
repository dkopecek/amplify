#include <build-config.h>
#include "GPSystem.hpp"

namespace Amplify
{
  GPSystem::GPSystem()
  {
    _generation_number = 0;
    setMaxPopulationSize(100);
    setState(GPSystem::Constructed);
  }

  void GPSystem::setMaxPopulationSize(size_t size)
  {
    if (!_population[0].empty() ||
	!_population[1].empty()) {
      throw std::runtime_error("Cannot change population size of an active instance");
    }
    _max_population_size = size;
    return;
  }

  size_t GPSystem::getMaxPopulationSize() const
  {
    return _max_population_size;
  }

  size_t GPSystem::getGenerationNumber() const
  {
    return _generation_number;
  }

  void GPSystem::setInputGenerator(UniquePointer<InputGenerator>& generator)
  {
    return;
  }

  void GPSystem::setFitnessCaseEvaluator(UniquePointer<FitnessCaseEvaluator>& case_evaluator)
  {
    return;
  }

  void GPSystem::setFitnessStopEvaluator(UniquePointer<FitnessStopEvaluator>& stop_evaluator)
  {
    return;
  }

  void GPSystem::addGenerationOperator(PointerVector<ProgramSelector> selector,
				       double population_percentage,
				       PointerVector<ProgramTransform> transform)
  {
  }

  GPSystem::State GPSystem::getState() const
  {
    return _state;
  }

  void GPSystem::setState(State state)
  {
    _state = state;
    return;
  }

  void GPSystem::generateFirstPopulation()
  {
    //
    setState(GPSystem::Initialized);
  }

  void GPSystem::generateNextPopulation()
  {
    /*
     * Give the generation operators a change to prepare for
     * creating a new generation of programs.
     *
     * This way the operators can implement per-generation states.
     */
    for (auto gop : _generation_operators) {
      gop->initForGeneration(_generation_number);
    }

    /* Evaluate the operators (may run in parallel?) */
    {
      std::vector<PointerVector<Program> > sub_populations(_generation_operators.size());
      size_t i = 0;

      for (auto gop : _generation_operators) {
	gop->evalForGeneration(_generation_number, _population[0], sub_populations[i]);
	++i;
      }

      /*
       * Merge subpopulations into one
       */
      _population[1].clear();
      for (auto const& sub : sub_populations) {
	_population[1].insert(_population[1].end(), sub.begin(), sub.end());
      }
    }

    /*
     * Finalize generation operators. Gives the operatos a chance
     * to release per-generation resources.
     */
    for (auto gop : _generation_operators) {
      gop->finiForGeneration(_generation_number);
    }

    setState(GPSystem::Running);

    return;
  }

  GPSystem::StopReason GPSystem::run()
  {    
  }
} /* namespace Amplify */
