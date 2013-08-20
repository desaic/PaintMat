#include "Util/Texture.hpp"
#include "png++/png.hpp"
#include "Util/util.h"
#include "vecmath.h"
#include <string.h>
#include <iostream>

Vector3f 
Texture::operator()(float x, float y)const
{
  Vector3f color;
  unsigned char buf[4];
  operator()(x,y,buf);
  for(int ii = 0;ii<3;ii++){
    color[ii] = buf[ii]/255.0f;
  }
  return color;
}

bool Texture::valid()
{
  return tex_buf!=0;
}


const unsigned char * Texture::getBuf()const
{
  return tex_buf;
}

void Texture::allocate(int w, int h)
{
  if(tex_buf!=0){
    delete tex_buf;
  }
  width = w;
  height = h;
  int bufsize = w*h*4;
  tex_buf = new unsigned char[bufsize];
  for(int ii = 0;ii<bufsize;ii++){
    tex_buf[ii] = 0;
  }
}

void
Texture::load(const char * filename)
{
  png::image<png::rgb_pixel> img(filename);
  allocate(img.get_width(),img.get_height());
  unsigned char color[4];
  color[3] = 255;
  png::rgb_pixel pixel;
  for(size_t ii = 0;ii<img.get_width(); ii++){
   for(size_t jj = 0;jj<img.get_height(); jj++){
     pixel = img.get_pixel(ii,img.get_height()-jj-1);
     color[0] = pixel.red;
     color[1] = pixel.green;
     color[2] = pixel.blue;
     setPixel(ii,jj,color);
   }
  }
}

void Texture::save(const char * filename)
{
  png::image<png::rgb_pixel> img(width,height);
  unsigned char color[4];
  color[3] = 255;
  png::rgb_pixel pixel;
  for(size_t jj = 0;jj<img.get_height(); jj++){
    for(size_t ii = 0;ii<img.get_width(); ii++){
     operator()((int)ii,(int)jj,color);
     pixel.red  = color[0];
     pixel.green= color[1];
     pixel.blue = color[2];
     img.set_pixel(ii,img.get_height()-jj-1,pixel);
   }
  }
  img.write(filename);
}

void
Texture::operator()(int x, int y,  unsigned char * color)const
{
  x = clamp(x, 0, width - 1);
  y = clamp(y, 0, height - 1);
  int idx = 4 * (y * width + x);
  for (int ii = 0; ii < 4; ii++){
    color[ii] = tex_buf[idx + ii];
  }
}

///@param x assumed to be between 0 and 1
void
Texture::operator()(float x, float y, unsigned char * color)const
{
  int ix, iy;

  ix = (int) x;
  iy = (int) y;
  x -= ix;
  y -= iy;
  if (x < 0) {
    x += 1;
  }

  if (y < 0) {
    y += 1;
  }
  x = x * width;
  y = y * height;
  ix = (int) x;
  iy = (int) y;
  unsigned char pixels[4][4];
  operator()(ix, iy, pixels[0]);
  operator()(ix + 1, iy, pixels[1]);
  operator()(ix, iy + 1, pixels[2]);
  operator()(ix + 1, iy + 1, pixels[3]);
  float alpha = x - ix;
  float beta = y - iy;
  for (int ii = 0; ii < 4; ii++) {
    color[ii] = (1 - alpha) * (1 - beta) * pixels[0][ii]
                   + alpha * (1 - beta) * pixels[1][ii]
              + (1 - alpha) * beta * pixels[2][ii]
                    + alpha * beta * pixels[3][ii];
  }
}

void Texture::setPixel(int x, int y, unsigned char * color)
{
  int baseIdx =x+y*width;
  for(int ii = 0 ;ii<4;ii++){
    tex_buf[baseIdx*4+ii] = color[ii];
  }
}

int Texture::getWidth()const
{
  return width;
}

int Texture::getHeight()const
{
  return height;
}

Texture::~Texture()
{
  if(tex_buf){
    delete[] tex_buf;
    tex_buf = 0;
  }
}

Texture::Texture():tex_buf(0),width(0),height(0)
{
}

Texture & Texture ::operator=(const Texture & t)
{
  if(tex_buf!=0){
    delete []tex_buf;
  }
  width = t.getWidth();
  height = t.getHeight();
  int bufsize = 4*sizeof(unsigned char)*width*height;
  tex_buf = new unsigned char [bufsize];
  memcpy(tex_buf, t.getBuf(), bufsize);
  return *this;
}

int
rgb2Gray(unsigned char*color)
{
  return (int) (0.299 * color[0] + 0.587 * color[1] + 0.114 * color[2]);
}
