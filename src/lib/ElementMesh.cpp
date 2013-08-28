/*
 * ElementMesh.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */
#include "Element/Element.hpp"
#include "Element/ElementMesh.hpp"
#include "Element/ElementHex.hpp"

static ElementMesh * CreateGrid(int nx, int ny, int nz)
{
  for(int ii =0 ;)
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

