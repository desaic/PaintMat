/*
 * TimeStatic.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: desaic
 */

#include "TimeStepper/TimeStatic.hpp"
#include "World/World.hpp"
#include "Element/ElementMesh.hpp"

static int NSteps = 100;
static float hh = 0.01;
void TimeStatic::StepElement(ElementMesh * mesh)
{
  std::vector<Eigen::Vector3f> nodalForces = mesh->GetForces();
  for(size_t ii = 0;ii<nodalForces.size();ii++){
    mesh->u[ii] += hh*nodalForces[ii];
    //manually set above the ground constraint
    if(mesh->u[ii][1]<0){
      mesh->u[ii][1] = 0;
    }
  }
}

void TimeStatic::Step(World * world)
{
  world->ApplyForce();
  for(int ii = 0;ii<NSteps;ii++){
    for(size_t jj = 0;jj<world->element.size();jj++){
      StepElement(world->element[jj]);
    }
  }
}

TimeStatic::TimeStatic()
{
}

TimeStatic::~TimeStatic()
{
}

