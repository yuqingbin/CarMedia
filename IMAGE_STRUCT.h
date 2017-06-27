#ifndef IMAGE_STRUCT_H
#define IMAGE_STRUCT_H
#include "PIXEL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
class IMAGE_STRUCT
{
   public:

        int x_size;
        int y_size;
        FILE *file;
        char *fname;
        PIXEL *pixel;
        int num_colors;
        int bytes_per_pixel;
        int numOfStuff_inRow;

         unsigned char *colormap; //int colormap;
        //PIXEL **bmp2D;
       IMAGE_STRUCT()
        {
                x_size=0;
                y_size=0;
                file=NULL;
                num_colors=0;
                bytes_per_pixel=0;
                numOfStuff_inRow=0;
                pixel=NULL;


        }
        ~IMAGE_STRUCT()
        {
             if(pixel!=NULL)
                free(pixel);
              pixel=NULL;
              file=NULL;

        }
        void IMAGE_STRUCT_resetMEM()
        {
            int numOfPixel=x_size*y_size;
            PIXEL *image_data; //temp pointer
            image_data=this->pixel;
           // memset((PIXEL *)this->pixel,0,numOfPixel*sizeof(PIXEL));
           for(int i=0;i<numOfPixel;i++)
           {
                (image_data+i)->red=0;
                (image_data+i)->green=0;
                (image_data+i)->blue=0;
           }
        }
};
#endif

