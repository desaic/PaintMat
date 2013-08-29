/*
 * ElementRegGrid.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: desaic
 */

#include "Element/ElementRegGrid.hpp"
#include "Element/ElementHex.hpp"
#include "Util/util.h"
//node index
#define IX(ii,jj,kk) ( (ii) * NY * NZ + (jj) * NZ + kk)

int ElementRegGrid::GetEleInd(const Eigen::Vector3f & p)
{
  int ii = (int)(p[0]*nx);
  int jj = (int)(p[1]*nx);
  int kk = (int)(p[2]*nx);
  ii = clamp(ii, 0, nx - 1);
  jj = clamp(jj, 0, ny - 1);
  kk = clamp(kk, 0, nz - 1);
  return ii * ny * nz + jj * ny + kk;
}

ElementRegGrid::  ElementRegGrid(int _nx , int _ny , int _nz )
:nx(_nx),ny(_ny),nz(_nz)
{
  double dx = 1.0 / nx;
  //number of vertices is 1 more than grid cells in each dimension
  int NX = nx + 1;
  int NY = ny + 1;
  int NZ = nz + 1;
  for (int ii = 0; ii <= nx; ii++) {
    for (int jj = 0; jj <= ny; jj++) {
      for (int kk = 0; kk <= nz; kk++) {
        nodes.push_back(dx * Eigen::Vector3f(ii, jj, kk));
      }
    }
  }
  u = nodes;
  for (int ii = 0; ii < nx; ii++) {
    for (int jj = 0; jj < ny; jj++) {
      for (int kk = 0; kk < nz; kk++) {
        std::vector<int> indices(8);
        //consistent with the drawing in working doc?
        indices[0] = IX(ii, jj, kk);
        indices[1] = IX(ii+1,jj, kk);
        indices[2] = IX(ii+1,jj+1,kk);
        indices[3] = IX(ii, jj+1,kk);

        indices[4] = IX(ii, jj, kk+1);
        indices[5] = IX(ii+1,jj, kk+1);
        indices[6] = IX(ii+1,jj+1,kk+1);
        indices[7] = IX(ii, jj+1,kk+1);

        ElementHex * ele = new ElementHex();
        ele->SetIndices(indices);
        elements.push_back(ele);
      }
    }
  }
}

ElementRegGrid::~ElementRegGrid()
{
  // TODO Auto-generated destructor stub
}
