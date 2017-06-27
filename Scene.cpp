//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation for the Scene class.  This class wraps in drawing in GL.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Scene.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "Scene.h"
#include <assert.h>

using namespace std;

//=================================================================================================================================
///
/// Constructor
///
/// \param none
///
/// \return null
//=================================================================================================================================
Scene::Scene()
{
   float32 color[] = { 0.3f, 0.3f, 0.7f, 1 };
   m_clearColor[0] = color[0];
   m_clearColor[1] = color[1];
   m_clearColor[2] = color[2];
   m_clearColor[3] = color[3];
   width=0;
   height=0;
   shi = 0;
   bai = 0;
   dataPath=NULL;
   TV_FLAG=FALSE;
   BLUETOOTH_FLAG=FALSE;
   FM_FLAG=FALSE;
   USB_FLAG=FALSE;
   NAVIGATE_FLAG=FALSE;
//   MenuNum=1;
}

//=================================================================================================================================
///
/// Destructor
///
/// \param void
///
/// \return void
//=================================================================================================================================
Scene::~Scene()
{
   //freeResources();
  
	if(dataPath)
	 {
			free((void*)dataPath);
			dataPath=NULL;
	  }
	/*if ( vertices != NULL )
	{
		free ( vertices );
	}

	if ( indices != NULL )
	{
		free ( indices );
	}
	if ( normals != NULL )
	{
		free ( normals);
	}*/
	// Delete texture object	
	//glDeleteTextures ( 1, &textureId );
	// Delete program object
	glDeleteProgram ( s_progHandle);

	if( MapTexId)
	{
		free( MapTexId);  
		MapTexId=NULL;
	}
}


void BuildAttribList( EGLint *attribList )
{
   int    nAttribCount = 0;
   
   attribList[nAttribCount++] = EGL_RED_SIZE;
   attribList[nAttribCount++] = 5; 
   attribList[nAttribCount++] = EGL_GREEN_SIZE;
   attribList[nAttribCount++] = 6;
   attribList[nAttribCount++] = EGL_BLUE_SIZE;
   attribList[nAttribCount++] = 5;
   attribList[nAttribCount++] = EGL_ALPHA_SIZE;
   attribList[nAttribCount++] = 0;
   attribList[nAttribCount++] = EGL_DEPTH_SIZE;
   attribList[nAttribCount++] = 16;
   attribList[nAttribCount++] = EGL_STENCIL_SIZE;
   attribList[nAttribCount++] = 0;
   attribList[nAttribCount++] = EGL_SAMPLES;
   attribList[nAttribCount++] = 4;
   attribList[nAttribCount++] = EGL_NONE;
   
   assert( nAttribCount < MAX_EGL_ATTRIBUTES );
}

//=================================================================================================================================


//=================================================================================================================================
///
/// Puts the named program in use
///
/// \param name - The name we gave to the program
///
/// \return void
//=================================================================================================================================
//void Scene::useProgram( const char8* name )
//{
//   GLuint progHandle = s_uniforms.getProgramHandle( name );
//   glUseProgram( progHandle );
//}

//=================================================================================================================================
///
/// Updates the value of a uniform
///
/// \param name - The name we gave to the uniform
/// \param vals - An array of values we want to to replace the current uniform values with
///
/// \return true=pass ... false=fail
//=================================================================================================================================
/*bool Scene::updateUniform( const char8* name, float32* vals )
{
   return m_uniforms.updateOneUniform( name, vals );
}
*/
//=================================================================================================================================
///
/// Deletes all the GL resources we have allocated
///
/// \param void
///
/// \return void
//=================================================================================================================================
//void Scene::freeResources()
//{
//
//   s_uniforms.freeAllData();
//
//}

//=================================================================================================================================
///
/// Binds a texture into GL
///
/// \param name - The name we gave the texture
/// \param width - The width of the texture
/// \param height - The height of the texture
///
/// \return true = pass     false = fail
//=================================================================================================================================
//bool Scene::bindTexture( const char8* name, int32 width, int32 height )
//{
//   bool ret;
//   ret = s_uniforms.bindTexture( name, width, height );
//   return ret;
//}

//=================================================================================================================================
///
/// Binds an FBO into GL
///
/// \param name - The name we gave the fbo
/// \param width - The width of the fbo
/// \param height - The height of the fbo
///
/// \return true = pass     false = fail
//=================================================================================================================================
//bool Scene::bindFbo( const char8* name, int32 width, int32 height )
//{
//   bool ret;
//   ret = s_uniforms.bindFbo( name, width, height );
//   return ret;
//}


//=================================================================================================================================
///
/// Funtion to create the EGL window.
///
/// \param hWnd the parent window HWND.
/// \param hInstance the class instance.
///
/// \return 1 = pass; 0 = fail.
//=================================================================================================================================
int Scene::CreateEGLContext( HWND hWnd) //, HINSTANCE hInstance )
{
   EGLint attribList[MAX_EGL_ATTRIBUTES];
   EGLint numConfigs;
   EGLint majorVersion;
   EGLint minorVersion;
   EGLConfig config;

   printf("Before  BuildAttribList\n");
   /// Build up the attribute list
   BuildAttribList( attribList );
   printf("After  BuildAttribList\n");
   // Get Display
   g_egl.dsp = eglGetDisplay( EGL_DEFAULT_DISPLAY );
   if ( g_egl.dsp == EGL_NO_DISPLAY )
   {   
	  printf("eglGetDisplay failed\n");
      assert( 0 && "eglGetDisplay failed" );      
      return FALSE;
   }
    printf("After eglGetDisplay\n");
   // Initialize EGL
   if ( ! eglInitialize( g_egl.dsp, &majorVersion, &minorVersion) )
   {
		printf("eglInitialize failed\n" );
      assert( 0 && "eglInitialize failed" );
      return FALSE;
   }
     printf("After eglInitialize\n");
   // Get configs
  /* if ( ! eglGetConfigs( g_egl.dsp, NULL, 0, &numConfigs ) )
   {  
      assert( 0 && "eglGetConfigs failed" );      
      return FALSE;
   }
printf("After eglGetConfigs,numConfigs=%d\n",numConfigs);*/

   // Choose config
 {
    int i;
   	EGLConfig configs[10];
   	if(!eglChooseConfig(g_egl.dsp, attribList, &configs[0], 10,  &numConfigs))
  	{
  		printf("failed: eglChooseConfig\r\n");
    #ifdef UNDER_CE
		RETAILMSG(1, (TEXT("failed: eglChooseConfig...\n")));
     #endif
		return EGL_FALSE;
  	}
  	
  	if(numConfigs < 1)
  	{
  		printf("failed: No Matching configs\r\n");
		#ifdef UNDER_CE
			RETAILMSG(1, (TEXT("failed: No Matching configs...\n")));
		#endif
  		return EGL_FALSE;
  	}
  	
#ifdef UNDER_CE 	
  	for ( i=0; i < numConfigs; i++ ){
  		EGLint value;
  	
  		/* Use this to explicitly check that the EGL config has the expected color depths */
  		eglGetConfigAttrib( g_egl.dsp, configs[i], EGL_RED_SIZE, &value );
  		if ( 5 != value ){
  			continue;
  		}
  		eglGetConfigAttrib( g_egl.dsp, configs[i], EGL_GREEN_SIZE, &value );
  		if ( 6 != value ){
  			continue;
  		}
  		eglGetConfigAttrib( g_egl.dsp, configs[i], EGL_BLUE_SIZE, &value );
 		   if ( 5 != value ){
  			continue;
  		}	
#else
for ( i=0; i < numConfigs; i++ ){
  		EGLint value;
  	
  		/* Use this to explicitly check that the EGL config has the expected color depths */
  		eglGetConfigAttrib( g_egl.dsp, configs[i], EGL_RED_SIZE, &value );
  		if ( 8 != value ){
  			continue;
  		}
  		eglGetConfigAttrib( g_egl.dsp, configs[i], EGL_GREEN_SIZE, &value );
  		if ( 8 != value ){
  			continue;
  		}
  		eglGetConfigAttrib( g_egl.dsp, configs[i], EGL_BLUE_SIZE, &value );
 		   if ( 8 != value ){
  			continue;
  		}	
#endif
  		//g_egl.cfg= configs[i];	
		   config = configs[i];
  		break;
  	}
	
  }
 /*  if ( !eglChooseConfig(g_egl.dsp, attribList, &g_egl.cfg, 1, &numConfigs) )
   {
	  
      assert( 0 && "eglChooseConfig failed" );      
      return FALSE;
   }*/
   printf("After eglChooseConfig\n");
   // Create a surface
   //g_egl.surf = eglCreateWindowSurface( g_egl.dsp, g_egl.cfg, NativeWindowType( hWnd ), NULL );
   g_egl.surf = eglCreateWindowSurface( g_egl.dsp, config, NativeWindowType( hWnd ), NULL );
   if ( g_egl.surf == EGL_NO_SURFACE )
   {  
	
      assert( 0 && "eglCreateWindowSurface failed" );      
      return FALSE;
   }
   printf("After eglCreateWindowSurface\n");
   // Create a GL context
   EGLint ctxAttribList[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  // g_egl.cxt = eglCreateContext( g_egl.dsp, g_egl.cfg, EGL_NO_CONTEXT, ctxAttribList );
   g_egl.cxt = eglCreateContext( g_egl.dsp, config, EGL_NO_CONTEXT, ctxAttribList );
   if ( g_egl.cxt == EGL_NO_CONTEXT )
   {
     
	   assert( 0 && "eglCreateContext failed" );      
      return FALSE;
   }    

   // Make the context current
   if ( ! eglMakeCurrent( g_egl.dsp, g_egl.surf, g_egl.surf, g_egl.cxt ) )
   {  
      assert( 0 && "eglMakeCurrent failed" );      
      return FALSE;
   }
 
   return TRUE;

 //  EGLint attribList[MAX_EGL_ATTRIBUTES];
 //  EGLint numConfigs;
 //  EGLint majorVersion;
 //  EGLint minorVersion;
 //
 //
 //  /// Build up the attribute list
 //  BuildAttribList( attribList );
 //  printf("After  BuildAttribList\n");
 // 
 //// Get Display
 // // g_egl.dsp = eglGetDisplay( EGL_DEFAULT_DISPLAY );
 //  g_egl.dsp = eglGetDisplay(GetDC(hWnd));
 // 
 //  if ( g_egl.dsp == EGL_NO_DISPLAY )
 //  {   
	//  printf("eglGetDisplay failed\n");
 //     assert( 0 && "eglGetDisplay failed" );      
 //     return FALSE;
 //  }
 // printf("After eglGetDisplay\n");
 // 
 // // Initialize EGL
 //  if ( ! eglInitialize( g_egl.dsp, &majorVersion, &minorVersion) )
 //  {
	//	printf("eglInitialize failed\n" );
	//	assert( 0 && "eglInitialize failed" );
	//	return FALSE;
 //  }
 ////printf("After eglInitialize\n");
 // // //code from the book - need to match the bits coming back from 
 // //// the Mali driver
 // // // Get configs
 // // if ( ! eglGetConfigs( g_egl.dsp, NULL, 0, &numConfigs ) )
 // // {  printf("eglGetConfigs failed\n");
 // //    assert( 0 && "eglGetConfigs failed" );      
 // //    return FALSE;
 // // }

 // // // Choose config
 // // if ( !eglChooseConfig(g_egl.dsp, attribList, &g_egl.cfg, 1, &numConfigs) )
 // // {
	// //   printf("eglChooseConfig failed\n");
 // //    assert( 0 && "eglChooseConfig failed" );      
 // //    return FALSE;
 // // }
 // 	EGLConfig configs[10];
	//if ( !eglChooseConfig( g_egl.dsp, attribList,  &configs[0], 10, &numConfigs) )
 //  {
	//    printf("eglChooseConfig failed\n");
 //     assert( 0 && "eglChooseConfig failed" );      
 //     return FALSE;
 //  }
	//if(numConfigs < 1)
 // 	{
 // 		printf("failed: No Matching configs\r\n");
 // 		return FALSE;
 // 	}
 //g_egl.cfg = configs;
	////EGLint value;
	////for ( int i=0; i < numConfigs; i++ )
	////{
 //// 		/* Use this to explicitly check that the EGL config has the expected color depths */
 //// 		eglGetConfigAttrib(  g_egl.dsp, configs[i], EGL_RED_SIZE, &value );
 //// 		if ( 5 != value ){
 //// 			continue;
 //// 		}
 //// 		eglGetConfigAttrib(  g_egl.dsp, configs[i], EGL_GREEN_SIZE, &value );
 //// 		if ( 6 != value ){
 //// 			continue;
 //// 		}
 //// 		eglGetConfigAttrib( g_egl.dsp, configs[i], EGL_BLUE_SIZE, &value );
 ////		   if ( 5 != value ){
 //// 			continue;
 //// 		}	
 //// 		 g_egl.cfg = configs[i];
 //// 		break;
 //// 	}
	//printf("After eglChooseConfig,numConfigs=%d\n",numConfigs);
 // 


 //  // Create a surface
 //  g_egl.surf = eglCreateWindowSurface(  g_egl.dsp, g_egl.cfg, NativeWindowType( hWnd ), NULL );
 //  if ( g_egl.surf == EGL_NO_SURFACE )
 //  {  
	//   printf("eglCreateWindowSurface failed\n");
 //     assert( 0 && "eglCreateWindowSurface failed" );      
 //     return FALSE;
 //  }
 //  printf("After eglCreateWindowSurface\n");
 // 
 //  // Create a GL context
 //  EGLint ctxAttribList[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
 //  g_egl.cxt = eglCreateContext( g_egl.dsp, g_egl.cfg, EGL_NO_CONTEXT, ctxAttribList );
 //  if ( g_egl.cxt == EGL_NO_CONTEXT )
 //  {
 //     printf("eglCreateContext failed\n");
	//   assert( 0 && "eglCreateContext failed" );      
 //     return FALSE;
 //  }     
 //  printf("After  eglCreateContex\n");

 //  // Make the context current
 //  if ( ! eglMakeCurrent( g_egl.dsp, g_egl.surf, g_egl.surf, g_egl.cxt ) )
 //  {  printf("eglMakeCurrent failed\n");
 //     assert( 0 && "eglMakeCurrent failed" );      
 //     return FALSE;
 //  }
 // printf("After eglMakeCurrent\n");
 //
 //  return TRUE;
}

void Scene::eDestroyContext ( )
{
	if(g_egl.dsp)
	  {
		eglMakeCurrent(g_egl.dsp, NULL, NULL, NULL);
		if(g_egl.cxt) eglDestroyContext(g_egl.dsp, g_egl.cxt);
		if(g_egl.surf) eglDestroySurface(g_egl.dsp, g_egl.surf);
		eglTerminate(g_egl.dsp);
		//esInitContext(g_egl.cxt);
	  }
}