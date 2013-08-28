/*
 * Render.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */

#include "Element/ElementMesh.hpp"
#include "Util/Render.hpp"
#include "Util/glheader.hpp"
#include "World/Camera.hpp"
#include "World/World.hpp"
#include "World/Mesh.hpp"
#include "vecmath.h"
#include <math.h>
static Quat4f rot;
World * wd;
Camera * cam;
int ldown;
int oldx,oldy;

void reshape (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
}

void keyboard(unsigned char key,int x, int y)
{
  switch(key){
  case 'w':
    cam->eye[1]+=0.1;
    break;
  case 's':
    cam->eye[1]-=0.1;
    break;
  case 'a':
    cam->eye[0]-=0.1;
    break;
  case 'd':
    cam->eye[0]+=0.1;
    break;
  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      ldown = 1;
      oldx = x;
      oldy = y;
      break;
    case GLUT_UP:
      ldown = 0;
      break;
    }
    break;
  default:
    break;
  }
}

void motion (int x, int y)
{
  if(ldown){
      Quat4f q;
      q.setAxisAngle((x-oldx)/60.0,Vector3f(0,0,1));
      rot =q*rot;
      rot.normalize();
      q.setAxisAngle((y-oldy)/60.0, Vector3f(1,0,0));
      rot =q*rot;
      rot.normalize();
      oldx=x;
      oldy=y;
      glutPostRedisplay();
    }
}

void animate(int t)
{
  glutTimerFunc(60, animate, 0);
  glutPostRedisplay();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cam->eye[0], cam->eye[1], cam->eye[2],

  cam->at[0], cam->at[1], cam->at[2], 0.0, 0.0, 0.1);
  GLfloat position[] =
  { 2.0, 2, 2, 1.0 };
  GLfloat position1[] =
  { -1.0, -1, -1, 1.0 };

  glPushMatrix();
  glDisable(GL_LIGHTING);
  glColor3f(0, 0, 0);
  glTranslatef(position[0], position[1], position[2]);
  glutWireCube(0.2);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(position1[0], position1[1], position1[2]);
  glutWireCube(0.2);
  glEnable(GL_LIGHTING);
  glPopMatrix();

  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glLightfv(GL_LIGHT1, GL_POSITION, position1);

  Vector3f axis;
  float angle;
  glPushMatrix();
  axis = rot.getAxisAngle(&angle);
  angle = angle * 180 / M_PI;

  glRotatef(angle, axis[0], axis[1], axis[2]);
  glTranslatef(-0.5, -0.5, -0.5);

  for(size_t ii =0; ii<wd->mesh.size();ii++){
    wd->mesh[ii]->draw(wd->mesh[ii]->v);
  }
  for (size_t ii = 0; ii < wd->element.size(); ii++) {
    wd->element[ii]->DrawOpenGL();
  }
  glPopMatrix();

  GLfloat floorCol[4] =
  { 1, 1, 1, 1 };
  glEnable(GL_LIGHTING);
  glMaterialfv(GL_FRONT, GL_SPECULAR, floorCol);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, floorCol);
  GLfloat s = 10;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &s);
  glBegin(GL_TRIANGLE_STRIP);

  glNormal3f(0, 1, 0);
  glVertex3f( -1.0f, -1.0f, -.5f );
  glVertex3f( -1.0f,  1.0f, -.5f );
  glVertex3f(  1.0f, -1.0f, -.5f );
  glVertex3f(  1.5f,  1.0f, -.5f );
  glEnd();
  glFlush();
}

void Render::init(World * _world)
{
  world = _world;
  int glutArgc=0;
  glutInit(&glutArgc, 0);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH|GLUT_ALPHA );
  glutInitWindowSize (800 , 600);
  glutCreateWindow ("title");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(0.1, animate, 0);

  glClearColor (1, 1, 1, 0.0);
  glShadeModel (GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA);

  GLfloat white[]={1.0,1.0,1.0,1.0};
  GLfloat grey[]={0.3,0.3,0.3,1.0};

  glLightfv (GL_LIGHT1, GL_DIFFUSE, white);
  glLightfv (GL_LIGHT1, GL_SPECULAR, white);
  glLightfv (GL_LIGHT0, GL_AMBIENT, grey);
  wd = world;
  cam = &world->camera;
  rot.setAxisAngle(0,Vector3f(0,0,1));
  ldown=0;
}

void Render::loop()
{
  glutMainLoop();
}

Render::Render():world(0)
{}

Render::~Render()
{}
