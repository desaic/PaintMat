#ifndef IMAGE_H
#define IMAGE_H

#include <cassert>
#include <vecmath.h>
#include "util.h"
//@brief Simple image class written for caustics rendering.
class Image
{
public:
  Image()
  {
    width = 0;
    height = 0;
    data = 0;
  }
  void resize(int w, int h)
  {
    if (data != 0) {
      delete[] data;
    }
    width = w;
    height = h;
    data = new Vector3f[width * height];
  }
  Image(int w, int h)
  {
    width = w;
    height = h;
    data = new Vector3f[width * height];
  }

  ~Image()
  {
    delete[] data;
  }

  void addImage(const Image & image);

  int Width() const
  {
    return width;
  }

  int Height() const
  {
    return height;
  }

  Vector3f GetPixel(int x, int y) const
  {
    x = clamp(x, 0, width - 1);
    y = clamp(y, 0, height - 1);
    return data[y * width + x];
  }
  Vector3f getPixel(float x, float y)const;

  void setAllPixels(const Vector3f& color)
  {
    for (int i = 0; i < width * height; ++i) {
      data[i] = color;
    }
  }

  void SetPixel(int x, int y, const Vector3f& color)
  {
    x = clamp(x, 0, width - 1);
    y = clamp(y, 0, height - 1);
    data[y * width + x] = color;
  }

  ///@brief if out of boundary , no color added.
  ///add colors to four overlapping neighbour pixels
  void addColor(float x, float y, const Vector3f& color);

  static Image* LoadPPM(const char* filename);
  void loadPNG(const char * filename);
  void savePPM(const char* filename) const;

  static Image* LoadTGA(const char* filename);
  void saveTGA(const char* filename) const;
  int saveBMP(const char *filename);
  void saveImage(const char *filename);
  // extension for image comparison
  static Image* compare(Image* img1, Image* img2);
  void drawLine(int x0, int x1, int y0, int y1);
private:
  inline bool inbound(int x, int y)
  {
    bool ans = (x >= 0) && (x < width) && (y >= 0) && (y < height);
    return ans;
  }
  inline int IX(int x, int y)
  {
    return y * width + x;
  }
  int width;
  int height;
  Vector3f * data;

};

#endif // IMAGE_H
