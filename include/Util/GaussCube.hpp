/*
 * GaussCube.hpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */

#ifndef GAUSSCUBE_HPP_
#define GAUSSCUBE_HPP_

#include "Eigen/Dense"
class Quadrature;
class GaussCube: public Quadrature
{
public:
  GaussCube();
  void Get(std::vector<float> & weights,
      std::vector<Eigen::Vector3f> & points);
  virtual ~GaussCube();
  Eigen::Vector3f mn, mx;
};

#endif /* GAUSSCUBE_HPP_ */
