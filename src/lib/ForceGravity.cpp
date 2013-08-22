/*
 * ForceGravity.cpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#include "World/ForceVolume.hpp"
#include "World/ForceGravity.hpp"
#include "Element/Element.hpp"
float
ForceGravity::GetEnergyDensity(const Eigen::Vector3f & p)
{
  return element->density*p.dot(g);
}

Eigen::Vector3f
ForceGravity::GetForce(const Eigen::Vector3f & X)
{
  return element->density*g;
}

ForceGravity::ForceGravity()
{}

ForceGravity::~ForceGravity()
{}

