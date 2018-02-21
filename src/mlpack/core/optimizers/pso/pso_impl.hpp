/**
 * @file pso_impl.hpp
 * @author Adeel Ahmad
 *
 * Implementation of the Particle Swarm Optimizer as proposed
 * by J. Kennedy et al. in "Particle swarm optimization".
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_CORE_OPTIMIZERS_PSO_PSO_IMPL_HPP
#define MLPACK_CORE_OPTIMIZERS_PSO_PSO_IMPL_HPP

// In case it hasn't been included yet.
#include "pso.hpp"

namespace mlpack {
namespace optimization {

template<typename VelocityVectorType>
PSOType<VelocityVectorType>::PSOType(const size_t dimension,
                                  const double interiaWeight,
                                  const double cognitiveAcceleration,
                                  const double socialAcceleration,
                                  const size_t maxIterations,
                                  const double tolerance,
                                  const VelocityVectorType& velocityType) :
    dimension(dimension),
    interiaWeight(interiaWeight),
    cognitiveAcceleration(cognitiveAcceleration),
    socialAcceleration(socialAcceleration),
    maxIterations(maxIterations),
    tolerance(tolerance),
    velocityType(velocityType)
{ /* Nothing to do. */ }

//! Optimize the function (minimize).
template<typename VelocityVectorType>
template<typename DecomposableFunctionType>
double PSOType<VelocityVectorType>::Optimize(
    DecomposableFunctionType& function, arma::mat& iterate)
{
  arma::arma_rng::set_seed_random();
  // Set size for particle position and velocity.
  particlePosition.set_size(iterate.n_rows, iterate.n_cols, dimension);
  particleVelocity.set_size(iterate.n_rows, iterate.n_cols, dimension);

  // Initialize particle positions and velocities with the given points.
  for (size_t i = 0; i < dimension; ++i)
  {
    particlePosition.slice(i) = iterate;
    particleVelocity.slice(i) = iterate;
  }

  // Best particle and swarm position values are initialized
  // from the first particle.
  bestParticlePosition = particlePosition.slice(0);
  bestSwarmPosition = particlePosition.slice(0);

  // Convenient variables to check if there's an improvement.
  double currentObjective;
  double lastObjectiveIndividual = DBL_MAX;
  double lastObjectiveGlobal = DBL_MAX;

  // Start iterating.
  for (size_t i = 0; i < maxIterations; ++i)
  {
    for (size_t k = 0; k < dimension; ++k)
    {
      // Calculate the objective function.
      currentObjective = function.Evaluate(particlePosition.slice(k));

      // Check if the current position is an individual best.
      if (currentObjective < lastObjectiveIndividual)
      {
        bestParticlePosition = particlePosition.slice(k);
        lastObjectiveIndividual = currentObjective;
      }
    }

    // Check if the current position is a global best.
    if (lastObjectiveIndividual < lastObjectiveGlobal)
    {
      bestSwarmPosition = bestParticlePosition;
      lastObjectiveGlobal = lastObjectiveIndividual;
    }

    // Update velocity for each particle.
    velocityType.Update(particlePosition, particleVelocity,
      bestParticlePosition, bestSwarmPosition, interiaWeight,
      cognitiveAcceleration, socialAcceleration, dimension);

    // Update position for each particle.
    particlePosition = particlePosition + particleVelocity;

    // Compare current objective with tolerance.
    if (currentObjective < tolerance)
    {
      Log::Info << "PSO: minimized within tolerance " << tolerance << "; "
          << "terminating optimization." << std::endl;
      return currentObjective;
    }
  }
  return lastObjectiveGlobal;
}

} // namespace optimization
} // namespace mlpack

#endif