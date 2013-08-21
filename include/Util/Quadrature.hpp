/*
 * Quadrature.hpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#ifndef QUADRATURE_HPP_
#define QUADRATURE_HPP_
#include "Eigen/Dense"

class Quadrature
{
public:
  Quadrature();
  virtual void Get(std::vector<float> & weights,
      std::vector<Eigen::Vector3f> & points)=0;
  virtual ~Quadrature();
};

#endif /* QUADRATURE_HPP_ */
