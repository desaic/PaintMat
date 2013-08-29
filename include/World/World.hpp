#ifndef WORLD_HPP
#define WORLD_HPP
#include "World/Camera.hpp"
#include <vector>
class Mesh;
class ElementMesh;
class ForceVolume;
class World
{
public:
  Camera camera;
  std::vector<Mesh * > mesh;
  std::vector<ElementMesh * > element;
  std::vector<double> getState();
  ///@brief forces that are applied  to all elements
  std::vector<ForceVolume * > forces;

  void ApplyForce();
};
#endif
