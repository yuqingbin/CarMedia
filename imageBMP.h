#ifndef _IMAGEBMP_H_
#define _IMAGEBMP_H_
#include "PIXEL.h"
#include "BMP_HEADER.h"
#include "IMAGE_STRUCT.h"

#include <stdio.h>
class imageBMP
{
   private:
        int MAC,MIN;
    public:
        //PIXEL **imageBMP2D;
        IMAGE_STRUCT *image_struct;
        //PIXEL pixel;
        BMP_HEADER bmp_header;
        imageBMP(int nRow,int nCol);
        imageBMP(char *fname,IMAGE_STRUCT *);
        ~imageBMP();

        unsigned char *Alloc_ImageColor(int);
        PIXEL *Alloc_Image(int);
        PIXEL **Alloc_BMPObj(int,int);
     

        void copydata_to_image(IMAGE_STRUCT *rgbPtr,
                        float **data,
                        long nrow);
        void copyPixel(IMAGE_STRUCT a,IMAGE_STRUCT b,int aD,int bD);
         PIXEL **convert1DTo2D(IMAGE_STRUCT *a);
         PIXEL *convert2DTo1D(IMAGE_STRUCT *a);
       


       
    
         void getPIXEL(IMAGE_STRUCT *input,int x,int y,PIXEL *cgopix);

     
          unsigned long getlong(FILE *);
         unsigned short getshort(FILE *);
         

         void putPIXEL(IMAGE_STRUCT *input,int x,int y,PIXEL *cpix);

         int putlong(FILE *,unsigned long);
         int putshort(FILE *,unsigned short);
         void putRowBMP(int i) {image_struct->x_size=i;};
         void putColBMP(int i){image_struct->y_size=i;};
    
         void Read_BMP(IMAGE_STRUCT *);
         void Read_BMP(char *,IMAGE_STRUCT *);

         int Read_BMP2D(char *,IMAGE_STRUCT *);
         int Read_BMP1D(char *,IMAGE_STRUCT *);
        

         int Write_BMP1D(IMAGE_STRUCT *);
         int Write_BMP2D(IMAGE_STRUCT *);
         int Write_BMP(IMAGE_STRUCT *output);
         int Write_BMP(IMAGE_STRUCT *output,char *fname);
         



};
#endif
/*end of imageBMP*/




