#ifndef TEXTURE_HPP
#define TEXTURE_HPP
class Vector3f;
///@brief helper class that stores a texture and faciliates lookup
///assume 4byte RGBA image data
class Texture{
public:
  Texture();
  ~Texture();
  Texture & operator=(const Texture & t);
  ///@create an empty image of size w x h
  void allocate(int w, int h);
  void load(const char * filename);
  void save(const char * filename);
  void operator()(int x, int y,  unsigned char * color)const;
  ///@param x assumed to be between 0 and 1
  void operator()(float x, float y, unsigned char * color)const;
  Vector3f operator()(float x, float y)const;
  void setPixel(int x, int y, unsigned char * color);
  bool valid();
  int getWidth()const;
  int getHeight()const;
  const unsigned char * getBuf()const;

//private:
  unsigned char * tex_buf;
  int width , height;
};

int rgb2Gray(unsigned char*color);
#endif
