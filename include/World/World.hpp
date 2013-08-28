#ifndef WORLD_HPP
#define WORLD_HPP
#include "World/Camera.hpp"
#include <vector>
class Mesh;
class ElementMesh;
class World
{
public:
  Camera camera;
  std::vector<Mesh * > mesh;
  std::vector<ElementMesh * > element;
  std::vector<double> getState();
};
#endif
