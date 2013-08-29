/*
 * MaterialModel.hpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#ifndef MATERIALMODEL_HPP_
#define MATERIALMODEL_HPP_
#include "Eigen/Dense"
class MaterialModel
{
public:
  MaterialModel();
  virtual Eigen::Matrix3f GetPK1(const Eigen::Matrix3f & F)=0;
  virtual float GetEnergy(const Eigen::Matrix3f & F)=0;
  virtual ~MaterialModel();
};

#endif /* MATERIALMODEL_HPP_ */
