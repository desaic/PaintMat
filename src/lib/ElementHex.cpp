/*
 * ElementHex.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */

#include "Element/ElementHex.hpp"
#include "Eigen/Core"

Eigen::Vector3f
ElementHex::ShapeFunGrad(int ii,
    const Eigen::Vector3f & p,
    const std::vector<Eigen::Vector3f> & X)
{
  Eigen::Vector3f xx = getLocalCoord(p,X);
  //min and max node
  Eigen::Array3f X1, X2;
  X1 = X[GetNodeIndex(0)].array();
  X2 = X[GetNodeIndex(6)].array() - X1;
  X2 = -1.0/(4*X2);
  Eigen::Vector3f gradN;
  switch (ii) {
  case 0:
    gradN[0] = X2[0] * (1 - xx[1]) * (1 - xx[2]);
    gradN[1] = X2[1] * (1 - xx[0]) * (1 - xx[2]);
    gradN[2] = X2[2] * (1 - xx[0]) * (1 - xx[1]);
    break;
  case 1:
    gradN[0] = X2[0] * (1 - xx[1]) * (1 - xx[2]);
    gradN[1] = X2[1] * (1 + xx[0]) * (1 - xx[2]);
    gradN[2] = X2[2] * (1 + xx[0]) * (1 - xx[1]);
    break;
  case 2:
    gradN[0] = X2[0] * (1 + xx[1]) * (1 - xx[2]);
    gradN[1] = X2[1] * (1 + xx[0]) * (1 - xx[2]);
    gradN[2] = X2[2] * (1 + xx[0]) * (1 + xx[1]);
    break;
  case 3:
    gradN[0] = X2[0] * (1 + xx[1]) * (1 - xx[2]);
    gradN[1] = X2[1] * (1 - xx[0]) * (1 - xx[2]);
    gradN[2] = X2[2] * (1 - xx[0]) * (1 + xx[1]);
    break;

  case 4:
    gradN[0] = X2[0] * (1 - xx[1]) * (1 + xx[2]);
    gradN[1] = X2[1] * (1 - xx[0]) * (1 + xx[2]);
    gradN[2] = X2[2] * (1 - xx[0]) * (1 - xx[1]);
    break;
  case 5:
    gradN[0] = X2[0] * (1 - xx[1]) * (1 + xx[2]);
    gradN[1] = X2[1] * (1 + xx[0]) * (1 + xx[2]);
    gradN[2] = X2[2] * (1 + xx[0]) * (1 - xx[1]);
    break;
  case 6:
    gradN[0] = X2[0] * (1 + xx[1]) * (1 + xx[2]);
    gradN[1] = X2[1] * (1 + xx[0]) * (1 + xx[2]);
    gradN[2] = X2[2] * (1 + xx[0]) * (1 + xx[1]);
    break;
  case 7:
    gradN[0] = X2[0] * (1 + xx[1]) * (1 + xx[2]);
    gradN[1] = X2[1] * (1 - xx[0]) * (1 + xx[2]);
    gradN[2] = X2[2] * (1 - xx[0]) * (1 + xx[1]);
    break;
  }
  return gradN;
}

Eigen::Matrix3f
ElementHex::GetDeformationGrad(const Eigen::Vector3f & _p,
    const std::vector<Eigen::Vector3f> & X,
    const std::vector<Eigen::Vector3f> & u)
{
  Eigen::Matrix3f F;
  Eigen::Vector3f p = getLocalCoord(_p, X);

  return F;
}

Eigen::Vector3f
ElementHex::getLocalCoord(const Eigen::Vector3f & p,
    const std::vector<Eigen::Vector3f> & X)
{
  Eigen::Vector3f local;
  //min and max node
  Eigen::Vector3f N1, N2;
  N1 = X[GetNodeIndex(0)];
  N2 = X[GetNodeIndex(6)];

  local = (2 * (p-N1).array() / (N2-N1).array() - 1).matrix();
  return local;
}

std::vector<float>
ElementHex::InterpWeights(const Eigen::Vector3f & p,
        const std::vector<Eigen::Vector3f> & X)
{
  std::vector<float> weights(8);

  Eigen::Vector3f xx = getLocalCoord(p,X);

  weights[0] = (1.0/8) * (1-xx[0]) * (1-xx[1]) * (1-xx[2]) ;
  weights[1] = (1.0/8) * (1+xx[0]) * (1-xx[1]) * (1-xx[2]) ;
  weights[2] = (1.0/8) * (1+xx[0]) * (1+xx[1]) * (1-xx[2]) ;
  weights[3] = (1.0/8) * (1-xx[0]) * (1+xx[1]) * (1-xx[2]) ;

  weights[4] = (1.0/8) * (1-xx[0]) * (1-xx[1]) * (1+xx[2]) ;
  weights[5] = (1.0/8) * (1+xx[0]) * (1-xx[1]) * (1+xx[2]) ;
  weights[6] = (1.0/8) * (1+xx[0]) * (1+xx[1]) * (1+xx[2]) ;
  weights[7] = (1.0/8) * (1-xx[0]) * (1+xx[1]) * (1+xx[2]) ;

  return weights;
}

ElementHex::ElementHex()
{}

ElementHex::~ElementHex()
{}
