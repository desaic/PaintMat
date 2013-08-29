#include <iostream>
#include <string.h>
#include <pthread.h>

#include "Element/Element.hpp"
#include "Element/ElementMesh.hpp"
#include "Element/ElementRegGrid.hpp"
#include "Element/EmbedHex.hpp"
#include "Element/MaterialStVK.hpp"
#include "Util/Render.hpp"
#include "World/World.hpp"
#include "World/Mesh.hpp"
#include "World/ForceVolume.hpp"
#include "World/ForceGravity.hpp"
#include "TimeStepper/TimeStepper.hpp"
#include "TimeStepper/TimeStatic.hpp"

#include "UnitTests.hpp"
#include <cmath>
#include <unistd.h>
struct StepperArg
{
  TimeStepper * stepper;
  World * world;
};

void *
stepTime(void * arg)
{
  StepperArg * stepperArg = (StepperArg*)(arg);
  //stepperArg->stepper->Step(stepperArg->world);
  ElementMesh * eleMesh = stepperArg->world->element[0];
  int t= 0;

  while(1){
    for(size_t ii = 0;ii<eleMesh->u.size();ii++){
      float displace = std::sin((float)(3.14 * t/100.0 + 8*eleMesh->nodes[ii][2]));
      eleMesh->u[ii][0] = eleMesh->nodes[ii][0] + 0.1*displace;

      displace = std::sin((float)(3.14 * t/100.0 + 16*eleMesh->nodes[ii][2]));
      eleMesh->u[ii][1] = eleMesh->nodes[ii][1] + 0.05*displace;
    }
    t++;
    if(t>=200){
      t=0;
    }
    usleep(10000);
  }
  return 0;
}

int main(int argc , char * argv[])
{

  EmbedHex embed;
  World world;
  EmbedHex m;
  ElementRegGrid * element = new ElementRegGrid(32,32,32);

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
  m.grid = element;
  m.init();
  m.update();
  pthread_create(&timeThread, NULL, stepTime, (void*)&arg);

  render.loop();
  return 0;
}
