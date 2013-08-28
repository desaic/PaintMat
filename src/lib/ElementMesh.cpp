/*
 * ElementMesh.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */
#include "Element/Element.hpp"
#include "Element/ElementMesh.hpp"
#include "Element/ElementHex.hpp"

//node index
#define IX(ii,jj,kk) ( (ii) * NY * NZ + (jj) * NZ + kk)

ElementMesh *
ElementMesh::CreateGrid(int nx, int ny, int nz)
{
  double dx = 1.0/nx;
  ElementMesh * mesh = new ElementMesh();
  //number of vertices is 1 more than grid cells in each dimension
  int NX = nx+1;
  int NY = ny+1;
  int NZ = nz+1;
  for (int ii = 0; ii <= nx; ii++) {
    for (int jj = 0; jj <= ny; jj++) {
      for (int kk = 0; kk <= nz; kk++) {
        mesh->nodes.push_back( dx * Eigen::Vector3f(ii,jj,kk) );
      }
    }
  }
  for (int ii = 0; ii < nx; ii++) {
    for (int jj = 0; jj < ny; jj++) {
      for (int kk = 0; kk < nz; kk++) {
        std::vector<int> indices(8);
        //consistent with the drawing in working doc?
        indices[0] = IX(ii,  jj,  kk);
        indices[1] = IX(ii+1,jj,  kk);
        indices[2] = IX(ii+1,jj+1,kk);
        indices[3] = IX(ii,  jj+1,kk);

        indices[4] = IX(ii,  jj,  kk+1);
        indices[5] = IX(ii+1,jj,  kk+1);
        indices[6] = IX(ii+1,jj+1,kk+1);
        indices[7] = IX(ii,  jj+1,kk+1);

        ElementHex * ele = new ElementHex();
        ele->SetIndices(indices);
        mesh->elements.push_back(ele);
      }
    }
  }
  return mesh;
}

void
ElementMesh::DrawOpenGL()
{
  for(size_t ii = 0; ii<elements.size(); ii++){
    elements[ii]->DrawOpenGL(nodes);
  }
}

ElementMesh::ElementMesh()
{

}

ElementMesh::~ElementMesh()
{
  for (size_t ii = 0; ii < elements.size(); ii++) {
    delete elements[ii];
  }
}

