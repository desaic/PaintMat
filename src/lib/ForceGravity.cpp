/*
 * ForceGravity.cpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#include "Element/ForceVolume.hpp"
#include "Element/ForceGravity.hpp"

Eigen::Vector3f
ForceGravity::GetForce(const Eigen::Vector3f & X)
{
  return g;
}

ForceGravity::ForceGravity()
{}

ForceGravity::~ForceGravity()
{}

