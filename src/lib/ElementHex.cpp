/*
 * ElementHex.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */

#include "Element/ElementHex.hpp"

std::vector<float>
ElementHex::InterpWeights(const Eigen::Vector3f & X,
        const std::vector<Eigen::Vector3f> & nodes)
{
  std::vector<float> weights(8);

  //min and max node
  Eigen::Vector3f N1, N2;
  N1 = nodes[GetNodeIndex(0)];
  N2 = nodes[GetNodeIndex(6)];

  float xx = 2*(X[0] - N1[0])/(N2[0] - N1[0]) - 1;
  float yy = 2*(X[1] - N1[1])/(N2[1] - N1[1]) - 1;
  float zz = 2*(X[2] - N1[2])/(N2[2] - N1[2]) - 1;

  weights[0] = (1/8) * (1-xx) * (1-yy) * (1-zz) ;
  weights[1] = (1/8) * (1+xx) * (1-yy) * (1-zz) ;
  weights[2] = (1/8) * (1+xx) * (1+yy) * (1-zz) ;
  weights[3] = (1/8) * (1-xx) * (1+yy) * (1-zz) ;

  weights[4] = (1/8) * (1-xx) * (1-yy) * (1+zz) ;
  weights[5] = (1/8) * (1+xx) * (1-yy) * (1+zz) ;
  weights[6] = (1/8) * (1+xx) * (1+yy) * (1+zz) ;
  weights[7] = (1/8) * (1-xx) * (1+yy) * (1+zz) ;

  return weights;
}

ElementHex::ElementHex()
{}

ElementHex::~ElementHex()
{}

