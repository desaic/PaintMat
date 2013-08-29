#include <iostream>
#include <string.h>
#include <pthread.h>

#include "Element/Element.hpp"
#include "Element/ElementMesh.hpp"
#include "Element/MaterialStVK.hpp"
#include "Util/Render.hpp"
#include "World/World.hpp"
#include "World/Mesh.hpp"
#include "World/ForceVolume.hpp"
#include "World/ForceGravity.hpp"
#include "TimeStepper/TimeStepper.hpp"
#include "TimeStepper/TimeStatic.hpp"

#include "UnitTests.hpp"

struct StepperArg
{
  TimeStepper * stepper;
  World * world;
};

void *
stepTime(void * arg)
{
  StepperArg * stepperArg = (StepperArg*)(arg);
  stepperArg->stepper->Step(stepperArg->world);
  return 0;
}

int main(int argc , char * argv[])
{

  QuadratureTest();
  return 0;

  World world;
  Mesh m;
  ElementMesh * element = ElementMesh::CreateGrid(2,3,4);

  MaterialStVK material;
  for(size_t ii = 0;ii<element->elements.size();ii++){
    element->elements[ii]->material = & material;
  }

  ForceGravity gravity;
  TimeStatic timeStepper;
  gravity.g = Eigen::Vector3f(0,-1,0);
  world.forces.push_back(&gravity);
  world.element.push_back(element);
  Render render;
  StepperArg arg;
  arg.stepper = &timeStepper;
  arg.world = & world;
  pthread_t timeThread;
  if(argc<2){
    std::cout<<"./PaintMat filename\n";
    return 0;
  }
  const char * meshfile = argv[1];
  const char * texfile = 0;

  for(int ii = 2;ii<argc;ii++){
    if(strcmp(argv[ii],"-t")==0 && ii+1<argc){
      ii++;
      texfile = argv[ii];
      continue;
    }
  }
  render.init(&world);
  m.load_mesh(meshfile,true);
  if(texfile!=0){
    m.load_tex(texfile);
  }
  world.mesh.push_back(&m);

  pthread_create(&timeThread, NULL, stepTime, (void*)&arg);

  render.loop();
  return 0;
}
