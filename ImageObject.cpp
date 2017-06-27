//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation of the ImageObject class.  This class holds and manages all image data information.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/ImageObject.cpp#2 $ 
// 
// Last check-in:  $DateTime: 2008/03/21 10:01:35 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "ImageObject.h"
#include <algorithm>
#include "assert.h"

using namespace std;

//=================================================================================================================================
///
/// Constructor
///
/// \param width - The image width
/// \param height - The image height
/// \param numChannels - Number of channels (ie RGB=3, RGBA=4)
/// \param bitsPerChannel - The bits per channel
///
/// \return void
//=================================================================================================================================
ImageObject::ImageObject( uint32 width, uint32 height, uint32 numChannels, uint32 bitsPerChannel )
{
   uint32 totalSize;
   assert ( width > 0 );
   assert ( height > 0 );
   assert ( ( numChannels > 0 ) && ( numChannels <= 4 ) );
   assert ( bitsPerChannel == 8 );

   m_width = width;
   m_height = height;
   m_numChannels = numChannels;
   m_bitsPerChannel = bitsPerChannel;
   
   totalSize = m_width * m_height * m_numChannels * m_bitsPerChannel / sizeof(uint8);

   m_data = new uint8[totalSize];
   assert( m_data );

   m_compressedType = IMG_NONE;
   m_compressedSize = 0;
   m_compressedData = NULL;
}

//=================================================================================================================================
///
/// Destructor 
///
/// \param void
///
/// \return void
//=================================================================================================================================
ImageObject::~ImageObject()
{
	if(m_data) {delete [] m_data; m_data=NULL;}
	if(m_compressedData) {delete [] m_compressedData;  m_compressedData=NULL;}
}

//=================================================================================================================================
///
/// Access a single component of a single pixel
///
/// \param x - x position of the pixel
/// \param y - y position of the pixel
/// \param channel - channel fo the pixel (ie R, G, B, A)
///
/// \return a reference to the pixel
//=================================================================================================================================
uint8& ImageObject::Pixel( uint32 x, uint32 y, uint32 channel )
{
   uint32 pixelPitch = m_numChannels * m_bitsPerChannel / 8;
   uint32 linePitch = m_width * pixelPitch;
   
   x = min( x, m_width );
   y = min( y, m_height );
   channel = min( channel, m_numChannels );

   return m_data[y * linePitch + x * pixelPitch + channel]; 
}

//=================================================================================================================================
///
/// Sets up the type for compressed data
///
/// \param type - the type of compression we will be doing
///
/// \return pass or fail bool
//=================================================================================================================================
bool ImageObject::SetupCompressedData( CompressedType type )
{
   if ( m_compressedData )
   {
      delete [] m_compressedData;
   }

   switch ( type )
   {
   case IMG_NONE:
   case IMG_ATITC_RGB:
   case IMG_ATITC_RGBA:
   case IMG_ATI2N:
   case IMG_ATI1N:
   case IMG_ETC1:
   case IMG_ETC3:
   case IMG_ETC5:
      break;
   default:
      assert( 0 );
   }

   m_compressedData = NULL;
   m_compressedType = type;
   m_compressedSize = 0;
   return true;
}

//=================================================================================================================================
///
/// Sets up the size and data buffer for a compressed texture
///
/// \param size - The size of the compressed data
///
/// \return void
//=================================================================================================================================
void ImageObject::SetCompressedSize( uint32 size )
{
   if ( m_compressedData )
   {
      delete [] m_compressedData;
   }
   m_compressedData = new uint8[size];
   assert( m_compressedData );

   m_compressedSize = size;
}


//=================================================================================================================================
///
/// Changes the main data (not compressed data) to red
///
/// \param void
///
/// \return void
//=================================================================================================================================
void ImageObject::MakeImageRed()
{
   uint32 x, y, c;

   for ( y = 0; y < m_height; ++y )
   {
      for ( x = 0; x < m_height; ++x )
      {
         Pixel( x, y, 0 ) = 255;

         for ( c = 1; c < m_numChannels; ++c )
         {
            Pixel( x, y, c ) = 0;
         }
      }
   }
}

//=================================================================================================================================
///
/// Changes the main data from RGBA -> ARGB or RGB -> 1RGB
///
/// \param void
///
/// \return True if image has three or four channels, false otherwise
//=================================================================================================================================
bool ImageObject::MakeImageARGB()
{
   uint32 x, y;

   if ( m_numChannels != 3 && m_numChannels != 4 )
      return false;

   if ( m_numChannels == 4 )
   {
      for ( y = 0; y < m_height; ++y )
      {
         for ( x = 0; x < m_height; ++x )
         {
            uint8 r = Pixel( x, y, 0 );
            uint8 g = Pixel( x, y, 1 );
            uint8 b = Pixel( x, y, 2 );
            uint8 a = Pixel( x, y, 3 );
            
            Pixel( x, y, 0 ) = b;
            Pixel( x, y, 1 ) = g;
            Pixel( x, y, 2 ) = r;
            Pixel( x, y, 3 ) = a;

         }
      }
   }
   else
   {      
      uint32 nPixelSize = 4 * m_bitsPerChannel / 8;
      uint8 *pNewData = new uint8[nPixelSize * m_width * m_height];

      for ( y = 0; y < m_height; ++y )
      {
         for ( x = 0; x < m_height; ++x )
         {

            uint32 nOffset = y * m_width * nPixelSize + x * nPixelSize;
            
            uint8 r = Pixel( x, y, 0 );
            uint8 g = Pixel( x, y, 1 );
            uint8 b = Pixel( x, y, 2 );
            
            pNewData[nOffset + 0 ] = b;
            pNewData[nOffset + 1 ] = g;
            pNewData[nOffset + 2 ] = r;
            pNewData[nOffset + 3 ] = 255;            
         }
      }

      delete [] m_data;
      m_data = pNewData;
      m_numChannels = 4;
   }

   return true;
}


