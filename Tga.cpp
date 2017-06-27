//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Very simple tga loader.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Tga.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================
/*Further, the Image Type code should contain one of the following values: 

Code Description

---- -----------

0 No Image Present, No image data included.

1 Color-mapped, uncompressed

2 True-color RGB images, uncompressed

3 Black-&-White, uncompressed

9 Color-mapped, RLE compressed (Runlength encoded) 

10 True-color, RLE compressed

11 Black-&-White, RLE compressed
32  -  Compressed color-mapped data, using Huffman, Delta, and
          runlength encoding.
33  -  Compressed color-mapped data, using Huffman, Delta, and
          runlength encoding.  4-pass quadtree-type process.

*/
////////////////////////////////////////////////////////////////////////////////////
//
//  TGA.cpp
//
//      Loads a 24-bit TGA image 
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

///
//  Macros
//
#define INVERTED_BIT            (1 << 5)

///
//  Types
//
#pragma pack(push,x1)                            // Byte alignment (8-bit)
#pragma pack(1)

typedef struct
{
    unsigned char   IdSize, //ID Field Length, offset=0; size=1 byte
                    MapType,  //Color-map Type, offset=1; size=1 byte
                    ImageType;  //offset=1; size=1 byte
    unsigned short  PaletteStart, //First Color-map Entry, //offset=3; size=5 byte ; //Color-map-specific info
                    PaletteSize;   //Color-map Length,//offset=5,	//Color-map-specific info
    unsigned char   PaletteEntryDepth; //Color-map Entry Size, offset=7	 //Color-map-specific info
    unsigned short  X,               // Image X Origin; //Image-specific Info
                    Y,               // Image y Origin; //Image-specific Info
                    Width,         //Image Width
                    Height;        //Image Height

    unsigned char   ColorDepth,       // Bits-Per-Pixel
                    Descriptor;       //Image-Descriptor Bits

               
}   TGA_HEADER;



#pragma pack(pop,x1)

////////////////////////////////////////////////////////////////////////////////////
//
//  Private Functions
//

////////////////////////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//


///
//  TGALoad()
//
int TGALoad(TCHAR *Filename, unsigned char **Buffer, int *Width, int *Height, int *Depth)
{
    FILE        *fp;
    TGA_HEADER   Header;
    
    if ( _wfopen_s( &fp, Filename, L"rb" ) != 0 )
    {
       return FALSE;
    }

    if(fp== NULL)
    {
        return FALSE;
    }

    fread(&Header, sizeof(TGA_HEADER), 1, fp);
	
	*Width=  Header.Width;
	*Height= Header.Height;
	*Depth=  Header.ColorDepth;

	if(Header.ColorDepth== 24)
    {
        RGBTRIPLE *Buffer24;

        Buffer24= (RGBTRIPLE*)malloc(sizeof(RGBTRIPLE) * (*Width) * (*Height));

        if(Buffer24)
        {
            int i=0;
            int x,
                y;

            size_t t=fread(Buffer24, sizeof(RGBTRIPLE), (*Width) * (*Height), fp);
			//if(t!=(*Width) * (*Height))
				printf("t=%d, (*Width) * (*Height)=%d, *Width)=%d,(*Height)=%d\n",t,((*Width)*(*Height)),*Width,(*Height));
            *Buffer= (unsigned char*) malloc(3 * (*Width) * (*Height));
              if(!(Header.Descriptor & INVERTED_BIT))
					{  
					printf("bbp=24, Filename=%s, with Header.Descriptor=%d\n",Filename,Header.Descriptor);
					}
            for(y = 0; y < *Height; y++)
                for(x = 0; x < *Width; x++)
                {
                    int Index= y * (*Width) + x;

                    if(!(Header.Descriptor & INVERTED_BIT))
					{    Index= ((*Height) - 1 - y) * (*Width) + x;
					//printf("bbp=24, Filename=%s, with Header.Descriptor=1\n",Filename);
					}
                    (*Buffer)[(i * 3)]=      Buffer24[Index].rgbtRed;
                    (*Buffer)[(i * 3) + 1]=  Buffer24[Index].rgbtGreen;
                    (*Buffer)[(i * 3) + 2]=  Buffer24[Index].rgbtBlue;
                    
                    i++;
                }

            fclose(fp);


            free(Buffer24);
            return(TRUE);
        }		
    }
	else if(Header.ColorDepth == 32){
        RGBQUAD *Buffer32;

        Buffer32= (RGBQUAD*)malloc(sizeof(RGBQUAD) * (*Width) * (*Height));

        if(Buffer32)
        {
            int i=0;
            int x,
                y;

            fread(Buffer32, sizeof(RGBQUAD), (*Width) * (*Height), fp);

            *Buffer= (unsigned char *) malloc(4 * (*Width) * (*Height));
              if(!(Header.Descriptor & INVERTED_BIT))
					{   
					printf("bbp=32, Filename=%s, with Header.Descriptor=%d\n",Filename,Header.Descriptor);
					}
            for(y = 0; y < *Height; y++)
                for(x = 0; x < *Width; x++)
                {
                    int Index= y * (*Width) + x;

                    if(!(Header.Descriptor & INVERTED_BIT))
					{     Index= ((*Height) - 1 - y) * (*Width) + x;
						//printf("bbp=32, Filename=%s, with Header.Descriptor=1\n",Filename);
					}
                    (*Buffer)[(i * 4)]=      Buffer32[Index].rgbRed;
                    (*Buffer)[(i * 4) + 1]=  Buffer32[Index].rgbGreen;
                    (*Buffer)[(i * 4) + 2]=  Buffer32[Index].rgbBlue;
                    (*Buffer)[(i * 4) + 3]=  Buffer32[Index].rgbReserved;
                   
                    i++;
                }

            fclose(fp);


            free(Buffer32);
            return(TRUE);
        }		
    }
    return(FALSE);
}


int GDI_TGALoad(const char *Filename, unsigned char **Buffer, int *Width, int *Height, int *Depth)
{
    FILE        *fp;
    TGA_HEADER   Header;
    
    if ( fopen_s( &fp, Filename, "rb" ) != 0 )
    {
       return FALSE;
    }

    if(fp== NULL)
    {
        return FALSE;
    }

    fread(&Header, sizeof(TGA_HEADER), 1, fp);
	
	*Width=  Header.Width;
	*Height= Header.Height;
	*Depth=  Header.ColorDepth;
	int m=(*Width) * (*Depth);
	int iRowDataLenOld=m/8;
	int iRowDataLenNew= ((m + 31) >> 5) << 2;
	printf("sizeof(Header)=%d,iRowDataLenOld=%d,iRowDataLenNew=%d,nrow=%d,ncol=%d\n",sizeof(Header),iRowDataLenOld,iRowDataLenNew,*Width,*Height);
	int padpixel,padb;     /*num pad bytes*/
	int req_row;  /*required row width*/
	padb=(iRowDataLenNew-iRowDataLenOld);
	//if(padb==4)
	//			padb=0;
	padpixel=padb*8/(*Depth);

	req_row=(*Width)+padb;
	printf("Filename=%s,padb=%d,*Width=%d,req_row=%d,padpixel=%d\n",Filename,padb,(*Width),req_row,padpixel);
	if(Header.ColorDepth== 24)
    {
        RGBTRIPLE *Buffer24;

        Buffer24= (RGBTRIPLE*)malloc(sizeof(RGBTRIPLE) * (*Width) * (*Height));

        if(Buffer24)
        {
            int i=0;
            int x,
                y;

            fread(Buffer24, sizeof(RGBTRIPLE), (*Width) * (*Height), fp);
			

            *Buffer= (unsigned char*) malloc(3 * (req_row) * (*Height));
            if(!(Header.Descriptor & INVERTED_BIT))
			{  
					printf("bbp=24, Filename=%s, with Header.Descriptor=%d\n",Filename,Header.Descriptor);
			}
            for(y = 0; y < *Height; y++)
			{    for(x = 0; x < *Width; x++)
                {
                    int Index= y * (*Width) + x;

                    if(!(Header.Descriptor & INVERTED_BIT))
					{    Index= ((*Height) - 1 - y) * (*Width) + x;
					//printf("bbp=24, Filename=%s, with Header.Descriptor=1\n",Filename);
					}
                    (*Buffer)[(i * 3)]=      Buffer24[Index].rgbtRed;
                    (*Buffer)[(i * 3) + 1]=  Buffer24[Index].rgbtGreen;
                    (*Buffer)[(i * 3) + 2]=  Buffer24[Index].rgbtBlue;
					i++;
				}
				if(padb>0)
				{
						  for(int w=0;w<padpixel;w++)
						  {	 (*Buffer)[(i * 3)]=0;
							(*Buffer)[(i * 3) + 1]=0;
							(*Buffer)[(i * 3) + 2]=0;
							i++;	
						  }
				}
          
                    
              }

            fclose(fp);


            free(Buffer24);
            return(TRUE);
        }		
    }
	else if(Header.ColorDepth == 32){
        RGBQUAD *Buffer32;

        Buffer32= (RGBQUAD*)malloc(sizeof(RGBQUAD) * (*Width) * (*Height));

        if(Buffer32)
        {
            int i=0;
            int x,
                y;

          fread(Buffer32, sizeof(RGBQUAD), (*Width) * (*Height), fp);
			

          *Buffer= (unsigned char*) malloc(4 * (req_row) * (*Height));
          //  *Buffer= (unsigned char *) malloc(4 * (*Width) * (*Height));
           if(!(Header.Descriptor & INVERTED_BIT))
			{   
					printf("bbp=32, Filename=%s, with Header.Descriptor=%d\n",Filename,Header.Descriptor);
			}
            for(y = 0; y < *Height; y++)
			{    for(x = 0; x < *Width; x++)
                {
                    int Index= y * (*Width) + x;

                    if(!(Header.Descriptor & INVERTED_BIT))
					{     Index= ((*Height) - 1 - y) * (*Width) + x;
						//printf("bbp=32, Filename=%s, with Header.Descriptor=1\n",Filename);
					}
                    (*Buffer)[(i * 4)]=      Buffer32[Index].rgbRed;
                    (*Buffer)[(i * 4) + 1]=  Buffer32[Index].rgbGreen;
                    (*Buffer)[(i * 4) + 2]=  Buffer32[Index].rgbBlue;
                    (*Buffer)[(i * 4) + 3]=  Buffer32[Index].rgbReserved;
                   
                    i++;
					}
					if(padb>0)
					{
						  for(int w=0;w<padpixel;w++)
						  {	 
							(*Buffer)[(i * 4)]=0;
							(*Buffer)[(i * 4) + 1]=0;
							(*Buffer)[(i * 4) + 2]=0;
							(*Buffer)[(i * 4) + 3]=0; 
							i++;	
						  }
					}
                }

            fclose(fp);
			*Width=req_row;

            free(Buffer32);
            return(TRUE);
        }		
    }
    return(FALSE);
}


  //TGALoad()

int TGAwrite (TCHAR *Filename, unsigned char *Buffer, int Width, int Height, int Depth)
{
    FILE        *fp;
    TGA_HEADER   *Header=(TGA_HEADER*)calloc(1,sizeof(TGA_HEADER));
    
    if ( _wfopen_s( &fp, Filename, L"wb+" ) != 0 )
    {
       return FALSE;
    }

    if(fp== NULL)
    {
        return FALSE;
    }
	 Header->IdSize=0; 
	 Header->MapType=0;
	 Header->ImageType=2;
      
     Header->PaletteStart=0; 
     Header->PaletteSize=0;   
     Header->PaletteEntryDepth=0; 
     Header->X=0; 
	 Header->Y=0;
   
	
	Header->Width=Width;
	Header->Height=Height;
	Header->ColorDepth=Depth;
	Header->Descriptor=0;
	//if(Depth==32)
	//			Header->Descriptor=28;

    if(fwrite(&Header, sizeof(TGA_HEADER), 1, fp)!=1)
	{	
		printf("Write file header fail!!!\n");
		return false;
	
	}
	if(Header->ColorDepth== 24)
    {
        RGBTRIPLE *Buffer24;

        Buffer24= (RGBTRIPLE*)malloc(sizeof(RGBTRIPLE) * (Width) * (Height));

        if(Buffer24)
        {
            int i=0;
            int x,
                y;
   
			
            Buffer= (unsigned char*) malloc(3 * (Width) * (Height));
				  /*if(!(Header.Descriptor & INVERTED_BIT))
						{  
						printf("bbp=24, Filename=%s, with Header.Descriptor=%d\n",Filename,Header.Descriptor);
						}*/
			unsigned char temp;
            for(y = 0; y < Height; y++)
                for(x = 0; x < Width; x++)
                {
                    int Index= y * (Width) + x;

                    if(!(Header->Descriptor & INVERTED_BIT))
					    Index= (Height - 1 - y) * (Width) + x;
				
					    temp=(Buffer)[(Index * 3)];
                   
                   (Buffer)[(Index * 3) + 2]=temp;

                    /* Buffer24[Index].rgbtRed=(Buffer)[(i * 3)];
                    Buffer24[Index].rgbtGreen=(Buffer)[(i * 3) + 1];
                    Buffer24[Index].rgbtBlue=(Buffer)[(i * 3) + 2];
                    
                    i++;*/
                }
			// size_t t=fwrite(Buffer24, sizeof(RGBTRIPLE), (Width) * (Height), fp);
			size_t t=fwrite(Buffer, sizeof(unsigned char), (Width) * (Height)*3, fp);
            fclose(fp);


            free(Buffer24);
            return(TRUE);
        }		
    }
	else if(Header->ColorDepth == 32)
	{
        RGBQUAD *Buffer32;

        Buffer32= (RGBQUAD*)malloc(sizeof(RGBQUAD) * (Width) * (Height));

        if(Buffer32)
        {
            int i=0;
            int x,
                y;
 

            Buffer= (unsigned char *) malloc(4 * (Width) * (Height));
        
            for(y = 0; y < Height; y++)
                for(x = 0; x < Width; x++)
                {
                    int Index= y * (Width) + x;

                    if(!(Header->Descriptor & INVERTED_BIT))
					{     Index= ((Height) - 1 - y) * (Width) + x;
						//printf("bbp=32, Filename=%s, with Header.Descriptor=1\n",Filename);
					}
                     Buffer32[Index].rgbRed=(Buffer)[(i * 4)];
                     Buffer32[Index].rgbGreen=(Buffer)[(i * 4) + 1];
                     Buffer32[Index].rgbBlue=(Buffer)[(i * 4) + 2];
                    Buffer32[Index].rgbReserved=(Buffer)[(i * 4) + 3];
                   
                    i++;
                }
			 fwrite(Buffer32, sizeof(RGBQUAD), (Width) * (Height), fp);
            fclose(fp);


            free(Buffer32);
            return(TRUE);
        }		
    }
	if(Header)
			free(Header);
    return(FALSE);
}