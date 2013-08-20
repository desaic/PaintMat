#ifndef TIMESTEPPER_HPP
#define TIMESTEPPER_HPP
class World;
class TimeStepper
{
public:
  virtual void Step(World * world)=0;
  virtual ~TimeStepper();
};
#endif
