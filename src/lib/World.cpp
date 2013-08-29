/*
 * World.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */
#include "World/World.hpp"

void World::ApplyForce()
{
  for(size_t ii = 0;ii<element.size();ii++){
    element[ii]->forces= forces;
  }
}

std::vector<double>
World::getState()
{
  std::cout<<"Note: Unimplemented\n";
  return std::vector<double>();
}
