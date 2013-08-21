/*
 * ForceVolume.hpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#ifndef FORCEVOLUME_HPP_
#define FORCEVOLUME_HPP_

#include "Eigen/Dense"
///@brief Volumetric force
class ForceVolume
{
public:
  ForceVolume();
  virtual Eigen::Vector3f GetForce(const Eigen::Vector3f & p)=0;
  virtual ~ForceVolume();
};

#endif /* FORCEVOLUME_HPP_ */
