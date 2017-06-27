//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation of the SimpleImageLoader class.  This class loads only handles tga files and does not use FreeImage.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/SimpleImageLoader.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "SimpleImageLoader.h"
#include <iostream>
#include <assert.h>
#include "Tga.h"

using namespace std;

//=================================================================================================================================
///
/// Constructor - Starts up the FreeImage lib.
///
/// \param void
///
/// \return void
//=================================================================================================================================
SimpleImageLoader::SimpleImageLoader()
{

}

//=================================================================================================================================
///
/// Destructor - Shuts down the FreeImage lib.
///
/// \param void
///
/// \return void
//=================================================================================================================================
SimpleImageLoader::~SimpleImageLoader()
{

}

//=================================================================================================================================
///
/// Loads an image from a file.
///
/// \param filename - The image filename
///
/// \return Pointer to a structure with all the FreeImage data in it
//=================================================================================================================================
ImageObject* SimpleImageLoader::LoadImageFile(TCHAR *filename )//( const char8 *filename )
{
   int ret;
   unsigned char *buf;
   int32 width, height, bpp, numChannels, bitsPerChannel;
  // int t1 = GetTickCount();
   ret = TGALoad( filename, &buf, &width, &height, &bpp);
  /* int t2 = GetTickCount();
   TCHAR str[100];
   _stprintf(str,L"%s%d",L"TGA",t2-t1);
   OutputDebugString(str);
   OutputDebugString(L"\n");*/

   if ( ! ret )
   {
      return NULL;
   }

   if ( bpp == 32 )
   {
      numChannels = 4;
      bitsPerChannel = 8;
   }
   else if ( bpp == 24 )
   {
      numChannels = 3;
      bitsPerChannel = 8;
   }
   else
   {
      assert(0);
      free(buf);
      return NULL;
   }

   ImageObject *pImg = new ImageObject( width, height, numChannels, bitsPerChannel );
   assert( pImg != NULL );

   for( uint32 y = 0; y < pImg->GetHeight(); y++ )
   {
	   //printf("\n");
      for( uint32 x = 0; x < pImg->GetWidth(); x++ )
      {
         if ( pImg->GetNumChannels() == 3 )
         {
            pImg->Pixel(x, y, 0) = buf[ width*y*3 + x*3 + 0 ];
            pImg->Pixel(x, y, 1) = buf[ width*y*3 + x*3 + 1 ];
            pImg->Pixel(x, y, 2) = buf[ width*y*3 + x*3 + 2 ];
			//printf("(R[%d][%d]=%d,G=%d,B[=%d);",x,y,pImg->Pixel(x, y, 0),pImg->Pixel(x, y, 1),pImg->Pixel(x, y, 2));
         }
         else if ( pImg->GetNumChannels() == 4 )
         {
            pImg->Pixel(x, y, 0) = buf[ width*y*4 + x*4 + 0 ];
            pImg->Pixel(x, y, 1) = buf[ width*y*4 + x*4 + 1 ];
            pImg->Pixel(x, y, 2) = buf[ width*y*4 + x*4 + 2 ];
            pImg->Pixel(x, y, 3) = buf[ width*y*4 + x*4 + 3 ];
         }
         else
         {
            assert( 0 );
         }
      }
   }

   free(buf);

   return pImg;
}

//=================================================================================================================================
///
/// Saves an image to a file.
///
/// \param filename - The image filename
/// \param imgObj - class that holds image data
///
/// \return pass or fail bool
//=================================================================================================================================
bool SimpleImageLoader::SaveImage( const char8 *filename, ImageObject *imgObj )
{
   assert(0);
   return false;
}


//void ColorConversion24To16 ( BYTE *lpBits24, int Width, int Height, BYTE *lpBits16   )
//{
//  int nPos16 = 0;
//  int Size = Width*Height*3;
//  for ( int nPos24 = 0 ; nPos24 < Size ; nPos24+=3 ,nPos16+=2)
//  {
//    BYTE Red24   = lpBits24[nPos24+2]; // 8-bit red
//    BYTE Green24 = lpBits24[nPos24+1]; // 8-bit green
//    BYTE Blue24  = lpBits24[nPos24+0]; // 8-bit blue
//
//    BYTE Red16   = Red24   >> 3;  // 5-bit red
//    BYTE Green16 = Green24 >> 2;  // 6-bit green
//    BYTE Blue16  = Blue24  >> 3;  // 5-bit blue
//
//    unsigned short RGB2Bytes = Blue16 + (Green16<<5) + (Red16<<(5+6));
//
//    lpBits16[nPos16]  = LOBYTE(RGB2Bytes);
//    lpBits16[nPos16+1]= HIBYTE(RGB2Bytes);
//  }
//}
//
//  // usage
//  ///*int Width  = 100;
//  //int Height = 100;
//
//  //BYTE *lpBits24 = new BYTE[Width*Height*3];
//  //BYTE *lpBits16 = new BYTE[Width*Height*2];
//
//
//  //ColorConversion24To16( lpBits24,Width,Height,lpBits16);
//
//
//  //delete [] lpBits24;
//  //delete [] lpBits16;Cheers */
//
//
//
//
//
//bool SimpleImageLoader::TGA2BMP565(char * tgaFname,)