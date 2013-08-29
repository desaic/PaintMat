/*
 * MaterialStVK.hpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#ifndef MATERIALSTVK_HPP_
#define MATERIALSTVK_HPP_

#include "MaterialModel.hpp"

class MaterialStVK: public MaterialModel
{
public:
  MaterialStVK();
  Eigen::Matrix3f GetPK1(const Eigen::Matrix3f & F);
  float GetEnergy(const Eigen::Matrix3f & F);
  float mu, lambda;
  virtual ~MaterialStVK();
};

#endif /* MATERIALSTVK_HPP_ */
