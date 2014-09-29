#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>
#include <png.h>
#include <cstdio>
#include <string>

GLuint loadTexture(const std::string filename, int &width, int &height); 

#endif //TEXTURE_H
