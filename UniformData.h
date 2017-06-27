//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Definitions for the UniformData class.  This is a base class for the Uniform class.  This holds all the data.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/UniformData.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#ifndef _UNIFORMDATA_H_
#define _UNIFORMDATA_H_

#ifdef USE_STANDARD_GL
#include <glew.h>
#else
//#include <GLES/gl.h> 
#include <GLES2/gl2.h>
#include <GLES2/gl2amdext.h>
#endif //USE_STANDARD_GL

#include <string>
#include "ArgDefines.h"

//=================================================================================================================================
/// UniformData class - Contains the actual data for uniforms, textures, and attributes.
//=================================================================================================================================
class UniformData
{
public:
   /// \brief Constructor.
   UniformData();

   /// \brief Destructor.
   ~UniformData();

public:
   /// Locations in uniform/texture/attribute index (or location)
   int32          m_location;

   /// Text name used for this uniform or attribute
   char*          m_name;

   /// Type of data used to represent this uniform
   GLenum         m_type;

   /// Matrix data
   float32        m_matrixData[4][4];

   /// Vector float data
   float32        m_floatData[4];
   
   /// Vector int data
   int32          m_intData[4];

   /// GL texture id
   GLuint         m_textureHandle;

   /// GL texture unit number
   GLuint         m_textureUnit;

   /// Keeps track of whether this is depth texture or not
   bool           m_depthTex;

   /// Texture width
   int32          m_width;

   /// Texture height
   int32          m_height;

   /// GL fbo id
   GLuint         m_fboHandle;

   /// GL color render buffer handle
   GLuint         m_colorRbHandle;

   /// GL vertex shader id
   GLuint         m_vertShaderHandle;

   /// GL fragment shader id
   GLuint         m_fragShaderHandle;

   /// GL program id
   GLuint         m_progHandle;

   /// The program this uniform was attached to
   GLuint         m_attachedProgram;
};

#endif // _UNIFORMDATA_H_
