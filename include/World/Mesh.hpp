#ifndef MESH_H
#define MESH_H

#include <map>
#include <vector>
#include <fstream>
#include "Util/Texture.hpp"
#include "Eigen/Dense"
#include "Util/glheader.hpp"

class Mesh{
public:
  std::vector<Eigen::Vector3f>v;
  std::vector<Eigen::Vector3f>n;
  std::vector<Eigen::Vector2f>tex;
  std::vector<Eigen::Vector3i>texId;
  ///@brief triangles
  std::vector<Eigen::Vector3i>t;

  Mesh();
  Mesh(const std::vector<Eigen::Vector3f>&_v,
      const std::vector<Eigen::Vector3i>&_t);

  Mesh(const char * filename,bool normalize);
  void load_mesh(const char * filename, bool normalize=true);
  virtual void draw();
  void draw(std::vector<Eigen::Vector3f>&v);
  void drawWire();
  void save(const char * filename);
  void read_ply(std::ifstream & f);
  void read_obj(std::ifstream & f);
  void save_obj(const char * filename, bool flip = false);
  void load_tex(const char * filename);
  Texture texture;
  void compute_norm();
  void rescale();
  void append(const Mesh & m);
  Mesh & operator= (const Mesh& m);
  virtual void update();
private:
  GLuint glTexID;
};
void makeCube(Mesh & m, const Eigen::Vector3f & mn,
    const Eigen::Vector3f mx);
///@brief cube [0,1]^3
extern Mesh UNIT_CUBE;
void BBox(const std::vector<Eigen::Vector3f >& v,
    Eigen::Vector3f & mn, Eigen::Vector3f & mx);
void BBox(const Mesh & m, Eigen::Vector3f & mn,
    Eigen::Vector3f & mx);
unsigned int b2int(GLubyte * b);
bool ptInBox(const Eigen::Vector3f & mn,
    const Eigen::Vector3f mx, const Eigen::Vector3f & x);
void adjlist(const Mesh & m, std::vector<std::vector<int> > & adjMat);

#endif
