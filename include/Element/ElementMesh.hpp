/*
 * ElementMesh.hpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */

#ifndef ELEMENTMESH_HPP_
#define ELEMENTMESH_HPP_
#include "Eigen/Dense"
#include <vector>

class Element;
class ElementMesh
{
public:
  ElementMesh();
  virtual ~ElementMesh();
  std::vector<Element> elements;
  std::vector<Eigen::Vector3f> nodes;
};

#endif /* ELEMENTMESH_HPP_ */
