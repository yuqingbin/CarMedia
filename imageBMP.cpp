#include "imageBMP.h"

#include "BMP_HEADER.h"
#include "IMAGE_STRUCT.h"

#include "PIXEL.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string.h>
using namespace std;
using std::cout;
using std::endl; 
using std::cerr;
//extern "C"
//{
//    #include "commIMG_function.h"
//    #include "comm_function.h"
//}
/*#include DEBUG 1 */

 /*This function used for HLS_to_RGB() method*/
static double value(double n1,double n2,double hue);
imageBMP::imageBMP(int nRow,int nCol)
{
  this->image_struct=new IMAGE_STRUCT();

  this->image_struct->x_size=nCol;
  this->image_struct->y_size=nRow;
  this->image_struct->pixel=this->Alloc_Image(nRow*nCol); //Use for read 1D
  
}


imageBMP::imageBMP(char *fname,IMAGE_STRUCT *bmp)
{

      
       if(strstr(fname,".bmp")!=NULL)
       {
                bmp->file= fopen(fname,"r");    /*open file*/

                if(bmp->file == NULL)
                { /*check file*/
                        printf("\n\n *** ERROR -> Unable to read file %s!\n",fname);
                        exit(-1);
                 }

                this->Read_BMP(bmp);
                fclose(bmp->file);

       }
    
      else
      {
                        printf("This is not speciated image type \n");
                        exit(1);

      }
}


/*========================================================
 *
 *=======================================================*/
/*void imageBMP::setPixel(int r,int g,int b)
 {
     this->pixel.red=r;
      this->pixel.green=g;
      this->pixel.blue=b;
  */ //  this->pixel.greyvalue=(int)(0.29900*r+0.58700*g+0.111400*b);
  //    this->pixel.greyvalue=(int)(r+g+b)/3;
/* }
*/

void imageBMP::putPIXEL(IMAGE_STRUCT *input,int x,int y,PIXEL *cpix)
{
  PIXEL *image_data; //temp pixel pointer
  image_data=input->pixel;
  int i=x*(input->x_size)+y;
  (image_data+i)->red=cpix->red;
  (image_data+i)->green=cpix->green;
  (image_data+i)-> blue=cpix->blue;

}

void imageBMP::getPIXEL(IMAGE_STRUCT *input,int x,int y,PIXEL *cgopix)
{
  PIXEL *image_data; //temp pixel pointer
  image_data=input->pixel;
   int i=x*(input->x_size)+y;
//   cout<<"orgreen="<<(*(input->pixel+3)).green<<endl;
   cgopix->red=(image_data+i)->red;
  cgopix->green=(image_data+i)->green;
  cgopix->blue=(image_data+i)->blue;

}


void imageBMP::Read_BMP(char *fname,IMAGE_STRUCT *input)
{
    	input->file=fopen(fname,"r");

        if(input->file==(FILE *)NULL)
       {
	 fprintf(stderr,"Main:  Could not open '%s' as map file ",fname);
          exit(1);
        }
	else
	{
	//printf("Reading BMP map from %s\n",fname);
         this->Read_BMP(input);
 	}
	fclose(input->file);


}



/* keep remind b is num of pixel and each pixel is 3 bytes;
 * bytes_in_image (bfSize) divided by 3 is one pixel
 * rows*cols=nuber of pixels in image
 * Otherwise, You get some wrong
 */
void imageBMP::Read_BMP(IMAGE_STRUCT *input)
{

 	BMP_HEADER filehdr;

        unsigned char *color_map=NULL;     /*temp color map ptr */
 	PIXEL *image_data;    /*temp PIXEL ptr*/
 	int bytes_in_image, bytes_in_clrmap;
 	int i,b;
 	int padb;   /*num pad bytes */
 	int bprow;      /*bytes per row */
	int bpp;        /*bytes per pixel */
 	unsigned char mung;
 	/*read in header information */
 	filehdr.bfTypeC1=fgetc(input->file);
        filehdr.bfTypeC2=fgetc(input->file);

	if((filehdr.bfTypeC1!='B')||(filehdr.bfTypeC2!='M'))
	{
             fprintf(stderr,"File '%s' is not of type BMP \n",input->fname);
	     exit(1);
        }
 	 filehdr.bfSize=getlong(input->file);
         fseek(input->file,10L,0L);   /*Skip reserved works */
         filehdr.bfOffBits=getlong(input->file);
         filehdr.biSize=getlong(input->file);
         filehdr.biWidth=getlong(input->file);
         filehdr.biHeight=getlong(input->file);

         filehdr.biPlanes=getshort(input->file);
         filehdr.biBitCount=getshort(input->file);
         filehdr.biCompression=getlong(input->file);
         filehdr.biSizeImage=getlong(input->file);
         filehdr.biXPelsPerMeter=getlong(input->file);
         filehdr.biYPelsPerMeter=getlong(input->file);
         filehdr.biClrUsed=getlong(input->file);
         filehdr.biClrImportant=getlong(input->file);

     /*copy relevant info image structure*/
         input->x_size=  filehdr.biWidth;
         input->y_size= filehdr.biHeight;
         input->num_colors= filehdr.biClrUsed;
 //       cout<<"image height="<<input->x_size<<endl;
 //        cout<<"image weight="<<input->y_size<<endl;


    // #if DEBUG
          printf("bfSize=%lu\n",filehdr.bfSize);
          printf("biSize=%lu\n",filehdr.biSize);
          printf("bfOffBits=%lu\n",filehdr.bfOffBits);
          printf("biWidth=%lu\n",filehdr.biWidth);
          printf("biHeight=%lu\n",filehdr.biHeight);
          printf("biBitCount=%lu\n",filehdr.biBitCount);
          printf("biClrUsed=%lu\n",filehdr.biClrUsed);
      // #endif

      /*if bit per Pixel less than 24,must read in color map*/
      if(filehdr.biBitCount!=24)
       {
           bytes_in_clrmap=filehdr.biClrUsed*4;//bytes_in_clrmap=filehdr.biClrUsed*4;
           input->colormap=Alloc_ImageColor(filehdr.biClrUsed*3);

           /*read  in color map */
           input->colormap=color_map;
           //     printf("color_map=%lu\n",color_map);
         
           if(fgets((char*)color_map,bytes_in_clrmap,input->file)==NULL)
            {
                fprintf(stderr,"Error in while reading clor map of'%s'\n",input->fname);
                exit(1);
	    }


        /*
          *strip out padded zero bytes
        */
          color_map=input->colormap;
          bpp=1;
        }
        else
        {
         /*No color map in present*/
          bytes_in_clrmap=0;
          bpp=3;
         }

        /*finally read in read itself
	*
	*stripping out padded zero at end of rows and reoder pixel
	*data so that the data starts at the top left pixel
	*/
        input->pixel=Alloc_Image(input->x_size*input->y_size);

        /*set temp image pointer */
          image_data=input->pixel;


         /*position t start of image*/
          fseek(input->file,filehdr.bfOffBits,0L);


          /*calculate number of pad bytes */
          bytes_in_image=filehdr.bfSize-(14+filehdr.biSize+bytes_in_clrmap);
          bprow=bytes_in_image/input->y_size;  /*bytes per row */

          padb=bprow-((bprow/4)*4);   /*padb=bprow mod 4 */

          int   bytes_in_image_Actual=0;
          int padT=0;
          if(filehdr.biBitCount==24)
          {     /*This part is wrote for paintshop,it insert one colum zero in image */
                bytes_in_image_Actual=3*input->y_size*input->x_size;
                padT=(bytes_in_image-bytes_in_image_Actual)/input->y_size;
               // cout<<"padT="<<padT<<" padb="<<padb<<endl;
                if(padT!=0)
                        padb=padT+padb;

          }

          input->numOfStuff_inRow=padb;
          //cout<<"padb="<<padb<<endl;
           b=0;
	  /*let image pointer put end of image,*/
          /*   then read image  backwork*/

	image_data+=((input->x_size*input->y_size)-(input->x_size));


           /*while not eof and bytes not read yet */
           while((!feof(input->file))&&(3*b<bytes_in_image))
          {

//if (nrows++>576) {
//        cout<<"b = "<<b<<" bytes_in_image="<<bytes_in_image<<endl;
//        return;
//}
                for(i=0;i<input->x_size;i++)
                   {
                        /*read one cols width */
                    //  for(c=0;c<bpp;c++)
                       {
                           (image_data+i)->blue=fgetc(input->file);
                           (image_data+i)->green=fgetc(input->file);
                           (image_data+i)->red=fgetc(input->file);

                              b++;
//       if(((image_data+i)->red==0)&&((image_data+i)->green==0)&&((image_data+i)->blue==0))
                          //      count1++;
  //                      if((image_data+i)->red==(image_data+i)->green==(image_data+i)->blue)
                            //    count2++;
                     //   cout<<"g="<<(image_data+i)->green<<"\t";
                        }
                     }
                    image_data-=(input->x_size);  /*go up one row*/
                    if(padb>0)
                    {
                       for(i=0;i<padb;i++) /*if padded, read in paddata*/                      {
                       {
                        	mung=fgetc(input->file);
                        	b++;
                       }
                    }
         }
       //  fprintf(stdout,"...read %d bytes from image\n",b);
         input->bytes_per_pixel=bpp;
     }
       //     return (0);
}/*end of Read_BMP()*/







/*read num_bytes from a raw binary file*/
//int Read_Raw(IMAGE_STRUCT *input,int num_bytes)
//{
 //	unsigned char *rawdata;
//	rawdata=(unsigned char*)calloc(num_bytes,sizeof(unsigned
//char));	  	      rawdata=(unsigned char
	 // if(rawdata==(unsigned char*)NULL)
         //{
           // cerr<<"Err"<<endl;
            //exit(1);
         //}
//	input->image=rawdata;
//	if(fgets((char*)rawdata,num_bytes,input->file)==NULL)
  //      {
//	fprintf(stderr,"errorwhile reading raw data form %s\n",input->fname);
//	exit(1);
  //      }
//	return (0);

//}/*End of Read_Raw()*/

	/*Read 4 bytes from cuttent postion in file and convert */
	/*little endian to long int*/
	unsigned long imageBMP::getlong(FILE *fileptr)
	{
 		unsigned char chr[4];
      	 	unsigned long number;
		int i;
     	   	number=0;
        	for(i=0;i<4;i++)
    	   	 {
                    chr[i]=fgetc(fileptr);
                 }
        	number=chr[0];
		//cout<<"Number="<<number<<"\t"<<endl;
      	   	number+=chr[1]*256;
        	number+=chr[2]*256*256;
         	number+=chr[3]*256*256*256;
       	  	return number;
  }


        /*
	*Read 2 bytes from current position in file
         *and convert little endian to short int
         */
        unsigned short imageBMP::getshort(FILE *fileptr)
         {
            unsigned char chr[2];
            unsigned short int numb=0;

             for(int i=0;i<2;i++)
             {
                chr[i]=fgetc(fileptr);
	      }
              numb=(unsigned short int)chr[0];
               numb +=(unsigned short)chr[1]*256;
	   return (numb);
         }/*end of getshort*/


/*Write 4 bytess to current position in file*/
/*and convert from long int to little endian*/

int imageBMP::putlong(FILE *fileptr,unsigned long number)
{
  	unsigned char chr;
	 int i;
         unsigned long num,idiv,imod;
         num=number;
         for(i=0;i<4;i++)
          {
             idiv=num/256;
             imod=num-(idiv*256);
             num=idiv;
             chr=(unsigned char)imod;
            fputc(chr,fileptr);
           }
	return (0);
	}/*end of putlong()*/


/*write 2 bytes to current position in file
       * and convert from short int to little endian
       */
    int imageBMP::putshort(FILE *fileptr,unsigned short number)
	{
	    unsigned char chr;
              int i;
              unsigned short num,idiv,imod;
   		num=number;
    		for(i=0;i<2;i++)
   		  {
		     idiv=num/256;
      		    imod=num-(idiv*256);
                    num=idiv;
                    chr=(unsigned char)imod;
                    fputc(chr,fileptr);
 	}
        return (0);
}/*end of putshort()*/

/*allocate memory for image and color maps*/
PIXEL* imageBMP::Alloc_Image(int num_pixels)
{
 	PIXEL *ptr=NULL;

	ptr=(PIXEL*)calloc(num_pixels,sizeof(PIXEL));


       if(ptr==(PIXEL *)NULL)
       {
             cerr<<"Could not allocate memory for image space"<<endl;
             exit(1);
       }

return (ptr);

}/*End of Alloc_Image*/



unsigned char* imageBMP::Alloc_ImageColor(int num_bytes)
{
 	unsigned char *ptr=NULL;
	//if(ptr!=NULL)
	//cout<<"I am notfirst null"<<endl;
	//else
	//cout<<"I am first null"<<endl;
        ptr=(unsigned char*)calloc(num_bytes,sizeof(unsigned char));
  	//cout<<"sucess locate messoryptr="<<(ptr)<<"\t"<<"num_bytes"<<num_bytes<<endl;
	//if(ptr!=NULL)
	//cout<<"I am notnull *ptr="<<(int)ptr<<endl;
 	if(ptr==(unsigned  char *)NULL)
 	{
                cout<<"I am in null"<<endl;
		cerr<<"Could not allocate memory for image space\n";
	        exit(1);
	}

       // cout<<"ptr="<<ptr<<endl;
   return (ptr);

}/*End of Alloc_Image*/



















 int imageBMP::Write_BMP(IMAGE_STRUCT *output,char *fname)
 {
         output->file=fopen(fname,"wb+");
         if(output->file==NULL)
                printf("Can't open file %s \n",fname);
         if(output->file!=NULL) //if((output->openfile_Write(fname))!=(FILE *)NULL)
           this->Write_BMP(output);
 	//}

	fclose(output->file);
 return 0;

 }

 


 int imageBMP::Write_BMP(IMAGE_STRUCT *output)
 {
     	BMP_HEADER filehdr;
        unsigned char *Alloc_Image();
      //  unsigned char *color_map;  /*temp color map ptr*//* Don't delete it because it backup to use color loop_up table*/
        PIXEL *image_data;  /*temp image */

        int bytes_in_image;
   //     int bytes_in_clrmap; /* Don't delete it because it backup to use color loop_up table*/
        int x,y;
        int padb;     /*num pad bytes*/
        int bprow;    /*bytes per row*/
        int bpp;     /*bytes per pixel*/
        int req_bprow;  /*required bytes per row*/
        unsigned char mung=0;


        /*Calculate some necessory numbers */
        bpp=output->bytes_per_pixel;
        bprow=output->x_size*bpp;
        padb=(((output->x_size*bpp)/4+1)*4)-(output->x_size*bpp);
		

        /*if (X*bpp) was evenly divisible by 4 */
         /*then no pad bytes required */
         if(padb==4)
            padb=0;
         req_bprow=bprow+padb;
		// printf("in imageBMP, padb=%d,bpp=%d,req_bprow=%d,output->x_size=%d \n",padb,bpp,req_bprow,output->x_size);
         bytes_in_image=(req_bprow*output->y_size)+54;



         /*Set up all file header info */
          filehdr.bfSize=bytes_in_image;
          filehdr.bfReserved1=(unsigned short)0;
          filehdr.bfReserved2=(unsigned short)0;
          filehdr.bfOffBits=54L;  /*fileheader=14;imageHeader=40*/
          filehdr.biSize=40L;
          filehdr.biWidth=output->x_size;
          filehdr.biHeight=output->y_size;
          filehdr.biPlanes=(unsigned short)1;  /*short*/
          filehdr.biBitCount=(unsigned short)24;   /*short*/
          filehdr.biCompression=0;
          filehdr.biSizeImage=bprow*output->y_size;
          filehdr.biXPelsPerMeter=0;
          filehdr.biYPelsPerMeter=0;
          filehdr.biClrUsed=output->num_colors;
          filehdr.biClrImportant=output->num_colors;

     /*
      *     #if DEBUG
      *
      *       printf("bfSize=%u\n",filehdr.bfSize);
      *       printf("bfOffBits=%u\n",filehdr.bfOffBits);
      *       printf("bfWidth=%u\n",filehdr.biWidth);
      *       printf("bfHeight=%u\n",filehdr.biHeight);
      *       printf("biBitCount=%u\n",filehdr.biBitCount);
      *       printf("biSizeImage=%u\n",filehdr.biSizeImage);
      *       printf("biClrUsed=%u\n", filehdr.biClrUsed);
      *       printf("bytes_per_pixel=%u\n",bpp);
      *       printf("numOfStuff_inRow=%u\n",padb);
      *     #endif
      */
        int m=0;
        /*output header to file */
        fputc('B',output->file);
        fputc('M',output->file);
        m=putlong(output->file,filehdr.bfSize);
        putshort(output->file,filehdr.bfReserved1);
        putshort(output->file,filehdr.bfReserved2);
        putlong(output->file,filehdr.bfOffBits);
        putlong(output->file,filehdr.biSize);
	putlong(output->file,filehdr.biWidth);
        putlong(output->file,filehdr.biHeight);
        putshort(output->file,filehdr.biPlanes);
        putshort(output->file,filehdr.biBitCount);
        putlong(output->file,filehdr.biCompression);
        putlong(output->file,filehdr.biSizeImage);
        putlong(output->file,filehdr.biXPelsPerMeter);
        putlong(output->file,filehdr.biYPelsPerMeter);
        putlong(output->file,filehdr.biClrUsed);
        putlong(output->file,filehdr.biClrImportant);

       /*output color map, if there is one*/
	if(filehdr.biBitCount!=24)
	 {	}
      /*output image,padding rows to a 4 bytes boundary*/

      image_data=output->pixel;
      // cout<<"where output->image pointer="<<(int)image_data<<endl;
      // cout<<"what isbpp="<<bpp<<"\t"<<"padb="<<padb<<"\t"<<"mung="<<mung<<endl;

	int a;
    //	cout<<"output->y_size="<<output->y_size<<"\t"
      //    <<"output->x_size="<<output->x_size<<endl;
       for(y=output->y_size-1;y>=0;y--)/*write row y form bottom up*/
           {
	   	for(x=0;x<output->x_size;x++)
 		{
               // cout<<"*image_data="<<int(image_data) <<"\t"<<count++<<endl;
          	a=(y*output->x_size+x);
              	fputc((output->pixel+a)->blue,output->file);
	fputc((output->pixel+a)->green,output->file);
	fputc((output->pixel+a)->red,output->file);

      	 //fputc((image_data+a)->blue,output->file);
	//fputc((image_data+a)->green,output->file);
	//fputc((image_data+a)->red,output->file);
//	      cout<<"y="<<y<<" x="<<x<<"#"<<(image_data+a)->blue
  //                <<"#"<<(image_data+a)->green
    //              <<"#"<<(image_data+a)->red<<"\t";
 }

     /*padding 0 into end of each row  and let number of bits in row can divided by four*/
     /*it same as RGBQUAD struct*/
      if(padb>0)
         {
          for(int w=0;w<padb;w++)
     	     fputc(mung,output->file);
	}
 }
return (0);
}       /*End of write_BMP*/





void imageBMP::copydata_to_image(IMAGE_STRUCT *rgbPtr,
                        float **data,
                        long nrow)   //nrow =num of pixel; data each row store one pixel
  {
         for(long i=0;i<nrow;i++)
         {
                (rgbPtr->pixel+i)->red=(int)data[i][0];
                (rgbPtr->pixel+i)->green=(int)data[i][1];
                (rgbPtr->pixel+i)->blue=(int)data[i][2];

         }
}



imageBMP::~imageBMP()
{

       if(this->image_struct=NULL)
       {
             if((this->image_struct)->pixel)
             {
                 free(image_struct->pixel);
                 image_struct->pixel=NULL;
             }
             delete  (this->image_struct);
             this->image_struct=NULL;
       }
     
 }
