//Ava Chong
//CS449
//Program Purpose: BMP Image processing and manipulation

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#pragma pack(1)

// function prototypes
void imageTransform(char *inputFile, char *outputFile, int flag);
void help();
void error(char str[]);       //error msg

//struct definitions
struct BMP_header{
  unsigned char formatId[2];     //format identifier
  unsigned char fileSize[4];     //file size
  char res1[2];         //two byte reserved val
  char res2[2];         //second two byte reserved val
  int offset;           //offset to start pixel array
};

struct DIB_header{
  unsigned int size;         //first four bytes: size of DIB header in bytes
  unsigned int width;        //width of image in pixels
  unsigned int height;       //height of image in pixels
  char numPlanes[2];    //num of color planes
  short numPixel;     //number of bits per pixel
  char comp[4];         //compression scheme used
  char imageSize[4];    //image size in bytes
  char hRes[4];         //horizontal resolution
  char vRes[4];         //vertical resolution
  char colors[4];       //num of colors used
  char impColors[4];    //num of important colors??
};

struct pixel{
  unsigned char blue;     //blue val
  unsigned char green;    //green val
  unsigned char red;      //red val 
};

int main(int argc, char *argv[]) {
  char *inputFile = argv[argc -1];
  int flg = 1;
  float th[5]  = {-1.0,0,0,0,0};

  // Check for arguments passed
  if(argc == 1)
  {
    error("No input found");
  }
  // grayscale mode
  else if(strcmp(argv[1],"-grayscale") == 0)
  {
    flg = 1;
    imageTransform(inputFile, inputFile, flg);
  }
  // invert mode
  else if(strcmp(argv[1],"-invert") == 0)
  {
    flg = 2;
    imageTransform(inputFile, inputFile, flg);
  } else {
    error("Bad input. use '-grayscale' or '-invert'\n");
  }
}

void imageTransform(char *inputFile, char *outputFile, int flag)
{
  int padding;
  struct BMP_header BMP;
  struct DIB_header DIB;
  struct pixel color;

  // read file contents till end of file 
  FILE *rfile = fopen(inputFile, "r+b");
  if (rfile == NULL) error("bad file");
  fread(&BMP, sizeof(struct BMP_header), 1, rfile);

  //Check to see if format ident is BM
  if (BMP.formatId[0] != 'B' || BMP.formatId[1] != 'M') {
    error("problem reading file: file not supported\n");
  }

  //read DIB header
  fread(&DIB, sizeof(struct DIB_header), 1, rfile);
  if (sizeof(DIB) != 40) error("problem readubng file. format not supported");

  //image dimensions
  int width = DIB.width;
  int height = DIB.height;
  short numPix = DIB.numPixel;
  if (numPix != 24) error ("wrong byetes per pixel\n");
  
  // Calculate padding
  padding = (4 - (width % 4));

  //grayscale
  if(flag == 1)
  {
    fseek(rfile, BMP.offset, SEEK_SET);             //read file at offset
    for (int i = 0; i < height; i++){
      for (int j = 0; j < width; j++){
        fread(&color, sizeof(color), 1, rfile);     //read pixel
        double R = (color.red/255.0);               //calculate grayscale val
        double B = (color.blue/255.0);
        double G = (color.green/255.0);  
        double Y = 0.2126*R + 0.7152*G + 0.0722*B;
        if(Y > 0.0031308){
          double ans = 1.055*pow(Y, 1/2.4)-0.055; 
          color.red = ans*255;
          color.blue = ans*255;
          color.green = ans*255;
        } else{
          double ans = Y*12.92;
          color.red = ans*255;
          color.blue = ans*255;
          color.green = ans*255;
        }    
        fseek(rfile, -3, SEEK_CUR);               //set pointer back 
        fwrite(&color, sizeof(color), 1, rfile);  //overwrite
      }
      if (DIB.width%4 != 0){
        fseek(rfile, padding, SEEK_CUR);            //include padding
      }
      }
      
  }

  // INVERT COLOR 
  else if(flag == 2)
  {
    fseek(rfile, BMP.offset, SEEK_SET);         //read from offset
    for (int i = 0; i < height; i++){
     for (int j = 0; j < width; j++){
        fread(&color, sizeof(color), 1, rfile);   //read pixel
        color.red = ~color.red;                   //invert pixel
        color.blue = ~color.blue;
        color.green = ~color.green;
        fseek(rfile, -3, SEEK_CUR);               //set pointer back
        fwrite(&color, sizeof(color), 1, rfile);
      }
      if (DIB.width%4 != 0){
        fseek(rfile, padding, SEEK_CUR);
      }
    }
  }
    fclose (rfile); 
 }
  void error(char str[])      //error functions
  {
     printf ("Error : %s\n",str);
     exit(1);
  }
