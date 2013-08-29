/*
 * UnitTests.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: desaic
 */

#include "../src/bin/UnitTests.hpp"
#include "Util/GaussCube.hpp"
#include <iostream>

float anaFun(Eigen::Vector3f & p){
  return p[0]*p[0] + p[1]*p[1] + p[2]*p[2];
}

void QuadratureTest()
{
  GaussCube cube;
  cube.mn = Eigen::Vector3f::Zero();
  cube.mx = 2*Eigen::Vector3f::Ones();
  cube.mx[0] = 1;
  std::vector<float> weights;
  std::vector<Eigen::Vector3f> points;
  cube.Get(weights, points);
  float integrate = 0;
  for(size_t ii = 0;ii<weights.size();ii++){
    std::cout<<weights[ii]<<" "<<points[ii][0]<<", "<<
        points[ii][1]<<", "<<points[ii][2]<<"\n";
    integrate += weights[ii] * anaFun(points[ii]);
  }
  std::cout<<"Integration: "<<integrate<<"\n";
}
