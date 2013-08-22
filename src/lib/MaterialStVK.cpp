/*
 * MaterialStVK.cpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#include "Element/MaterialStVK.hpp"
#include "Eigen/Dense"
Eigen::Matrix3f
MaterialStVK::GetPK1(const Eigen::Matrix3f & F)
{
  Eigen::Matrix3f I = Eigen::Matrix3f::Identity();
  Eigen::Matrix3f E = 0.5*(F.transpose()*F - I);
  Eigen::Matrix3f P = F*(2*mu*E + lambda*E.trace() * I);
  return P;
}

float
MaterialStVK::GetEnergy(const Eigen::Matrix3f & F)
{
  Eigen::Matrix3f I = Eigen::Matrix3f::Identity();
  Eigen::Matrix3f E = 0.5*(F.transpose()*F - I);
  return mu * E.squaredNorm() + 0.5*lambda*E.trace();
}

MaterialStVK::MaterialStVK():
mu(1),lambda(0.4)
{}

MaterialStVK::~MaterialStVK()
{
  // TODO Auto-generated destructor stub
}

