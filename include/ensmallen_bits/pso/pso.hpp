/**
 * @file pso.hpp
 * @author Adeel Ahmad
 *
 * Definition of the Particle Swarm Optimizer as proposed
 * by J. Kennedy et al. in "Particle swarm optimization".
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef ENSMALLEN_PSO_PSO_HPP
#define ENSMALLEN_PSO_PSO_HPP

#include "inertia_weight.hpp"
#include "constriction_factor.hpp"

namespace ens {

/**
 * PSO - Particle swarm optimization is a method based on  the social behavior
 * of bird flocks when moving from one place to another and was proposed
 * mainly to solve numerical optimization problems. PSO with intertia weight
 * variant is implemented in the code below.
 *
 * For more information, please refer to:
 *
 * @code
 * @article{Kennedy1995
 *   author    = {J. Kennedy and R. Eberhart},
 *   title     = {Particle swarm optimization},
 *   volume    = {4},
 *   number    = {},
 *   year      = {1995},
 *   pages     = {1942-1948 vol.4},
 *   publisher = {},
 * }
 * @endcode
 *
 * For PSO to work, the class must implement the following function:
 *
 *   double Evaluate(const arma::mat& coordinates);
 *
 * Evaluate(coordinates) will evaluate the objective function using
 * coordinates as the dataset.
 *
 * @tparam SelectionPolicy The velocty / position update strategy used for the
 *     evaluation step.
 */
template<typename VelocityVectorType = InertiaWeight>
class PSOType
{
 public:
  /**
   * Construct the particle swarm optimizer with the given function
   * and parameters.
   *
   * @param dimension Dimension of the search space i.e. number
   *     of particles.
   * @param interiaWeight Inertia weight of the particles (omega).
   * @param cognitiveAcceleration Cognitive acceleration of the particles.
   * @param socialAcceleration Social acceleration of the particles.
   * @param maxIterations Maximum number of iterations allowed (0 means no
   *     limit).
   * @param tolerance Maximum absolute tolerance to terminate the algorithm.
   * @param velocityType Velocity update policy used to calculate the
   *     objective.
   */
  PSOType(const size_t dimension = 10,
          const double interiaWeight = 0.9,
          const double cognitiveAcceleration = 0.5,
          const double socialAcceleration = 0.3,
          const size_t maxIterations = 200,
          const double tolerance = 1e-5,
          const VelocityVectorType& velocityType = VelocityVectorType());

  /**
   * Optimize the given function using PSO. The given starting point will be
   * modified to store the finishing point of the algorithm, and the final
   * objective value is returned.
   *
   * @tparam DecomposableFunctionType Type of the function to be optimized.
   * @param function Function to optimize.
   * @param iterate Starting point for the particles (will be modified).
   * @return Objective value of the final point.
   */
  template<typename DecomposableFunctionType>
  double Optimize(DecomposableFunctionType& function, arma::mat& iterate);

  //! Get the dimension of the search space.
  size_t PopulationSize() const { return dimension; }
  //! Modify the dimension of the search space.
  size_t& PopulationSize() { return dimension; }

  //! Get the intertia weight of particles.
  double InteriaWeight() const { return interiaWeight; }
  //! Modify the interia weight of particles.
  double& InteriaWeight() { return interiaWeight; }

  //! Get the cognitive acceleration of particles.
  double CognitiveAcceleration() const { return cognitiveAcceleration; }
  //! Modify the cognitive acceleration of particles.
  double& CognitiveAcceleration() { return cognitiveAcceleration; }

  //! Get the social acceleration of particles.
  double SocialAcceleration() const { return socialAcceleration; }
  //! Modify the social acceleration of particles.
  double& SocialAcceleration() { return socialAcceleration; }

  //! Get the maximum number of iterations (0 indicates no limit).
  size_t MaxIterations() const { return maxIterations; }
  //! Modify the maximum number of iterations (0 indicates no limit).
  size_t& MaxIterations() { return maxIterations; }

  //! Get the tolerance for termination.
  double Tolerance() const { return tolerance; }
  //! Modify the tolerance for termination.
  double& Tolerance() { return tolerance; }

  //! Get the velocity type.
  const VelocityVectorType& VelocityType() const { return velocityType; }
  //! Modify the velocity type.
  VelocityVectorType& VelocityType() { return velocityType; }

 private:
  //! Dimension of the search space i.e. number of particles.
  size_t dimension;

  //! Position of the particles.
  arma::cube particlePosition;

  //! Velocity of the particles.
  arma::cube particleVelocity;

  //! Best position of the swarm.
  arma::mat bestSwarmPosition;

  //! Inertia weight of the particles (omega).
  double interiaWeight;

  //! Cognitive acceleration of the particles.
  double cognitiveAcceleration;

  //! Social acceleration of the particles.
  double socialAcceleration;

  //! The maximum number of allowed iterations.
  size_t maxIterations;

  //! The tolerance for termination.
  double tolerance;

  //! The velocity update policy used.
  VelocityVectorType velocityType;
};

// Define an alias to PSOType.
using PSO = PSOType<InertiaWeight>;
using ConstrictionPSO = PSOType<ConstrictionFactor>;

} // namespace ens

// Include implementation.
#include "pso_impl.hpp"

#endif // ENSMALLEN_PSO_PSO_HPP