/*
 * ForceGravity.cpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#include "World/ForceVolume.hpp"
#include "World/ForceGravity.hpp"

Eigen::Vector3f
ForceGravity::GetForce(const Eigen::Vector3f & X)
{
  return g;
}

ForceGravity::ForceGravity()
{}

ForceGravity::~ForceGravity()
{}

