#ifndef CAMERA_HPP
#define CAMERA_HPP

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

  double eye[3];
  double at[3];
  double up[3];
};

#endif
