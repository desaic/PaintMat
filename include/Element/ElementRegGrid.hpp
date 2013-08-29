/*
 * ElementRegGrid.hpp
 *
 *  Created on: Aug 29, 2013
 *      Author: desaic
 */

#ifndef ELEMENTREGGRID_HPP_
#define ELEMENTREGGRID_HPP_
#include "Element/ElementMesh.hpp"
#include "Eigen/Dense"
class ElementRegGrid:public ElementMesh
{
public:
  ElementRegGrid(int _nx = 1 , int _ny = 1, int _nz = 1);
  int GetEleInd(const Eigen::Vector3f & p);
  virtual ~ElementRegGrid();
private:
  int nx,ny,nz;
};

#endif /* ELEMENTREGGRID_HPP_ */
