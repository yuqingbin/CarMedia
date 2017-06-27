//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Very simple tga loader.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Tga.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#ifndef _TGA_H_
#define _TGA_H_

int TGALoad(TCHAR *Filename, unsigned char **Buffer, int *Width, int *Height, int *Depth);
int GDI_TGALoad(const char *Filename, unsigned char **Buffer, int *Width, int *Height, int *Depth);
int TGAwrite (TCHAR *Filename, unsigned char *Buffer, int Width, int Height, int Depth);


#endif // _TGA_H_