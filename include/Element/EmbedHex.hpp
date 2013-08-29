/*
 * EmbedHex.hpp
 *
 *  Created on: Aug 29, 2013
 *      Author: desaic
 */

#ifndef EMBEDHEX_HPP_
#define EMBEDHEX_HPP_
#include "Eigen/Dense"

class ElementMesh;
class Mesh;
///@brief embeds a mesh in a regular hexahedron grid
class EmbedHex
{
public:
  EmbedHex();
  virtual ~EmbedHex();
  ElementMesh * grid;
  Mesh * mesh;
  ///@brief deformed vertices
  std::vector<Eigen::Vector3f> u;
  ///@brief which element is each vertex in
  std::vector<int>eleInd;
};

#endif /* EMBEDHEX_HPP_ */
