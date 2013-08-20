/*
 * Render.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */

#include "Util/Render.hpp"
#include "Util/glheader.hpp"

void Render::init(World * _world)
{
  world = _world;
  glClearColor (1, 1, 1, 0.0);
  // glShadeModel (GL_SMOOTH);
  //glShadeModel (GL_FLAT );
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA);
  // glEnable(GL_NORMALIZE);
  GLfloat white[]={1.0,1.0,1.0,1.0};
  GLfloat grey[]={0.3,0.3,0.3,1.0};

  glLightfv (GL_LIGHT1, GL_DIFFUSE, white);
  glLightfv (GL_LIGHT1, GL_SPECULAR, white);
  glLightfv (GL_LIGHT0, GL_AMBIENT, grey);

}

Render::Render():world(0)
{}

Render::~Render()
{}
