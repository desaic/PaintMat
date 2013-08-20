#ifndef TIMESTEPPER_HPP
#define TIMESTEPPER_HPP
class TimeStepper
{
public:
  virtual void Step()=0;
  virtual ~TimeStepper();
};
#endif
