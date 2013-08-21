/*
 * GaussCube.cpp
 *
 *  Created on: Aug 21, 2013
 *      Author: desaic
 */
#include "Util/Quadrature.hpp"
#include "Util/GaussCube.hpp"

void
GaussCube::Get(std::vector<float> & weights,
      std::vector<Eigen::Vector3f> & points)
{
  Eigen::Vector3f size = mx-mn;
  weights.resize(8, (1.0/8) *size[0] * size[1] * size[2]);
  points.resize(8);

}

GaussCube::GaussCube()
{}

GaussCube::~GaussCube()
{
}

