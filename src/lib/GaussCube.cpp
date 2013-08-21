/*
 * GaussCube.cpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */
#include "Util/Quadrature.hpp"
#include "Util/GaussCube.hpp"
#include <cmath>
const float quadCoef[2]=
{
    (1+1/std::sqrt(3.0)),
    (1-1/std::sqrt(3.0))
};
void
GaussCube::Get(std::vector<float> & weights,
      std::vector<Eigen::Vector3f> & points)
{

  Eigen::Vector3f size = mx-mn;
  weights.resize(8, (1.0/8) *size[0] * size[1] * size[2]);
  points.resize(8);
  size = 0.5*size;

  points[0] = Eigen::Vector3f(quadCoef[0],quadCoef[0],quadCoef[0]);
  points[1] = Eigen::Vector3f(quadCoef[1],quadCoef[0],quadCoef[0]);
  points[2] = Eigen::Vector3f(quadCoef[0],quadCoef[1],quadCoef[0]);
  points[3] = Eigen::Vector3f(quadCoef[1],quadCoef[1],quadCoef[0]);

  points[4] = Eigen::Vector3f(quadCoef[0],quadCoef[0],quadCoef[1]);
  points[5] = Eigen::Vector3f(quadCoef[1],quadCoef[0],quadCoef[1]);
  points[6] = Eigen::Vector3f(quadCoef[0],quadCoef[1],quadCoef[1]);
  points[7] = Eigen::Vector3f(quadCoef[1],quadCoef[1],quadCoef[1]);

  for(size_t ii = 0;ii<points.size();ii++){
    points[ii] = (points[ii].array() * size.array()).matrix() + mn;
  }

}

GaussCube::GaussCube()
{}

GaussCube::~GaussCube()
{
}

