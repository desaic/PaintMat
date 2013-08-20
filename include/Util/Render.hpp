/*
 * Render.hpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */

#ifndef RENDER_HPP_
#define RENDER_HPP_
class World;
class Render
{
public:
  Render();
  void init(World * world);
  void loop();
  virtual ~Render();
private:
  World * world;
};

#endif /* RENDER_HPP_ */
