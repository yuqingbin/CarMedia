//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Definitions for the Uniform class.  This class loads textures, attributes, and uniforms.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/esTriangle/src/Misc.h#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#ifndef _MISC_H_
#define _MISC_H_

#include <EGL/egl.h>
//#include <GLES/gl.h> 
#include <GLES2/gl2.h>
#include "ArgDefines.h"



#define VERTTYPE float

#define MAX_EGL_ATTRIBUTES 64
#define PVRT_PIf			(3.1415926535f)

// Index to bind the attributes to vertex shaders
#define VERTEX_ARRAY	0
#define NORMAL_ARRAY	1
#define COLOR_ARRAY		2
#define TEXCOORD_ARRAY	3
const float g_fCameraNear = 2.0f;
const float g_fCameraFar  = 5000.0f;
const float g_FOV = 0.78539819f;
const float prefWidth=800.0f;
const float prefHeight=480.0f;





/// \brief Builds the egl attrib list
void BuildAttribList( EGLint *attribList );



/// \brief Sets up the textures including compression (located here for convienience
GLuint LoadCompressedTexture( const char8 *filename, const char8 *sType, GLuint texUnit );




typedef struct
{
	float x;	/*!< x coordinate */
	float y;	/*!< y coordinate */
	float z;	/*!< z coordinate */
} PVRTVECTOR3f;

typedef PVRTVECTOR3f PVRTVec3;

typedef struct
{
	float x;	/*!< x coordinate */
	float y;	/*!< y coordinate */
} PVRTVECTOR2f;
typedef PVRTVECTOR2f PVRTVec2;

typedef struct
{
	float x;	/*!< x coordinate */
	float y;	/*!< y coordinate */
	float z;	/*!< z coordinate */
	float w;	/*!< w coordinate */
} PVRTVECTOR4f;

typedef PVRTVECTOR4f PVRTVec4;
struct SVertex
{
	PVRTVec3	p;
	PVRTVec3	n;
	PVRTVec4	c;
	PVRTVec2	t;
};

enum PVRShellKeyName
{
	PVRShellKeyNameNull,
	PVRShellKeyNameQUIT,
	PVRShellKeyNameSELECT,
	PVRShellKeyNameACTION1,
	PVRShellKeyNameACTION2,
	PVRShellKeyNameUP,
	PVRShellKeyNameDOWN,
	PVRShellKeyNameLEFT,
	PVRShellKeyNameRIGHT,
	PVRShellKeyNameScreenshot
};
enum PVRShellKeyRotate
{
	PVRShellKeyRotateNone=0,
	PVRShellKeyRotate90,
	PVRShellKeyRotate180,
	PVRShellKeyRotate270
};


/*!***************************************************************************
 @Function Name		PVRTMatrixTranslationF
 @Output			mOut	Translation matrix
 @Input				fX		X component of the translation
 @Input				fY		Y component of the translation
 @Input				fZ		Z component of the translation
 @Description		Build a transaltion matrix mOut using fX, fY and fZ.
*****************************************************************************/
void myMatrixTranslationF(
	float *mOut,
	const float fX,
	const float fY,
	const float fZ);

/*!***************************************************************************
 @Function Name		PVRTMatrixRotationYF
 @Output			mOut	Rotation matrix
 @Input				fAngle	Angle of the rotation
 @Description		Create an Y rotation matrix mOut.
*****************************************************************************/
void myMatrixRotationYF(
	float * mOut,
	const float fAngle);

/*!***************************************************************************
 @Function Name		PVRTMatrixRotationZF
 @Output			mOut	Rotation matrix
 @Input				fAngle	Angle of the rotation
 @Description		Create an Z rotation matrix mOut.
*****************************************************************************/
void myMatrixRotationZF(
	float *	mOut,
	const float fAngle);


/*!***************************************************************************
 @Function			PVRTMatrixMultiplyF
 @Output			mOut	Result of mA x mB
 @Input				mA		First operand
 @Input				mB		Second operand
 @Description		Multiply mA by mB and assign the result to mOut
					(mOut = p1 * p2). A copy of the result matrix is done in
					the function because mOut can be a parameter mA or mB.
*****************************************************************************/
void myMatrixMultiplyF(
	float * mOut,
	 float * mA,
	 float * mB);

/*!***************************************************************************
 @Function		PVRTMatrixPerspectiveFovRHF
 @Output		mOut		Perspective matrix
 @Input			fFOVy		Field of view
 @Input			fAspect		Aspect ratio
 @Input			fNear		Near clipping distance
 @Input			fFar		Far clipping distance
 @Input			bRotate		Should we rotate it ? (for upright screens)
 @Description	Create a perspective matrix.
*****************************************************************************/
void myMatrixPerspectiveFovRHF(
	float *mOut,
	const float	fFOVy,
	const float	fAspect,
	const float	fNear,
	const float	fFar,
	const bool  bRotate);

/*!***************************************************************************
 @Function		PVRTMatrixPerspectiveFovLHF
 @Output		mOut		Perspective matrix
 @Input			fFOVy		Field of view
 @Input			fAspect		Aspect ratio
 @Input			fNear		Near clipping distance
 @Input			fFar		Far clipping distance
 @Input			bRotate		Should we rotate it ? (for upright screens)
 @Description	Create a perspective matrix.
*****************************************************************************/
void myMatrixPerspectiveFovLHF(
	float * mOut,
	const float	fFOVy,
	const float	fAspect,
	const float	fNear,
	const float	fFar,
	const bool  bRotate);

/*!***************************************************************************
 @Function			PVRTMatrixVec3NormalizeF
 @Output			vOut	Normalized vector
 @Input				vIn		Vector to normalize
 @Description		Normalizes the supplied vector.
*****************************************************************************/
void myMatrixVec3NormalizeF(
	PVRTVECTOR3f		&vOut,
	const PVRTVECTOR3f	&vIn);

/*!***************************************************************************
 @Function			PVRTMatrixVec3CrossProductF
 @Output			vOut	Cross product of the two vectors
 @Input				v1		First vector
 @Input				v2		Second vector
 @Description		This function performs the cross product of the two
					supplied vectors.
*****************************************************************************/
void myMatrixVec3CrossProductF(
	PVRTVECTOR3f		&vOut,
	const PVRTVECTOR3f	&v1,
	const PVRTVECTOR3f	&v2);

/*!***************************************************************************
 @Function			myMatrixLookAtRHF
 @Output			mOut	Look-at view matrix
 @Input				vEye	Position of the camera
 @Input				vAt		Point the camera is looking at
 @Input				vUp		Up direction for the camera
 @Description		Create a look-at view matrix.
*****************************************************************************/
void myMatrixLookAtRHF(
	float * mOut,
	const PVRTVECTOR3f	&vEye,
	const PVRTVECTOR3f	&vAt,
	const PVRTVECTOR3f	&vUp);


/*!***************************************************************************
 @Function			PVRTMatrixLookAtLHX
 @Output			mOut	Look-at view matrix
 @Input				vEye	Position of the camera
 @Input				vAt		Point the camera is looking at
 @Input				vUp		Up direction for the camera
 @Description		Create a look-at view matrix.
*****************************************************************************/
void myMatrixLookAtLHF(
	float*			&mOut,
	const PVRTVECTOR3f	&vEye,
	const PVRTVECTOR3f	&vAt,
	const PVRTVECTOR3f	&vUp);

/*!***************************************************************************
 @Function			PVRTVec3
 @Input				fX	X component of vector
 @Input				fY	Y component of vector
 @Input				fZ	Z component of vector
 @Description		Simple constructor from 3 values.
*****************************************************************************/
	PVRTVec3 myPVRTVec3(VERTTYPE fX, VERTTYPE fY, VERTTYPE fZ);

/*!***************************************************************************
 @Function			multiple value constructor
 @Input				fX value of x component
 @Input				fY value of y component
 @Input				fZ value of z component
 @Input				fW value of w component
 @Description		Constructs a PVRTVec4 from 4 separate values
****************************************************************************/
	PVRTVec4 myPVRTVec4(VERTTYPE fX, VERTTYPE fY, VERTTYPE fZ, VERTTYPE fW);



void eLogMessage ( const char *formatStr, ... );




#endif // _MISC_H_

