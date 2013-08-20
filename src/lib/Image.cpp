#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "vecmath.h"
#include "Util/Image.h"
#include "Util/Texture.hpp"
Vector3f
Image::getPixel(float x, float y) const
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
  Vector3f colors[4];
  colors[0] = GetPixel(ix, iy);
  colors[1] = GetPixel(ix + 1, iy);
  colors[2] = GetPixel(ix, iy + 1);
  colors[3] = GetPixel(ix + 1, iy + 1);
  Vector3f final;
  float alpha = x - ix;
  float beta = y - iy;
  for (int ii = 0; ii < 3; ii++) {
    final[ii] = (1 - alpha) * (1 - beta) * colors[0][ii]
        + alpha * (1 - beta) * colors[1][ii]
        + (1 - alpha) * beta * colors[2][ii]
        + alpha * beta * colors[3][ii];
  }
  return final;
}

void Image::loadPNG(const char * filename)
{

  Texture t;
  t.load(filename);
  if(data!=0){
    delete data;
  }
  width = t.getWidth();
  height = t.getHeight();
  data = new Vector3f[width*height];
  int idx =0 ;
  for(int ii =0 ;ii<height;ii++){
    for (int jj = 0; jj < width; jj++) {
      unsigned char buf[4];
      t(ii,jj,buf);
      Vector3f color(buf[0]/255.0f,buf[1]/255.0f,buf[2]/255.0f);
      SetPixel(ii,jj,color);
    }
  }
}

void Image::addImage(const Image & image)
{
  for(int ii =0 ;ii<height;ii++){
    for(int jj =0 ;jj<width;jj++){
      data[IX(jj,ii)] += image.GetPixel(jj,ii);
    }
  }
}

void Image::addColor(float x, float y, const Vector3f& color)
{
  int ix = (int)(x+0.5);
  int iy = (int)(y+0.5);
  float area = 0 ;
  
  //lower left
  if(inbound(ix-1, iy-1)){
    area = (ix - x + 0.5)*(iy-y+0.5);
    data[ IX(ix-1, iy-1) ] += area * color;
  }
  
  //lower right
  if(inbound(ix, iy-1)){
    area = (x + 0.5-ix)*(iy-y+0.5);
    data[ IX(ix, iy-1) ] += area * color;
  }
  
  //upper left
  if(inbound(ix-1, iy)){
    area = (ix - x + 0.5)*(y+0.5-iy);
    data[ IX(ix-1, iy) ] += area * color;
  }
  
  //upper right
  if(inbound(ix, iy)){
    area = (x + 0.5-ix)*(y+0.5-iy);
    data[ IX(ix, iy) ] += area * color;
  }
}

void Image::drawLine(int x0, int x1, int y0,int y1)
{
  int dx = abs(x1-x0);
  int dy = abs(y1-y0); 
  int sx, sy;
  if( x0 < x1 ){ sx = 1;} else{ sx = -1;}
   if( y0 < y1){ sy = 1;} else{ sy = -1;}
   int err = dx-dy;
 
   while(true){
     SetPixel(x0,y0,Vector3f(1,1,1));
     if (x0 == x1 && y0 == y1 ) {
       break;
     }
     int e2 = 2*err;
     if (e2 > -dy ){ 
       err = err - dy;
       x0 = x0 + sx;
     }
     if (e2 <  dx ){ 
       err = err + dx;
       y0 = y0 + sy ;
     }
   }
}

// some helper functions for save & load

unsigned char ReadByte( FILE* file)
{
    unsigned char b;
    int success = fread( ( void* )( &b ), sizeof( unsigned char ), 1, file );
    assert( success == 1 );
    return b;
}

void WriteByte( FILE* file, unsigned char b )
{
    int success = fwrite( ( void* )( &b ), sizeof( unsigned char ), 1, file );
    assert( success == 1 );
}

unsigned char ClampColorComponent( float c )
{
    int tmp = int( c * 255 );
    
    if( tmp < 0 )
    {
        tmp = 0;
    }
    
    if( tmp > 255 )
    {
        tmp = 255;
    }

    return ( unsigned char )tmp;
}

// Save and Load data type 2 Targa (.tga) files
// (uncompressed, unmapped RGB images)

void Image::saveTGA( const char* filename) const
{
    assert( filename != NULL );
    // must end in .tga
    const char* ext = &filename[ strlen( filename ) - 4 ];
    assert( !strcmp( ext,".tga" ) );
    FILE* file = fopen( filename, "wb" );
    // misc header information
    for( int i = 0; i < 18; i++)
    {
        if (i == 2) WriteByte(file,2);
        else if (i == 12) WriteByte(file,width%256);
        else if (i == 13) WriteByte(file,width/256);
        else if (i == 14) WriteByte(file,height%256);
        else if (i == 15) WriteByte(file,height/256);
        else if (i == 16) WriteByte(file,24);
        else if (i == 17) WriteByte(file,32);
        else WriteByte(file,0);
    }
    // the data
    // flip y so that (0,0) is bottom left corner
    for (int y = height-1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            Vector3f v = GetPixel(x,y);
            // note reversed order: b, g, r
            WriteByte(file,ClampColorComponent(v[2]));
            WriteByte(file,ClampColorComponent(v[1]));
            WriteByte(file,ClampColorComponent(v[0]));
        }
    }
    fclose(file);
}

Image* Image::LoadTGA(const char *filename) {
    assert(filename != NULL);
    // must end in .tga
    const char *ext = &filename[strlen(filename)-4];
    assert(!strcmp(ext,".tga"));
    FILE *file = fopen(filename,"rb");
    // misc header information
    int width = 0;
    int height = 0;
    for (int i = 0; i < 18; i++) {
        unsigned char tmp;
        tmp = ReadByte(file);
        if (i == 2) assert(tmp == 2);
        else if (i == 12) width += tmp;
        else if (i == 13) width += 256*tmp;
        else if (i == 14) height += tmp;
        else if (i == 15) height += 256*tmp;
        else if (i == 16) assert(tmp == 24);
        else if (i == 17) assert(tmp == 32);
        else assert(tmp == 0);
    }
    // the data
    Image *answer = new Image(width,height);
    // flip y so that (0,0) is bottom left corner
    for (int y = height-1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            unsigned char r,g,b;
            // note reversed order: b, g, r
            b = ReadByte(file);
            g = ReadByte(file);
            r = ReadByte(file);
            Vector3f color(r/255.0,g/255.0,b/255.0);
            answer->SetPixel(x,y,color);
        }
    }
    fclose(file);
    return answer;
}

// Save and Load PPM image files using magic number 'P6' 
// and having one comment line

void Image::savePPM(const char *filename) const {
    assert(filename != NULL);
    // must end in .ppm
    const char *ext = &filename[strlen(filename)-4];
    assert(!strcmp(ext,".ppm"));
    FILE *file = fopen(filename, "w");
    // misc header information
    assert(file != NULL);
    fprintf (file, "P6\n");
    fprintf (file, "# Creator: Image::SavePPM()\n");
    fprintf (file, "%d %d\n", width,height);
    fprintf (file, "255\n");
    // the data
    // flip y so that (0,0) is bottom left corner
    for (int y = height-1; y >= 0; y--) {
        for (int x=0; x<width; x++) {
            Vector3f v = GetPixel(x,y);
            fputc (ClampColorComponent(v[0]),file);
            fputc (ClampColorComponent(v[1]),file);
            fputc (ClampColorComponent(v[2]),file);
        }
    }
    fclose(file);
}

Image* Image::LoadPPM(const char *filename) {
    assert(filename != NULL);
    // must end in .ppm
    const char *ext = &filename[strlen(filename)-4];
    assert(!strcmp(ext,".ppm"));
    FILE *file = fopen(filename,"rb");
    // misc header information
    int width = 0;
    int height = 0;  
    char tmp[100];
    fgets(tmp,100,file); 
    assert (strstr(tmp,"P6"));
    fgets(tmp,100,file); 
    assert (tmp[0] == '#');
    fgets(tmp,100,file); 
    sscanf(tmp,"%d %d",&width,&height);
    fgets(tmp,100,file); 
    assert (strstr(tmp,"255"));
    // the data
    Image *answer = new Image(width,height);
    // flip y so that (0,0) is bottom left corner
    for (int y = height-1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            unsigned char r,g,b;
            r = fgetc(file);
            g = fgetc(file);
            b = fgetc(file);
            Vector3f color(r/255.0,g/255.0,b/255.0);
            answer->SetPixel(x,y,color);
        }
    }
    fclose(file);
    return answer;
}

Image* Image::compare(Image* img1, Image* img2) {
    assert (img1->Width() == img2->Width());
    assert (img1->Height() == img2->Height());

    Image* img3 = new Image(img1->Width(), img1->Height());

    for (int x = 0; x < img1->Width(); x++) {
        for (int y = 0; y < img1->Height(); y++) {
            Vector3f color1 = img1->GetPixel(x, y);
            Vector3f color2 = img2->GetPixel(x, y);
            Vector3f color3 =
                Vector3f
                (
                    fabs(color1[0] - color2[0]),
                    fabs(color1[1] - color2[1]),
                    fabs(color1[2] - color2[2])
                );
            img3->SetPixel(x, y, color3);
        }
    }

    return img3;
}
/****************************************************************************
    bmp.c - read and write bmp images.
    Distributed with Xplanet.  
    Copyright (C) 2002 Hari Nair <hari@alumni.caltech.edu>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
****************************************************************************/
struct BMPHeader
{
    char bfType[3];       /* "BM" */
    int bfSize;           /* Size of file in bytes */
    int bfReserved;       /* set to 0 */
    int bfOffBits;        /* Byte offset to actual bitmap data (= 54) */
    int biSize;           /* Size of BITMAPINFOHEADER, in bytes (= 40) */
    int biWidth;          /* Width of image, in pixels */
    int biHeight;         /* Height of images, in pixels */
    short biPlanes;       /* Number of planes in target device (set to 1) */
    short biBitCount;     /* Bits per pixel (24 in this case) */
    int biCompression;    /* Type of compression (0 if no compression) */
    int biSizeImage;      /* Image size, in bytes (0 if no compression) */
    int biXPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biYPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biClrUsed;        /* Number of colors in the color table (if 0, use 
                             maximum allowed by biBitCount) */
    int biClrImportant;   /* Number of important colors.  If 0, all colors 
                             are important */
};
int 
Image::saveBMP(const char *filename)
{
    int i, j, ipos;
    int bytesPerLine;
    unsigned char *line;
	Vector3f*rgb = data;
    FILE *file;
    struct BMPHeader bmph;

    /* The length of each line must be a multiple of 4 bytes */

    bytesPerLine = (3 * (width + 1) / 4) * 4;

    strcpy(bmph.bfType, "BM");
    bmph.bfOffBits = 54;
    bmph.bfSize = bmph.bfOffBits + bytesPerLine * height;
    bmph.bfReserved = 0;
    bmph.biSize = 40;
    bmph.biWidth = width;
    bmph.biHeight = height;
    bmph.biPlanes = 1;
    bmph.biBitCount = 24;
    bmph.biCompression = 0;
    bmph.biSizeImage = bytesPerLine * height;
    bmph.biXPelsPerMeter = 0;
    bmph.biYPelsPerMeter = 0;
    bmph.biClrUsed = 0;       
    bmph.biClrImportant = 0; 

    file = fopen (filename, "wb");
    if (file == NULL) return(0);
  
    fwrite(&bmph.bfType, 2, 1, file);
    fwrite(&bmph.bfSize, 4, 1, file);
    fwrite(&bmph.bfReserved, 4, 1, file);
    fwrite(&bmph.bfOffBits, 4, 1, file);
    fwrite(&bmph.biSize, 4, 1, file);
    fwrite(&bmph.biWidth, 4, 1, file);
    fwrite(&bmph.biHeight, 4, 1, file);
    fwrite(&bmph.biPlanes, 2, 1, file);
    fwrite(&bmph.biBitCount, 2, 1, file);
    fwrite(&bmph.biCompression, 4, 1, file);
    fwrite(&bmph.biSizeImage, 4, 1, file);
    fwrite(&bmph.biXPelsPerMeter, 4, 1, file);
    fwrite(&bmph.biYPelsPerMeter, 4, 1, file);
    fwrite(&bmph.biClrUsed, 4, 1, file);
    fwrite(&bmph.biClrImportant, 4, 1, file);
  
    line = (unsigned char *)malloc(bytesPerLine);
    if (line == NULL)
    {
        fprintf(stderr, "Can't allocate memory for BMP file.\n");
        return(0);
    }

    for (i = 0; i < height ; i++)
    {
        for (j = 0; j < width; j++)
        {
            ipos = (width * i + j);
            line[3*j] = ClampColorComponent(rgb[ipos][2]);
            line[3*j+1] =ClampColorComponent( rgb[ipos][1]);
            line[3*j+2] = ClampColorComponent( rgb[ipos][0]);
        }
        fwrite(line, bytesPerLine, 1, file);
    }

    free(line);
    fclose(file);

    return(1);
}

void Image::saveImage(const char * filename)
{
	int len = strlen(filename);
	if(strcmp(".bmp", filename+len-4)==0){
		saveBMP(filename);
	}else{
		saveTGA(filename);
	}
}
