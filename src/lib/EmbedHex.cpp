/*
 * EmbedHex.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: desaic
 */

#include "Element/Element.hpp"
#include "Element/EmbedHex.hpp"
#include "Element/ElementRegGrid.hpp"
#include "World/Mesh.hpp"
#include <iostream>

void EmbedHex::draw()
{
  Mesh::draw(u);
}

void EmbedHex::update()
{
  for(size_t ii = 0; ii< u.size();ii++){
    Element * ele = grid->elements[eleInd[ii]];
    std::vector<float> weights = ele->InterpWeights(v[ii],grid->nodes);
    Eigen::Vector3f tmp = Eigen::Vector3f::Zero();
    for(size_t jj = 0;jj<weights.size();jj++){
      int nodeIdx = ele->GetNodeIndex(jj);
      tmp += weights[jj] * grid->u[nodeIdx];
    }
    u[ii] = tmp;
  }
}

void EmbedHex::init()
{
  if(grid == 0 ){
    std::cout<<"Error: Need a grid and a mesh";
    return;
  }
  u.resize(v.size());
  eleInd.resize(u.size());
  for(size_t ii = 0;ii<u.size();ii++){
    eleInd[ii] = grid->GetEleInd(v[ii]);
  }
}

EmbedHex::EmbedHex():grid(0)
{
}

EmbedHex::~EmbedHex()
{}

