/*
 * ElementRegGrid.hpp
 *
 *  Created on: Aug 29, 2013
 *      Author: desaic
 */

#ifndef ELEMENTREGGRID_HPP_
#define ELEMENTREGGRID_HPP_
#include "Element/ElementMesh.hpp"
class ElementRegGrid:public ElementMesh
{
public:
  ElementRegGrid(int _nx = 1 , int _ny = 1, int _nz = 1);
  virtual ~ElementRegGrid();
private:
  int nx,ny,nz;
};

#endif /* ELEMENTREGGRID_HPP_ */
