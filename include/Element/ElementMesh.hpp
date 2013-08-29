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
  ///@brief pointers are owned and freed by this class
  std::vector<Element*> elements;
  ///@brief each node has 3 degrees of freedom.
  std::vector<Eigen::Vector3f> nodes;
  ///@brief Deformed state
  std::vector<Eigen::Vector3f> u;
  ///@brief list of volume forces that are applied to all elements in the mesh
  std::vector<ForceVolume * > forces;
  //@brief get forces for each node.
  std::vector<Eigen::Vector3f> GetForces();

  void DrawOpenGL();

  static ElementMesh * CreateGrid(int nx, int ny, int nz);
};

#endif /* ELEMENTMESH_HPP_ */
