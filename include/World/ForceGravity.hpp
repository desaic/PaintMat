/*
 * ForceGravity.hpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#ifndef FORCEGRAVITY_HPP_
#define FORCEGRAVITY_HPP_

#include "Eigen/Dense"

class ForceVolume;

class ForceGravity: public ForceVolume
{
public:
  ForceGravity();
  Eigen::Vector3f GetForce(const Eigen::Vector3f & X);
  float GetEnergyDensity(const Eigen::Vector3f & p);
  virtual ~ForceGravity();
  Eigen::Vector3f g;
};

#endif /* FORCEGRAVITY_HPP_ */
