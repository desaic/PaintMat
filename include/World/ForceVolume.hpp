/*
 * ForceVolume.hpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#ifndef FORCEVOLUME_HPP_
#define FORCEVOLUME_HPP_

#include "Eigen/Dense"
class Element;
///@brief Volumetric force
class ForceVolume
{
public:
  ForceVolume();
  virtual Eigen::Vector3f GetForce(const Eigen::Vector3f & p)=0;
  virtual float GetEnergyDensity(const Eigen::Vector3f & p)=0;
  virtual ~ForceVolume();
  Element * element;
};

#endif /* FORCEVOLUME_HPP_ */
