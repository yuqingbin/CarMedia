//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Definitions for the ImageObject class.  This class holds and manages all image data information.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/ImageObject.h#2 $ 
// 
// Last check-in:  $DateTime: 2008/03/21 10:01:35 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#ifndef _IMAGEOBJECT_H_
#define _IMAGEOBJECT_H_

#include "ArgDefines.h"

enum CompressedType
{
   IMG_NONE,
   IMG_ATITC_RGB,
   IMG_ATITC_RGBA,
   IMG_ATI2N,
   IMG_ATI1N,
   IMG_ETC1,
   IMG_ETC3,
   IMG_ETC5,
};

class ImageObject
{
public:
   /// \brief Constructor
   ImageObject( uint32 width, uint32 height, uint32 numChannels, uint32 bitsPerChannel );

   /// \brief Destructor
   ~ImageObject();

   /// \brief Access a single component of a single pixel
   uint8& Pixel( uint32 x, uint32 y, uint32 channel );

   /// \brief Sets up the type for compressed data
   bool SetupCompressedData( CompressedType type );

   /// \brief Sets up the size and data buffer for a compressed texture
   void SetCompressedSize( uint32 size );

   /// \brief Changes the main data (not compressed data) to red
   void MakeImageRed();

   /// \brief Changes the main data from RGBA -> ARGB
   bool MakeImageARGB();

   uint32 GetWidth()             { return m_width; }
   uint32 GetHeight()            { return m_height; }
   uint32 GetNumChannels()       { return m_numChannels; }
   uint32 GetBitsPerChannel()    { return m_bitsPerChannel; }
   uint32 GetBpp()               { return m_numChannels * m_bitsPerChannel; }
   uint8* GetDataPtr()           { return m_data; }
   uint32 GetCompressedType()    { return m_compressedType; }
   uint32 GetCompressedSize()    { return m_compressedSize; }
   uint8* GetCompressedDataPtr() { return m_compressedData; }

private:
   /// The width of the texture
   uint32 m_width;

   /// The height of the texture
   uint32 m_height;

   /// The number of channels for the texture
   uint32 m_numChannels;

   /// The bits per channel for the texture
   uint32 m_bitsPerChannel;

   /// The data (not compressed) for the texture
   uint8 *m_data;

   /// The compressed texture type
   CompressedType m_compressedType;

   /// The size of the data for the compressed texture
   uint32 m_compressedSize;

   /// The compressed data for the texture
   uint8  *m_compressedData;
};

#endif // _IMAGEOBJECT_H_
