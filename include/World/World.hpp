#ifndef WORLD_HPP
#define WORLD_HPP
#include "World/Camera.hpp"
#include <vector>
class Mesh;
class World
{
public:
  Camera camera;
  std::vector<Mesh * > mesh;
  std::vector<double> getState();
};
#endif
