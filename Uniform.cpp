//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation for the Uniform class.  This class loads textures, attributes, and uniforms.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Uniform.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================
#pragma warning (disable:4996)
#include "uniform.h"

#include "SimpleImageLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>
#include "comm1.h"
#include <Misc.h>
#include "Misc.h"
using namespace std;




//=================================================================================================================================
///
/// Loads all the shader objects.
///
/// \param fileName is the name for the file where we get objects
/// \param stateHandle is the ES handle to different types of shaders
///
/// \return bool saying whether we passed or failed
//=================================================================================================================================
bool Uniform::loadShaderObject( const char8* filename, GLuint shaderHandle )
{
   char8* source = NULL;

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

   const char8* gls[1] = { source };
  //printf("gls=%s\n",source);
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
      cout << "Failed to compile shader:";
      cout << filename << endl;
      return false;
   }
  
   return true;
}



//=================================================================================================================================
///
/// Parses in text file with info about the textures/uniforms/attributes
///
/// \param fileName is the filename we are to parse
///
/// \return pass or fail boolean
//=================================================================================================================================
bool Uniform::parseUniforms(const char8* fileName,const char8* dataPath)
{
   int currentLine = 0;
   int nrow,ncol; //number of row and col in uniform.txt
  // m_uniforms.clear();
   m_unitID = 1;
   int i;
   commf_get_data_info(fileName,&nrow,&ncol);
   printf("nrow=%d,ncol=%d\n",nrow,ncol);
   m_uniforms=(UniformDataList*)calloc(1,sizeof(UniformDataList));
   m_uniforms->UniformDList=(UniformData**)calloc(nrow,sizeof(UniformData*));
   m_uniforms->size=0;
   m_uniforms->capacity=nrow;
   for(i=0;i<nrow;i++)
   {
		  m_uniforms->UniformDList[i]=new UniformData();  //(UniformData*)calloc(1,sizeof(UniformData));
   }
   ifstream fin( fileName, std::ios_base::binary );

   if ( fin.fail() )
   {
      cout << "Failed to open uniform file: ";
      cout << fileName << endl;
      return false;
   }

   while ( ! fin.eof() )
   {
      //UniformData *newUniform = new UniformData;
      
	   UniformData *newUniform ;

	   string lineString;
      string sItem;
      bool addToVec = false;
      newUniform=m_uniforms->UniformDList[m_uniforms->size];
      memset( newUniform, 0, sizeof(UniformData) );

      getline( fin, lineString );
      stringstream line( lineString );

      ++currentLine;

      line >> sItem;   
	 
printf("INPUT===%s, m_uniforms->size=%d\n",sItem.c_str(),m_uniforms->size);
      if ( sItem == "uniform_float" )
      {
         string sKey;
         float32 fVal;
         
         line >> sKey >> fVal;

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_FLOAT;
         newUniform->m_floatData[0] = fVal;
         newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );
         glUniform1f( newUniform->m_location, newUniform->m_floatData[0] );
      }
      else if (sItem == "uniform_vec2")
      {
         string sKey;
         float32 vVal[2];
         line >> sKey >> vVal[0] >> vVal[1];

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_FLOAT_VEC2;
         newUniform->m_floatData[0] = vVal[0];
         newUniform->m_floatData[1] = vVal[1];
         newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );
         glUniform2f( newUniform->m_location, newUniform->m_floatData[0], newUniform->m_floatData[1] );
      }
      else if (sItem == "uniform_vec3")
      {
         string sKey;
         float32 vVal[3];
         line >> sKey >> vVal[0] >> vVal[1] >> vVal[2];

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_FLOAT_VEC3;
         newUniform->m_floatData[0] = vVal[0];
         newUniform->m_floatData[1] = vVal[1];
         newUniform->m_floatData[2] = vVal[2];
         newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );
         glUniform3f( newUniform->m_location, newUniform->m_floatData[0], newUniform->m_floatData[1], newUniform->m_floatData[2] );
      }
      else if ( sItem == "uniform_vec4" )
      {
         string sKey;
         float32 vVal[4];
         line >> sKey >> vVal[0] >> vVal[1] >> vVal[2] >> vVal[3]; 

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_FLOAT_VEC4;
         newUniform->m_floatData[0] = vVal[0];
         newUniform->m_floatData[1] = vVal[1];
         newUniform->m_floatData[2] = vVal[2];
         newUniform->m_floatData[3] = vVal[3];
         newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );
         glUniform4f( newUniform->m_location, newUniform->m_floatData[0], newUniform->m_floatData[1],
                      newUniform->m_floatData[2], newUniform->m_floatData[3] );
      }
      else if ( sItem == "uniform_mat4" )
      {
		   printf("In parseUniforms ====uniform_mat4 beginning\n");
         string sKey;
         line >> sKey;

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         for (int i=0; i<4; ++i)
         {
            float32 vVal[4];
            line >> vVal[0] >> vVal[1] >> vVal[2] >> vVal[3]; 
            newUniform->m_matrixData[i][0] = vVal[0];
            newUniform->m_matrixData[i][1] = vVal[1];
            newUniform->m_matrixData[i][2] = vVal[2];
            newUniform->m_matrixData[i][3] = vVal[3];
         }
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_FLOAT_MAT4;
         newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );
         glUniformMatrix4fv( newUniform->m_location, 1, GL_FALSE, &newUniform->m_matrixData[0][0] );
      }
      else if (sItem == "attribute")
      {
         string sKey;
         int32 vVal;

         line >> sKey >> vVal;
		 printf("In Attribute sKey=%s,vVal=%d\n ",sKey.c_str(),vVal);
         glBindAttribLocation( m_recentProgramHandle, vVal, sKey.c_str() );		
         addToVec = false;
      }
      else if (sItem == "texture")
      {
		   printf("In parseUniforms ====texture beginning\n");
         SimpleImageLoader imgLoader;

         ImageObject *pImgObj;
         uint32 texId;

         string sKey, sVal;
         int32 texUnit;
         line >> sKey >> texUnit >> sVal;

		 printf("sKey=%s, texUnit=%d, sVal=%s\n",sKey.c_str(),texUnit,sVal.c_str());
         
		
		 char *texturefname=(char *)calloc(MAX_PATH,sizeof(char));

		 strcpy_s(texturefname,MAX_PATH,dataPath);
		 strcat_s(texturefname,MAX_PATH,sVal.c_str());
		
		 printf("In TEXTURE :: texturefname =%s\n",texturefname );
         pImgObj = imgLoader.LoadImageFile( texturefname );
		 if(texturefname )
		 {
			 free(texturefname);  texturefname=NULL; 
		 }

         if ( pImgObj != NULL )
         {
            glGenTextures( 1, &texId );
            glActiveTexture( GL_TEXTURE0 + texUnit );
            glBindTexture( GL_TEXTURE_2D, texId );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            printf("In TEXTURE pImgObj->GetNumChannels()=%d\n",pImgObj->GetNumChannels() );
            if ( pImgObj->GetNumChannels() == 3 )
            {
             
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, pImgObj->GetWidth(), pImgObj->GetHeight(),
                             0, GL_RGB, GL_UNSIGNED_BYTE, pImgObj->GetDataPtr() );
            }
            else
            {
               assert( pImgObj->GetNumChannels() == 4 );
               glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pImgObj->GetWidth(), pImgObj->GetHeight(),
                             0, GL_RGBA, GL_UNSIGNED_BYTE, pImgObj->GetDataPtr() );
            }
            delete pImgObj;
          
            addToVec = true;
            newUniform->m_attachedProgram = m_recentProgramHandle;
            newUniform->m_name = new char [sKey.length()+1];
            strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
            newUniform->m_type = SAMPLE_INT;
            newUniform->m_textureHandle = texId;
            newUniform->m_textureUnit = texUnit;
            newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str());			
            glUniform1i( newUniform->m_location, newUniform->m_textureUnit );
			 printf("In Uniform sKey.c_str()=%s,texUnit=%d\n",sKey.c_str(),newUniform->m_textureUnit);
         }
         else
         {
            // Failed to load texture;
            newUniform->m_location = NULL;
            assert( 0 );
         }
		  printf("In parseUniforms ====texture end \n");
      }
      else if ( sItem == "compressed_texture" )
      {
         string sCompressionType, sKey, sVal;
         int32 texUnit;
         line >> sCompressionType >> sKey >> texUnit >> sVal;

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_INT;
         newUniform->m_textureUnit = texUnit;

#ifndef USE_STANDARD_GL
         newUniform->m_textureHandle = LoadCompressedTexture( sVal.c_str(), sCompressionType.c_str(), texUnit );
#endif // USE_STANDARD_GL
         newUniform->m_location = glGetUniformLocation(m_recentProgramHandle,sKey.c_str());			
         glUniform1i( newUniform->m_location, newUniform->m_textureUnit );
		
      }
      else if (sItem == "genTexture")
      {
         GLuint texId;
         int32 texUnit;
         string sKey;
         line >> sKey >> texUnit;

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_INT;
         newUniform->m_textureUnit = texUnit;
         glGenTextures( 1, &texId );
         newUniform->m_textureHandle = texId;
         newUniform->m_location = glGetUniformLocation(m_recentProgramHandle,sKey.c_str());			
         glUniform1i( newUniform->m_location, newUniform->m_textureUnit );
      }
      else if (sItem == "nullTexture")
      {
         GLuint texId;
         int32 texUnit;
         string sKey;
         line >> sKey >> texUnit;

         addToVec = true;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_INT;
         newUniform->m_textureUnit = texUnit;
         glGenTextures( 1, &texId );
         newUniform->m_textureHandle = texId;

         glActiveTexture( GL_TEXTURE0 + texUnit );
         glBindTexture( GL_TEXTURE_2D, texId );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      }
      else if (sItem == "colorTexFbo")
      {
         GLuint texId, fboId;
         int32 texUnit;
         string sKey;
         line >> sKey >> texUnit;

         addToVec = true;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_INT;
         newUniform->m_textureUnit = texUnit;
         glGenTextures( 1, &texId );
#ifdef USE_STANDARD_GL
         glGenFramebuffersEXT( 1, &fboId );
#else
         glGenFramebuffers( 1, &fboId );
#endif // USE_STANDARD_GL
         newUniform->m_textureHandle = texId;
         newUniform->m_fboHandle = fboId;

         glActiveTexture( GL_TEXTURE0 + texUnit );
         glBindTexture( GL_TEXTURE_2D, texId );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      }
      else if ( sItem == "depthTexFbo" )
      {
         GLuint texId, fboId;
         int32 texUnit;
         string sKey;
         line >> sKey >> texUnit;

         addToVec = true;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_INT;
         newUniform->m_textureUnit = texUnit;
         newUniform->m_depthTex = true;
         glGenTextures( 1, &texId );
#ifdef USE_STANDARD_GL
         glGenFramebuffersEXT( 1, &fboId );
#else
         glGenFramebuffers( 1, &fboId );
#endif // USE_STANDARD_GL
         newUniform->m_textureHandle = texId;
         newUniform->m_fboHandle = fboId;

         glActiveTexture( GL_TEXTURE0 + texUnit );
         glBindTexture( GL_TEXTURE_2D, texId );

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }
      else if ( sItem == "program" )
      {
		  printf("In parseUniforms ====program beginning\n");
         string sKey, vertShader, fragShader;
         line >> sKey >> vertShader >> fragShader;

		 printf("sKey=%s,vertShader=%s,fragShader=%s\n",sKey.c_str(),vertShader.c_str(), fragShader.c_str());
         addToVec = true;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_PROGRAM;
		  printf("newUniform->m_name=%s,  newUniform->m_type=%d\n",newUniform->m_name, newUniform->m_type);
         // Create the one program we are going to use.
         newUniform->m_progHandle = glCreateProgram();
         m_recentProgramHandle = newUniform->m_progHandle;

         // Create one shader of object of each type.
         newUniform->m_vertShaderHandle = glCreateShader( GL_VERTEX_SHADER );
         newUniform->m_fragShaderHandle = glCreateShader( GL_FRAGMENT_SHADER );
         char *verShaderFname=(char *)calloc(MAX_PATH,sizeof(char));
		 char *fragShaderFname=(char *)calloc(MAX_PATH,sizeof(char));
		
		 strcpy_s(verShaderFname,MAX_PATH,dataPath);
		 strcat_s(verShaderFname,MAX_PATH,vertShader.c_str());
		 strcpy_s(fragShaderFname,MAX_PATH,dataPath);
         strcat_s(fragShaderFname,MAX_PATH,fragShader.c_str());     

		 printf("verShaderFname=%s,fragShaderFname=%s\n",verShaderFname,fragShaderFname);
         loadShaderObject( verShaderFname, newUniform->m_vertShaderHandle );
         loadShaderObject(fragShaderFname, newUniform->m_fragShaderHandle );
		 //
		 if(verShaderFname) {free(verShaderFname); verShaderFname=NULL;}
		 if(fragShaderFname) {free(fragShaderFname); fragShaderFname=NULL;}

         GLint compileVertResult = 0;
         GLint compileFragResult = 0;
         glGetShaderiv( newUniform->m_vertShaderHandle, GL_COMPILE_STATUS, &compileVertResult );
         glGetShaderiv( newUniform->m_fragShaderHandle, GL_COMPILE_STATUS, &compileFragResult );
         if ( compileVertResult == 0 || compileFragResult == 0 )
         {
            char8 *log = (char8*)malloc(1000);
            GLint length2;
            if ( compileVertResult == 0 )
            {
               cout << "Failed to compile vert shader." << endl;
               glGetShaderInfoLog( newUniform->m_vertShaderHandle, 1000, &length2, log );
               cout << log << endl;
            }
            if ( compileFragResult == 0 )
            {
               cout << "Failed to compile frag shader." << endl;
               glGetShaderInfoLog( newUniform->m_fragShaderHandle, 1000, &length2, log );
               cout << log << endl;
            }
            free(log);
         }

         // Attach them to the program.
         glAttachShader( newUniform->m_progHandle, newUniform->m_vertShaderHandle );
         glAttachShader( newUniform->m_progHandle, newUniform->m_fragShaderHandle );
		  printf("In parseUniforms ====program end\n");
      }
      else if (sItem == "binaryProgram" )
      {
         string sKey, filename;
         line >> sKey >> filename;

         addToVec = true;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_PROGRAM;

         char8* source = NULL;
         long len;
         {
            // Use file io to load the code of the shader.
            std::ifstream fp( filename.c_str() , ios_base::binary );
            if( fp.fail() ) 
            {
               cout << "Failed to open shader file: ";
               cout << filename << endl;
               return false;
            }

            fp.seekg( 0, std::ios_base::end );
            len = fp.tellg();
            fp.seekg( 0, std::ios_base::beg );

            source = new char[len];
            fp.read(source, sizeof(char)*len);
         }

         // Create the one program we are going to use.
         newUniform->m_progHandle = glCreateProgram();
         m_recentProgramHandle = newUniform->m_progHandle;

         // Create one shader of object of each type.
         newUniform->m_vertShaderHandle = glCreateShader( GL_VERTEX_SHADER );
         newUniform->m_fragShaderHandle = glCreateShader( GL_FRAGMENT_SHADER );

#ifndef USE_STANDARD_GL
         GLuint shaders[2] = { newUniform->m_vertShaderHandle, newUniform->m_fragShaderHandle };
         glShaderBinary( 2, shaders, GL_Z400_BINARY_AMD, source, len );         
#endif // USE_STANDARD_GL

         // Attach them to the program.
         glAttachShader( newUniform->m_progHandle, newUniform->m_vertShaderHandle );
         glAttachShader( newUniform->m_progHandle, newUniform->m_fragShaderHandle );

         delete[] source;
      }
      else if ( sItem == "link" )
      {
		   printf("In parseUniforms ====link beginning\n");
         string progName;
         line >> progName;

         addToVec = false;
         GLuint progHandle = getProgramHandle( progName.c_str() );
		 printf("In parseUniforms ====After getProgramHandle\n");	
         // Link the whole program together.
         glLinkProgram( progHandle );

         // Check for link success
         GLint linkResult = 0;
         glGetProgramiv( progHandle, GL_LINK_STATUS, &linkResult );
         glUseProgram( progHandle );
     printf("In parseUniforms ====Before  updateAllUniforms\n");
         updateAllUniforms( progHandle );
     printf("In parseUniforms ====After  updateAllUniforms\n");
         if ( linkResult == 0 )
         {
            char8 log[1000];
            GLint length;
            glGetProgramInfoLog( progHandle, 1000, &length, log );
            cout << "Failed to link program object.";
            cout << log << endl;
            return false;
         }
      }
      else if ( sItem.empty() || sItem[0]=='/' )
      {
         // Either an empty line or a comment
         addToVec = false;
      }
      else
      {
         addToVec = false;
         cout << "Unsupported keyword in uniform file." << endl;
         assert(0);
      }
   
      if ( addToVec )	
      {
		//m_uniforms->UniformDList[m_uniforms->size]=newUniform;
		m_uniforms->size++;
		//delete newUniform;
  printf("m_uniforms->UniformDList[%d]->m_name=%s,,m_uniforms->UniformDList[%d]->m_type=%d\n",m_uniforms->size-1, m_uniforms->UniformDList[m_uniforms->size-1]->m_name,m_uniforms->size-1,m_uniforms->UniformDList[m_uniforms->size-1]->m_type);
         //m_uniforms.push_back(newUniform);			//put this uniform object into uniform vector
      }
      else
      {
         //delete newUniform;
         newUniform=NULL;
      }
	   printf("In parseUniforms ====link end\n");
   } //end of while ( ! fin.eof() )
     fin.close();
   return true;
}

//=================================================================================================================================
///
/// Updates the value of a uniform
///
/// \param name - The name we gave to the uniform
/// \param vals - An array of values we want to to replace the current uniform values with
///
/// \return true=pass ... false=fail
//=================================================================================================================================
bool Uniform::updateOneUniform( const char8* name, float32* vals )
{
   bool updated = false;
   
   UniformData* updatedUniformData; /*After confirmed, it is the uniformData to be updated*/
   UniformData* current;           /*currently in processing UniformData*/
 
   uint32 i;
   for(i=0;i< m_uniforms->size;i++)   
   {
		  current = m_uniforms->UniformDList[i];

		  if ( strcmp( name, current->m_name ) == 0 )
		  {
			updated = true;
			updatedUniformData=current;
			break;
		  }
   }
       
   if ( updated )
   {
       switch( updatedUniformData->m_type )
       {
         case SAMPLE_FLOAT:
            memcpy( updatedUniformData->m_floatData, vals, sizeof(float32) * 1 );
            glUniform1f( updatedUniformData->m_location, updatedUniformData->m_floatData[0] );
            break;
         case SAMPLE_FLOAT_VEC2:
            memcpy( updatedUniformData->m_floatData, vals, sizeof(float32) * 2 );
            glUniform2f( updatedUniformData->m_location, updatedUniformData->m_floatData[0], updatedUniformData->m_floatData[1] );
            break;
         case SAMPLE_FLOAT_VEC3:
            memcpy( updatedUniformData->m_floatData, vals, sizeof(float32) * 3 );
            glUniform3f( updatedUniformData->m_location, updatedUniformData->m_floatData[0], updatedUniformData->m_floatData[1], updatedUniformData->m_floatData[2] );
            break;
         case SAMPLE_FLOAT_VEC4:
            memcpy( updatedUniformData->m_floatData, vals, sizeof(float32) * 4 );
            glUniform4f( updatedUniformData->m_location, updatedUniformData->m_floatData[0], updatedUniformData->m_floatData[1], updatedUniformData->m_floatData[2], updatedUniformData->m_floatData[3] );
            break;
         case SAMPLE_FLOAT_MAT4:
            memcpy( updatedUniformData->m_matrixData, vals, sizeof(float32) * 16 );
            glUniformMatrix4fv( updatedUniformData->m_location, 1, GL_FALSE, &updatedUniformData->m_matrixData[0][0] );
            break;
         default:
            assert(0);
            break;
         }
    }
   else
   {
          assert( 0 ); // They probably passed in an a non-texture type or an invalid name
   }
     
  
   
   //assert( updated == 1 ); //assert( updated == true ); // They probably passed in un unsupported type or invalid name
   return updated;
}

//=================================================================================================================================
///
/// Gets the program handle
///
/// \param name - The name we gave to the program
///
/// \return true=pass ... false=fail
//=================================================================================================================================
GLuint Uniform::getProgramHandle( const char8* name )
{
   bool updated = false;
   GLuint ret = 0;
  
   uint32 i;
   UniformData* updatedUniformData; /*Indicate UniformData with updated==1*/
    UniformData* current;           /*Indicate UniformData currently in loop*/
 //printf(" In etProgramHandle m_uniforms->size=%d\n", m_uniforms->size);

   for(i=0;i< m_uniforms->size;i++)            
   {
		
		  current = m_uniforms->UniformDList[i];   
		 //printf(" In etProgramHandle i=%d,current->m_name=%s  \n",i,current->m_name );
		  if ( strcmp( name, current->m_name ) == 0 )
		  {
			updated = true;
			updatedUniformData=current;
			break;
	    
		  }
   }
   if ( updated )
   {
    	  switch( updatedUniformData->m_type )
         {
			 case SAMPLE_PROGRAM:
				ret = updatedUniformData->m_progHandle;
				break;
			 default:
				assert(0);
         }
   }
   else
   {
      assert( 0 ); // They probably passed in an a non-texture type or an invalid name
   }
  //assert(updated==1);   //assert( updated == true ); // They probably passed in un unsupported type or invalid name
   return ret;
}

//=================================================================================================================================
///
/// Binds a texture in GL
///
/// \param name - The name we gave to the texture
/// \param width - The width of the texture
/// \param height - The height of the texture
///
/// \return true=pass ... false=fail
//=================================================================================================================================
bool Uniform::bindTexture( const char8* name, int32 width, int32 height )
{
   bool updated = false;
   
   UniformData* current;
   UniformData* updatedUniformData; 
   uint32 i;
   for(i=0;i< m_uniforms->size;i++)                //for ( index = m_uniforms.begin(); index != m_uniforms.end() ; ++index)
   {
        current = m_uniforms->UniformDList[i];
	

        if ( strcmp( name, current->m_name ) == 0 )
        {
			 if ( current->m_type == SAMPLE_INT && current->m_textureHandle )
			 {
			    updated = true;
			    updatedUniformData=current;
			 }
			 break;
       }
   }
   
   if ( updated )
   {
      GLint curProg, uniformLocation;
      glGetIntegerv( GL_CURRENT_PROGRAM, &curProg );
      uniformLocation = glGetUniformLocation( curProg, updatedUniformData->m_name );
      glUniform1i( uniformLocation, updatedUniformData->m_textureUnit );
      glActiveTexture( GL_TEXTURE0 + updatedUniformData->m_textureUnit );
      glBindTexture( GL_TEXTURE_2D, updatedUniformData->m_textureHandle );

      if (updatedUniformData->m_width != width || updatedUniformData->m_height != height )
      {
         if ( width != 0 && height != 0 ) // If these are 0 then we just want to keep the current size
         {
            updatedUniformData->m_width = width;
            updatedUniformData->m_height = height;
            if ( updatedUniformData->m_depthTex )
            {
               glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
            }
            else
            {
               glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
            }
         }
      }
   }
   else
   {
      assert( 0 ); // They probably passed in an a non-texture type or an invalid name
   }
   return updated;
}

//=================================================================================================================================
///
/// Binds an FBO in GL
///
/// \param name - The name of the fbo
/// \param width - The width of the fbo
/// \param height - The height of the fbo
///
/// \return true=pass ... false=fail
//=================================================================================================================================
bool Uniform::bindFbo( const char8* name, int32 width, int32 height )
{
   bool updated = false;
    UniformData* current;
   UniformData* updatedUniformData;
  
   uint32 i;
   for(i=0;i< m_uniforms->size;i++)               
   {
		  current = m_uniforms->UniformDList[i];  
		  	 
		  if ( strcmp( name, current->m_name ) == 0 )
		  {
			 if ( current->m_type == SAMPLE_INT && current->m_textureHandle && current->m_fboHandle )
			 {
				updated = true;
				updatedUniformData=current;
			 }
			 break;
		  }
   }
   
   if ( updated )
   {
#ifdef USE_STANDARD_GL
      glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, updatedUniformData->m_fboHandle );
#else
      glBindFramebuffer( GL_FRAMEBUFFER, updatedUniformData->m_fboHandle );
#endif // USE_STANDARD_GL

      if (updatedUniformData->m_width != width || updatedUniformData->m_height != height )
      {
         GLint curTexture;
         glGetIntegerv( GL_TEXTURE_BINDING_2D, &curTexture );

         updatedUniformData->m_width = width;
         updatedUniformData->m_height = height;
         glBindTexture( GL_TEXTURE_2D,  updatedUniformData->m_textureHandle );
         if ( updatedUniformData->m_depthTex )
         {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
         }
         else
         {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
         }

         glBindTexture( GL_TEXTURE_2D, curTexture );

#ifdef USE_STANDARD_GL
         if ( updatedUniformData->m_depthTex )
         {
            if (!updatedUniformData->m_colorRbHandle)
            {
               glGenRenderbuffersEXT( 1, &updatedUniformData->m_colorRbHandle );
            }
            glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, updatedUniformData->m_colorRbHandle );
            glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_RGB, width, height );
            glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, updatedUniformData->m_colorRbHandle );
            glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, updatedUniformData->m_textureHandle, 0 );
         }
         else
         {
            glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, updatedUniformData->m_textureHandle, 0 );
         }
#else
         if ( updatedUniformData->m_depthTex )
         {
            if(!updatedUniformData->m_colorRbHandle)
            {
               glGenRenderbuffers( 1, &updatedUniformData->m_colorRbHandle );
            }
            glBindRenderbuffer( GL_RENDERBUFFER, updatedUniformData->m_colorRbHandle );
            glRenderbufferStorage( GL_RENDERBUFFER, GL_RGB, width, height );
            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, updatedUniformData->m_colorRbHandle );
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, updatedUniformData->m_textureHandle, 0 );
         }
         else
         {
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, updatedUniformData->m_textureHandle, 0 );
         }
#endif // USE_STANDARD_GL
      }
   }
   else
   {
      assert( 0 ); // They probably passed in an a non-texture type or an invalid name
   }
   return updated;
}


//=================================================================================================================================
///
/// Deletes all the GL resources that we have loaded
///
/// \param name - The name we gave to the program
///
/// \return true=pass ... false=fail
//=================================================================================================================================
void Uniform::freeAllData()
{
    UniformData* current;
   uint32 i;
   for(i=0;i< m_uniforms->size;i++)         
   {
      current = m_uniforms->UniformDList[i];
	 
      if ( current->m_textureHandle )
      {
         glDeleteTextures(1,&( current->m_textureHandle)); 
      }
      if (  current->m_vertShaderHandle )
      {
         glDeleteShader(  current->m_vertShaderHandle );
      }
      if ( current->m_fragShaderHandle )
      {
         glDeleteShader(  current->m_fragShaderHandle );
      }
      if (  current->m_progHandle )
      {
         glDeleteProgram(  current->m_progHandle );
      }

#ifdef USE_STANDARD_GL
      if ( current->m_fboHandle )
      {
         glDeleteFramebuffersEXT(1, & current->m_fboHandle );
      }
      if (  current->m_colorRbHandle )
      {
         glDeleteRenderbuffersEXT(1, & current->m_colorRbHandle );
      }
#else
      if (  current->m_fboHandle )
      {
         glDeleteFramebuffers(1, & current->m_fboHandle );
      }
      if (  current->m_colorRbHandle )
      {
         glDeleteRenderbuffers(1, & current->m_colorRbHandle );
      }
#endif // USE_STANDARD_GL
   }
   if(m_uniforms!=NULL)
   {  
		  if(m_uniforms->UniformDList!=NULL)
		  {
			   for ( uint32 i = 0; i < m_uniforms->capacity ; i++ )
			   {
				   if ( m_uniforms->UniformDList[i] != NULL )
				   {
						delete m_uniforms->UniformDList[i]; //free(m_uniforms->UniformDList[i]);
						m_uniforms->UniformDList[i]=NULL;	
				   }
			   }
			  free( m_uniforms->UniformDList);
			  m_uniforms->UniformDList=NULL;
		  }
		  free(m_uniforms);
		  m_uniforms=NULL;
   }
}

//=================================================================================================================================
///
/// Updates all the uniform data after a link
///
/// \param void
///
/// \return void
//=================================================================================================================================
void Uniform::updateAllUniforms( GLuint curProg )
{
   UniformData* current;
   uint32 i;
   for(i=0;i< m_uniforms->size;i++)         
   {
      current = m_uniforms->UniformDList[i];  

 
      if ( curProg == current->m_attachedProgram )
      {
         switch( current->m_type )
         {
         case SAMPLE_FLOAT:
            glUniform1f( current->m_location, current->m_floatData[0] );
            break;
         case SAMPLE_FLOAT_VEC2:
            glUniform2f( current->m_location, current->m_floatData[0], current->m_floatData[1] );
            break;
         case SAMPLE_FLOAT_VEC3:
            glUniform3f( current->m_location, current->m_floatData[0], current->m_floatData[1], current->m_floatData[2] );
            break;
         case SAMPLE_FLOAT_VEC4:
            glUniform4f( current->m_location, current->m_floatData[0], current->m_floatData[1], current->m_floatData[2], current->m_floatData[3] );
            break;
         case SAMPLE_FLOAT_MAT4:
            glUniformMatrix4fv( current->m_location, 1, GL_FALSE, &current->m_matrixData[0][0] );
            break;
         case SAMPLE_INT:
            if ( current->m_location != 0 ) // Prevents error on nullTexture which has not yet been given a location (done at bind time)
            {
               glUniform1i( current->m_location, current->m_textureUnit );
            }
            break;
         case SAMPLE_PROGRAM:
            break;
         default:
            assert(0);
            break;
         }
      }
   }
}