//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// ESShader.c
//
//    Utility functions for loading shaders and creating program objects.
//

//
//  Includes
//
#include "eUtil.h"
#include "Misc.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
//#include <sstream>
using namespace std;
///=================================================================================================================================
///
/// Loads all the shader objects.
///
/// \param fileName is the name for the file where we get objects
/// \param stateHandle is the ES handle to different types of shaders
///
/// \return bool saying whether we passed or failed
//=================================================================================================================================
GLuint eLoadShader (  GLenum type,TCHAR* filename )  //;bool eLoadShaderObject(  GLenum type,const char* filename)
{
   char* source = NULL;
 GLuint shaderHandle;
  // GLint compiled;
  
   {
      // Use file io to load the code of the shader.
      std::ifstream fp( filename , ios_base::binary );
      if( fp.fail() ) 
      {
         cout << "Failed to open shader file: ";
         cout << filename << endl;
         return false;
      }

      fp.seekg( 0, std::ios_base::end );
      const long len = fp.tellg();
      fp.seekg( 0, std::ios_base::beg );
      // The +1 is optional, depending on how you call glShaderSourceARB
      source = new char[len+1];
      fp.read(source, sizeof(char)*len);
      source[len] = NULL;
	  fp.close();
   }


   const char* gls[1] = { source };
  //printf("gls=%s\n",source);
    
   // Create the shader object
   shaderHandle = glCreateShader ( type );

   if ( shaderHandle == 0 )
   	return 0;

   // Pass our sources to OpenGL. Our sources are NULL terminated, so pass NULL for the lenghts.
   glShaderSource( shaderHandle, 1, gls, NULL );
   // OpenGL made a copy. Do not need the source anymore.
   delete[] source;
   source = 0;

   // Compile that one object.
   glCompileShader(shaderHandle);

   // Check for compilation success
   GLint compilationResult = 0;
   glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &compilationResult );

   // current implementation always succeeds.
   // The error will happen at link time.
   if ( compilationResult == 0 )
   {
		  GLint infoLen = 0;

		  glGetShaderiv ( shaderHandle, GL_INFO_LOG_LENGTH, &infoLen );
	      
		  if ( infoLen > 1 )
		  {
				 char* infoLog = (char*)malloc (sizeof(char) * infoLen );

				 glGetShaderInfoLog ( shaderHandle, infoLen, NULL, infoLog );
				 eLogMessage ( "Error compiling shader:\n%s\n", infoLog );            
		         
				 free ( infoLog );
			  }

			  glDeleteShader ( shaderHandle );
			  return 0;
		 
		}
		return shaderHandle;

}


//
///
/// \brief Load a vertex and fragment shader, create a program object, link program.
//         Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure
//
GLuint eLoadProgram ( TCHAR *vertShaderSrcFName, TCHAR *fragShaderSrcFName )
{
   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   // Load the vertex/fragment shaders
   vertexShader = eLoadShader ( GL_VERTEX_SHADER, vertShaderSrcFName);
   if ( vertexShader == 0 )
      return 0;

   fragmentShader = eLoadShader ( GL_FRAGMENT_SHADER, fragShaderSrcFName );
   if ( fragmentShader == 0 )
   {
      glDeleteShader( vertexShader );
      return 0;
   }

   // Create the program object
   programObject = glCreateProgram ( );
   
   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );
glBindAttribLocation(programObject, 0, "a_position");
glBindAttribLocation(programObject, 1, "a_texCoord");
   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked ) 
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog =(char*) malloc (sizeof(char) * infoLen );

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         eLogMessage ( "Error linking program:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteProgram ( programObject );
      return 0;
   }
   GLint numActiveAttribs = 0;
    GLint isize; GLenum itype;
char **attrib_name=NULL;
	
	
      glGetProgramiv ( programObject, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs );
	  int t[10];
	  attrib_name=(char **)calloc(numActiveAttribs,sizeof(char *));
	  for(int i=0;i<numActiveAttribs; i++)
	  {
		  attrib_name[i]=new char[50];
	  glGetActiveAttrib(programObject,i,50,NULL,&isize,&itype,attrib_name[i]); 
       t[i]= glGetAttribLocation(programObject,attrib_name[i]); 
	   printf("attrib_name=%s,t[%d]=%d\n",attrib_name[i],i,t[i]);
	if(attrib_name)
		delete  attrib_name[i];
	  }
	free(attrib_name);

	  printf("eShader::numActiveAttribs=%d\n", numActiveAttribs);
   // Free up no longer needed shader resources
   glDeleteShader ( vertexShader );
   glDeleteShader ( fragmentShader );

   return programObject;
}
