/*
 * ElementHex.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */

#include "Element/ElementHex.hpp"
#include "Element/MaterialModel.hpp"
#include "Util/GaussCube.hpp"
#include "World/ForceVolume.hpp"

#include "Eigen/Core"

#include "Util/glheader.hpp"

#include <iostream>

float ElementHex::GetEnery(const std::vector<Eigen::Vector3f> & X,
  const std::vector<Eigen::Vector3f> & u)
{
  std::vector<float> weights;
  std::vector<Eigen::Vector3f> points;

  GaussCube quadrature;
  quadrature.mn = X[GetNodeIndex(0)];
  quadrature.mx = X[GetNodeIndex(6)];
  quadrature.Get(weights,points);

  std::vector<float> energyDensity(points.size(), 0);

  for(size_t ii = 0; ii<points.size();ii++){
    Eigen::Matrix3f F = GetDeformationGrad(points[ii],X,u);
    energyDensity[ii] = material->GetEnergy(F);
  }

  for(size_t jj = 0;jj<forces.size();jj++) {
    forces[jj]->element = this;
    for(size_t ii = 0; ii<points.size();ii++){
      energyDensity[ii] += forces[jj]->GetEnergyDensity(points[ii]);
    }
  }

  float energy=0;
  for (size_t jj = 0; jj < points.size(); jj++) {
    energy += weights[jj] * energyDensity[jj];
  }

  return energy;
}

std::vector<Eigen::Vector3f>
ElementHex::GetNodalForces(
        const std::vector<Eigen::Vector3f> & X,
        const std::vector<Eigen::Vector3f> & u
    )
{
  std::vector<float> weights;
  std::vector<Eigen::Vector3f> points;

  GaussCube quadrature;
  quadrature.mn = X[GetNodeIndex(0)];
  quadrature.mx = X[GetNodeIndex(6)];
  quadrature.Get(weights,points);

  std::vector<Eigen::Vector3f> f(points.size(),
      Eigen::Vector3f::Zero());

  std::vector<Eigen::Matrix3f> P(points.size());
  for(size_t ii = 0; ii<points.size();ii++){
    Eigen::Matrix3f F = GetDeformationGrad(points[ii],X,u);
    P[ii] = material->GetPK1(F);
  }

  std::vector<Eigen::Vector3f> extForces(points.size());
  for(size_t jj = 0;jj<forces.size();jj++) {
    forces[jj]->element = this;
    for(size_t ii = 0; ii<extForces.size();ii++){
      extForces[ii] += forces[jj]->GetForce(points[ii]);
    }
  }

  for(size_t ii = 0;ii<f.size();ii++){
    for(size_t jj = 0; jj<points.size();jj++){
      Eigen::Vector3f gradN = ShapeFunGrad(ii, points[jj], X);
      f[ii] += weights[jj]* (P[jj]*gradN + extForces[jj]);
    }
  }
  return f;
}

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
ElementHex::GetDeformationGrad(const Eigen::Vector3f & p,
    const std::vector<Eigen::Vector3f> & X,
    const std::vector<Eigen::Vector3f> & u)
{
  Eigen::Matrix3f F = Eigen::Matrix3f::Identity();
  for(int ii = 0;ii<8;ii++){
    int idx = GetNodeIndex(ii);
    Eigen::Vector3f gradN = ShapeFunGrad(ii,p,X);
    //outer product
    F += u[idx] * gradN.transpose();
  }
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

void ElementHex::DrawOpenGL(const std::vector<Eigen::Vector3f> & nodes)
{
  const std::vector<int> & idx = GetNodeIndices();
  std::cout<<"draw hex element\n";
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3f(0,1,1);
  for(int ii = 0; ii<4; ii++){
    glVertex3f(nodes[idx[ii]][0],nodes[idx[ii]][1],nodes[idx[ii]][2]);
    int jj = (ii+1) % 4;
    glVertex3f(nodes[idx[jj]][0],nodes[idx[jj]][1],nodes[idx[jj]][2]);

    int kk = ii+4;
    jj = jj+4;
    glVertex3f(nodes[idx[kk]][0],nodes[idx[kk]][1],nodes[idx[kk]][2]);
    glVertex3f(nodes[idx[jj]][0],nodes[idx[jj]][1],nodes[idx[jj]][2]);

    glVertex3f(nodes[idx[kk]][0],nodes[idx[kk]][1],nodes[idx[kk]][2]);
    glVertex3f(nodes[idx[ii]][0],nodes[idx[ii]][1],nodes[idx[ii]][2]);
  }
  glEnd();
  glEnable(GL_LIGHTING);
}

ElementHex::ElementHex()
{}

ElementHex::~ElementHex()
{}

