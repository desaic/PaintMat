/*
 * TimeStatic.hpp
 *
 *  Created on: Aug 29, 2013
 *      Author: desaic
 */

#ifndef TIMESTATIC_HPP_
#define TIMESTATIC_HPP_

#include "TimeStepper.hpp"

class World;
class ElementMesh;
class TimeStatic: public TimeStepper
{
public:
  TimeStatic();
  virtual ~TimeStatic();
  void Step(World * world);
private:
  void StepElement(ElementMesh * mesh);
};

#endif /* TIMESTATIC_HPP_ */
