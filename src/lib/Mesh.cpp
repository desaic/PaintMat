#include "World/Mesh.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#ifdef _WIN32
#define NOMINMAX //Stop errors with std::max
#include <windows.h>
#endif
#include <stdio.h>
#include <cstdlib>
#include <utility>
#include <map>
#include <sstream>
#include <string.h>
#include "Util/util.h"

#include "vecmath.h"
///@brief is a point inside a box
bool ptInBox(const Vector3f & mn, const Vector3f mx, const Vector3f & x)
{
  for(int dim = 0 ;dim<3;dim++){
    if(x[dim]<mn[dim] || x[dim] > mx[dim]){
        return false;
    }
  }
  return true;
}
void makeCube(Mesh & m, const Vector3f & mn, const Vector3f mx)
{
  Vector3f ss = mx -mn;
  Matrix4f tran= Matrix4f::scaling(ss[0],ss[1],ss[2]);
  tran = Matrix4f::translation(mn[0],mn[1],mn[2]) * tran;
  m=UNIT_CUBE;
  for(unsigned int ii = 0;ii<m.v.size();ii++){
    m.v[ii] = (tran*Vector4f(m.v[ii],1)).xyz();
  }
}
void Mesh::append(const Mesh & m)
{
  unsigned int offset = v.size();
  unsigned int ot = t.size();
  v.insert(v.end(),m.v.begin(),m.v.end());
  t.insert(t.end(),m.t.begin(), m.t.end());
  for(unsigned int ii = ot;ii<t.size();ii++){
    for(int jj = 0 ;jj<3;jj++){
      t[ii][jj] += offset;
    }
  }
}

Mesh & Mesh::operator= (const Mesh& m)
{
  v = m.v;
  t = m.t;
  n = m.n;
  tex = m.tex;
  texId = m.texId;
  glTexID = m.glTexID;
  return *this;
}

///@brief cube [0,1]^3
std::vector<Vector3f> CUBE_VERT={Vector3f (0,0,0),
    Vector3f (1,0,0),
    Vector3f (1, 0, 1),
    Vector3f (0, 0, 1),
    Vector3f (0, 1, 0),
    Vector3f (1, 1, 0),
    Vector3f (1, 1, 1),
    Vector3f (0, 1, 1)};

std::vector<Eigen::Vector3i>CUBE_TRIG={Eigen::Vector3i(0,1,3),
    Eigen::Vector3i(1, 2, 3),
    Eigen::Vector3i(5, 0, 4),
    Eigen::Vector3i(5, 1, 0),
    Eigen::Vector3i(6, 1, 5),
    Eigen::Vector3i(6, 2, 1),
    Eigen::Vector3i(3, 2, 6),
    Eigen::Vector3i(3, 6, 7),
    Eigen::Vector3i(4, 0, 3),
    Eigen::Vector3i(3, 7, 4),
    Eigen::Vector3i(7, 5, 4),
    Eigen::Vector3i(7, 6, 5)};
Mesh UNIT_CUBE(CUBE_VERT,CUBE_TRIG);

Mesh::Mesh():v(0),t(0),
    glTexID(0){}

Mesh::Mesh(const std::vector<Vector3f>&_v,
    const std::vector<Eigen::Vector3i>&_t):v(_v),t(_t),
    glTexID(0)
{
  compute_norm();
}

void Mesh::save(const char * filename)
{
  std::ofstream out;
  out.open(filename);
  out<<v.size()<<"\n"<<t.size()<<"\n";
  for(size_t ii=0; ii<v.size(); ii++) {
    for(int jj=0; jj<3; jj++) {
      out<<v[ii][jj]<<" ";
    }
    out<<"\n";
  }
  for(size_t ii=0; ii<t.size(); ii++) {
    out<<"3";
    for(int jj=0; jj<3; jj++) {
      out<<" "<<t[ii][jj];
    }
    out<<"\n";
  }
  out.close();
}

void Mesh::load_tex(const char * filename) {
  texture.load(filename);
  if(!texture.valid()){
    return;
  }
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1,&glTexID);
  glBindTexture( GL_TEXTURE_2D, glTexID );
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_LUMINANCE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
               texture.getHeight(),  texture.getHeight(),
               0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getBuf());
  printf("gl error %d\n",(int)glGetError());
  printf("max size %d\n",(int)GL_MAX_TEXTURE_SIZE);
}

void Mesh::read_obj(std::ifstream & f)
{
  std::string line;
  std::string vTok("v");
  std::string fTok("f");
  std::string texTok("vt");
  char bslash='/',space=' ';
  std::string tok;
  while(1) {
    std::getline(f,line);
    if(f.eof()) {
      break;
    }
    if(line.size()<3) {
      continue;
    }
    if(line.at(0)=='#') {
      continue;
    }
    std::stringstream ss(line);
    ss>>tok;
    if(tok==vTok) {
      Vector3f vec;
      ss>>vec[0]>>vec[1]>>vec[2];
      v.push_back(vec);
    } else if(tok==fTok) {
      bool hasTexture = false;
      if (line.find(bslash) != std::string::npos) {
        std::replace(line.begin(), line.end(), bslash, space);
        hasTexture = true;
      }
      std::stringstream facess(line);
      facess>>tok;
      std::vector<int> vidx;
      std::vector<int> texIdx;
      int x;
      while(facess>>x){
        vidx.push_back(x);
        if(hasTexture){
          facess>>x;
          texIdx.push_back(x);
        }
      }
      texIdx.resize(vidx.size());
      for(int ii = 0;ii<vidx.size()-2;ii++){
        Eigen::Vector3i trig, textureId;
        trig[0] = vidx[0]-1;
        textureId[0] = texIdx[0]-1;
        for (int jj = 1; jj < 3; jj++) {
          trig[jj] = vidx[ii+jj]-1;
          textureId[jj] = texIdx[ii+jj]-1;
        }
        t.push_back(trig);
        texId.push_back(textureId);
      }
    } else if(tok==texTok) {
      Vector2f texcoord;
      ss>>texcoord[0];
      ss>>texcoord[1];
      tex.push_back(texcoord);
    }
  }
  std::cout<<"Num Triangles: "<< t.size()<<"\n";
}

void Mesh::read_ply(std::ifstream & f)
{
  std::string line;
  std::string vertLine("element vertex");
  std::string faceLine("element face");
  std::string texLine("property float s");
  std::string endHeaderLine("end_header");
  while(true) {
    std::getline(f,line);
    if(std::string::npos!=line.find(vertLine)) {
      break;
    }
  }
  std::string token;
  std::stringstream ss(line);
  ss>>token>>token;
  int nvert;
  ss>>nvert;
  bool hasTex=false;
  while(true) {
    std::getline(f,line);
    if(std::string::npos!=line.find(faceLine)) {
      break;
    }
    if(std::string::npos!=line.find(texLine)) {
      hasTex=true;
    }
  }
  std::stringstream ss1(line);
  ss1>>token>>token;
  int nface;
  ss1>>nface;
  while(true) {
    std::getline(f,line);
    if(std::string::npos!=line.find(endHeaderLine)) {
      break;
    }
  }

  v.resize(nvert);
  t.resize(nface);
  if(hasTex) {
    tex.resize(nvert);
  }
  for (int ii =0; ii<nvert; ii++) {
    for (int jj=0; jj<3; jj++) {
      f>>v[ii][jj];
    }
    if(hasTex) {
      for (int jj=0; jj<2; jj++) {
        f>>tex[ii][jj];
      }
      tex[ii][1]=1-tex[ii][1];;
    }
  }
  for (int ii =0; ii<nface; ii++) {
    int nidx;
    f>>nidx;
    for (int jj=0; jj<3; jj++) {
      f>>t[ii][jj];
    }
  }
}

void int2b(unsigned int x, GLubyte * b)
{
  for(int ii=0;ii<4;ii++){
    b[ii]=x&0xff;
    x=x>>8;
  }
}

unsigned int b2int(GLubyte * b)
{
  unsigned int x=0;
  for(int ii=3;ii>=0;ii--){
    x=x<<8;
    x+=b[ii];
  }
  return x;
}

void Mesh::save_obj(const char * filename, bool flip  )
{
  std::ofstream out(filename);
  if(!out.good()){
    std::cout<<"cannot open output file"<<filename<<"\n";
    return;
  }
  int flip1= 0, flip2 = 1;
  if(flip){
    flip1 = 1;
    flip2 = 0;
  }
  std::string vTok("v");
  std::string fTok("f");
  std::string texTok("vt");
  char bslash='/';
  std::string tok;
  for(size_t ii=0;ii<v.size();ii++){
    out<<vTok<<" "<<v[ii][0]<<" "<<v[ii][1]<<" "<<v[ii][2]<<"\n";
  }
  if(tex.size()>0){
    for(size_t ii=0;ii<tex.size();ii++){
      out<<texTok<<" "<<tex[ii][0]<<" "<<tex[ii][1]<<"\n";
    }
    for(size_t ii=0;ii<t.size();ii++){
      out<<fTok<<" "<<t[ii][flip1]+1<<bslash<<texId[ii][flip1]+1<<" "
      <<t[ii][flip2]+1<<bslash<<texId[ii][flip2]+1<<" "
      <<t[ii][2]+1<<bslash<<texId[ii][2]+1<<"\n";
    }
  }else{
    for(size_t ii=0;ii<t.size();ii++){
      out<<fTok<<" "<<t[ii][flip1]+1<<" "<<
          t[ii][flip2]+1<<" "<<t[ii][2]+1<<"\n";
    }
  }

  out.close();
}

Mesh::Mesh(const char * filename,bool normalize)
{
  load_mesh(filename,normalize);
}


void Mesh::load_mesh(const char * filename, bool normalize)
{
  std::ifstream f ;
  f.open(filename);
  if(!f.is_open()) {
    std::cout<<"Error: cannot open mesh "<<filename<<"\n";
    return;
  }
  switch(filename[strlen(filename)-1]) {
  case 'y':
    read_ply(f);
    break;
  case 'j':
    read_obj(f);
    break;
  default:
    break;
  }
  if(normalize){
    rescale();
  }
  compute_norm();

  f.close();
}

void Mesh::rescale()
{
  if(v.size()==0){
    std::cout<<"empty mesh\n";
    return;
  }
  Vector3f mn=v[0],mx=v[0];

  //scale and translate to [0 , 1]
  for (unsigned int dim = 0; dim<3; dim++) {
    for( size_t ii=0; ii<v.size(); ii++) {
      mn[dim]= std::min(v[ii][dim],mn[dim]);
      mx[dim] = std::max(v[ii][dim],mx[dim]);
    }
    real_t translate = -mn[dim];
    for(size_t ii=0; ii<v.size(); ii++) {
      v[ii][dim]=(v[ii][dim]+translate);
    }
  }

  real_t scale = 1/(mx[0]-mn[0]);
  for(unsigned int dim=1; dim<3; dim++) {
    scale=std::min(1/(mx[dim]-mn[dim]),scale);
  }

  for(size_t ii=0; ii<v.size(); ii++) {
    for (unsigned int dim = 0; dim<3; dim++) {
      v[ii][dim]=v[ii][dim]*scale;
    }
  }
}

void Mesh::compute_norm()
{
  n.resize(v.size());
  for(unsigned int ii=0; ii<t.size(); ii++) {
    Vector3f a = v[t[ii][1]] - v[t[ii][0]];
    Vector3f b = v[t[ii][2]] - v[t[ii][0]];
    b=Vector3f::cross(a,b);
    b.normalize();
    for(int jj=0; jj<3; jj++) {
      n[t[ii][jj]]+=b;
      if(t[ii][jj]>=(int)n.size() || t[ii][jj]<0){
        std::cout<<ii<<" "<<jj<<" "<<t[ii][jj]<<" normal computation error\n";
      }
    }
  }
  for(unsigned int ii=0; ii<v.size(); ii++) {
    n[ii].normalize();
  }
}

void Mesh::drawWire()
{
  glColor3f(0.3,0.3,0.2);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);

  for(unsigned int ii=0; ii<t.size(); ii++) {
    glVertex3f(v[t[ii][0]][0],v[t[ii][0]][1],v[t[ii][0]][2]);
    glVertex3f(v[t[ii][1]][0],v[t[ii][1]][1],v[t[ii][1]][2]);

    glVertex3f(v[t[ii][1]][0],v[t[ii][1]][1],v[t[ii][1]][2]);
    glVertex3f(v[t[ii][2]][0],v[t[ii][2]][1],v[t[ii][2]][2]);
  }
  glEnd();
  glEnable(GL_LIGHTING);
}

void Mesh::draw(std::vector<Vector3f>&v)
{
  glDisable(GL_LIGHTING);
  //glUseProgramObjectARB(prog);
  GLfloat specular[4]= {0.51f,0.51f,0.51f,1.0f};
  GLfloat ambient[4]= {0.3f,0.3f,0.3f,1.0f};
  GLfloat diffuse[4]= {1.0f,1.0f,1.0f,1.0f};

  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
//  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  Vector3f mn,mx;
  BBox(v,mn,mx);
  glPushMatrix();
  mx = mx-mn;
  //float scale = std::max(mx[0],mx[1]);
  //scale = 0.05;//std::max(mx[2],scale);
  //glScalef(1.0f/scale, 1.0f/scale, 1.0f/scale);
  //glTranslatef(-mn[0],-mn[1],-mn[2]);
  GLfloat s=10;
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&s);
  if(texture.valid()) {
    glBindTexture(GL_TEXTURE_2D,glTexID);
  }else{
    glDisable(GL_TEXTURE_2D);
  }
  glBegin(GL_TRIANGLES);

  for(unsigned int ii=0; ii<t.size(); ii++) {
    if(texture.valid() && tex.size()>0 && texId.size()>0) {
      glTexCoord2f(tex[texId[ii][0]][0],tex[texId[ii][0]][1]);
      glNormal3f(n[t[ii][0]][0],n[t[ii][0]][1],n[t[ii][0]][2]);
      glVertex3f(v[t[ii][0]][0],v[t[ii][0]][1],v[t[ii][0]][2]);

      glTexCoord2f(tex[texId[ii][1]][0],tex[texId[ii][1]][1]);
      glNormal3f(n[t[ii][1]][0],n[t[ii][1]][1],n[t[ii][1]][2]);
      glVertex3f(v[t[ii][1]][0],v[t[ii][1]][1],v[t[ii][1]][2]);

      glTexCoord2f(tex[texId[ii][2]][0],tex[texId[ii][2]][1]);
      glNormal3f(n[t[ii][2]][0],n[t[ii][2]][1],n[t[ii][2]][2]);
      glVertex3f(v[t[ii][2]][0],v[t[ii][2]][1],v[t[ii][2]][2]);
    } else {
      glNormal3f(n[t[ii][0]][0],n[t[ii][0]][1],n[t[ii][0]][2]);
      glVertex3f(v[t[ii][0]][0],v[t[ii][0]][1],v[t[ii][0]][2]);
      glNormal3f(n[t[ii][1]][0],n[t[ii][1]][1],n[t[ii][1]][2]);
      glVertex3f(v[t[ii][1]][0],v[t[ii][1]][1],v[t[ii][1]][2]);
      glNormal3f(n[t[ii][2]][0],n[t[ii][2]][1],n[t[ii][2]][2]);
      glVertex3f(v[t[ii][2]][0],v[t[ii][2]][1],v[t[ii][2]][2]);
    }
  }
  glEnd();
  glPopMatrix();
  if(texture.valid()) {
    glBindTexture(GL_TEXTURE_2D,0);
  }else{
    glEnable(GL_TEXTURE_2D);
  }

}

#ifdef USE_SHADER
GLcharARB * read_entire_file(const char * filename, int * len )
{
  FILE * file = fopen(filename, "r");
  if(!file){
    printf("cannot open shader %s\n", filename);
    return 0;
  }
  GLcharARB * buf=0;
  fseek(file, 0, SEEK_END);
	size_t length = ftell(file);
	fseek(file, 0, SEEK_SET);
	buf = new GLcharARB[length+1];
  length = fread( buf, 1, length, file);
  buf[length]=0;
  *len=length;
  return buf;
}

void printInfoLog(GLhandleARB obj)
{
    GLint infologLength = 0;
    GLsizei charsWritten  = 0;
    char *infoLog;

	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                         &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}
#endif

void BBox(const std::vector<Vector3f >& v, Vector3f & mn, Vector3f & mx)
{
  mn = v[0];
  mx = v[0];
  for(unsigned int ii = 1 ;ii<v.size();ii++){
    for(int dim = 0 ; dim<3;dim++){
      if(v[ii][dim]<mn[dim]){
        mn[dim] = v[ii][dim];
      }
      if(v[ii][dim]>mx[dim]){
        mx[dim] = v[ii][dim];
      }
    }
  }
}

void BBox(const Mesh & m, Vector3f & mn, Vector3f & mx)
{
  BBox(m.v,mn,mx);
}


bool is_nbr(const Eigen::Vector3i & a, const Eigen::Vector3i&b, int vert)
{
  for (int ii=0; ii<3; ii++) {

    int va=a[ii];
    if(va<=vert) {
      continue;
    }

    for (unsigned int jj=0; jj<3; jj++) {
      int vb=b[jj];
      if(vb<=vert) {
        continue;
      }
      if(va==vb) {
        return true;
      }
    }
  }
  return false;
}


void adjlist(const Mesh & m, std::vector<std::vector<int> > & adjMat)
{
  if(adjMat.size()==m.t.size()) {
    return;
  }
  std::vector<std::vector<int> >trigList;
  trigList.resize(m.v.size());
  for (unsigned int ii=0; ii<m.t.size(); ii++) {
    for (unsigned int jj=0; jj<3; jj++) {
      int vidx=m.t[ii][jj];
      trigList[vidx].push_back(ii);
    }
  }
  adjMat.resize(m.t.size());
  for (unsigned int ii=0; ii<m.v.size(); ii++) {
    int n_nbr=trigList[ii].size();
    for (int jj=0; jj<n_nbr; jj++) {
      int tj=trigList[ii][jj];
      for (int kk=(jj+1); kk<n_nbr; kk++) {
        int tk=trigList[ii][kk];
        if(is_nbr(m.t[tj],m.t[tk],ii)) {
          adjMat[tj].push_back(tk);
          adjMat[tk].push_back(tj);
        }

      }
    }
  }
}

