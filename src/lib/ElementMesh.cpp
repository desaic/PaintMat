/*
 * ElementMesh.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */
#include "Element/Element.hpp"
#include "Element/ElementMesh.hpp"
#include "Element/ElementHex.hpp"
#include <iostream>

//@brief get forces for each node.
std::vector<Eigen::Vector3f>
ElementMesh::GetForces()
{
  std::vector<Eigen::Vector3f> nodalForces(nodes.size(),
      Eigen::Vector3f::Zero());
  for(size_t ii = 0;ii<elements.size();ii++){
    Element * e =elements[ii];
    e->forces = forces;
    std::vector<Eigen::Vector3f> eForce;
    eForce = e->GetNodalForces(nodes,u);
    std::vector<int> indices = e->GetNodeIndices();
    if(indices.size() != eForce.size()){
      std::cout<<"Error: number of forces does not equal number of nodes\n";
    }
    for(size_t jj = 0;jj<indices.size();jj++){
      nodalForces[indices[jj]] += eForce[jj];
    }
  }
  return nodalForces;
}

void
ElementMesh::DrawOpenGL()
{
  for(size_t ii = 0; ii<elements.size(); ii++){
    elements[ii]->DrawOpenGL(u);
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

