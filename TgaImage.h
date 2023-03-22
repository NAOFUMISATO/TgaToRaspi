#pragma once
#include <iostream>
#include <glut.h>

class TGAImage
{
protected:
   GLuint imageSize;
   GLubyte* imageData;
   GLenum format;
   GLuint internalFormat;
   GLuint width;
   GLuint height;
   GLuint bpp;

public:
   GLuint ID;
   TGAImage() {};
   ~TGAImage() {};
   bool ReadTGA(const char* filename);
   GLuint Load(const char* filename);
};