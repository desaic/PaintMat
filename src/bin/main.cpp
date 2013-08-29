#include <iostream>
#include <string.h>
#include "Element/ElementMesh.hpp"
#include "Util/Render.hpp"
#include "World/World.hpp"
#include "World/Mesh.hpp"


int main(int argc , char * argv[])
{
  World world;
  Mesh m;
  ElementMesh * element = ElementMesh::CreateGrid(2,3,4);
  ForceGravity gravity;
  gravity.g = Vector3f(0,-1,0);
  world.forces.push_back(&gravity);
  world.element.push_back(element);
  Render render;

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

  render.loop();
  return 0;
}
