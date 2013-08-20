#include "util.h"

struct Camera{
  Camera(){
    for (int ii=0;ii<3;ii++){
      eye[ii]=0.0;
      at[ii]=0.0;
      up[ii] = 0.0f;
    }
    eye[1]=-2;
    up[2] = 1;
  }

  real_t eye[3];
  real_t at[3];
  real_t up[3];
};

