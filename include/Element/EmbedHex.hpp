/*
 * EmbedHex.hpp
 *
 *  Created on: Aug 29, 2013
 *      Author: desaic
 */

#ifndef EMBEDHEX_HPP_
#define EMBEDHEX_HPP_
#include "Eigen/Dense"
#include "World/Mesh.hpp"
class ElementRegGrid;

///@brief embeds a mesh in a regular hexahedron grid
class EmbedHex:public Mesh
{
public:
  EmbedHex();
  virtual ~EmbedHex();
  ElementRegGrid * grid;

  ///@brief deformed vertices
  std::vector<Eigen::Vector3f> u;
  ///@brief which element is each vertex in
  std::vector<int>eleInd;
  void init();
  virtual void update();
  virtual void draw();
};

#endif /* EMBEDHEX_HPP_ */
