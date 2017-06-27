//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Various helpful functions that are project specific.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/esTriangle/src/Misc.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================
#include "SimpleImageLoader.h"
#include "Misc.h"
#include "assert.h"
#include "comm1.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h> //#include <GLES/gl.h>
// Global holding all the global egl data
//EglStruct g_egl;


//=================================================================================================================================
///
/// Builds the egl attrib list
///
/// \param attribList - The egl attrib list
///
/// \return void
//=================================================================================================================================
#define BUFSIZ 256



//=================================================================================================================================
///
/// Sets up the textures including compression (located here for convienience.
///
/// \param filename - The file name of the texture to be loaded
/// \param texUnit - The texture unit we want to use for this texture
///
/// \return - The ES texture bind number
//=================================================================================================================================
GLuint LoadCompressedTexture( const char8 *filename, const char8 *sType, GLuint texUnit )
{
   assert( 0 );
   return 0;
}



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
	const float fZ)
{
	mOut[ 0]=1.0f;	mOut[ 4] =0.0f;	mOut[8]=0.0f;	mOut[12]=fX;
	mOut[ 1]=0.0f;	mOut[ 5] =1.0f;	mOut[9]=0.0f;	mOut[13]=fY;
	mOut[ 2]=0.0f;	mOut[ 6] =0.0f;	mOut[10]=1.0f;	mOut[14]=fZ;
	mOut[ 3]=0.0f;	mOut[ 7] =0.0f;	mOut[11]=0.0f;	mOut[15]=1.0f;
}

/*!***************************************************************************
 @Function Name		PVRTMatrixRotationYF
 @Output			mOut	Rotation matrix
 @Input				fAngle	Angle of the rotation
 @Description		Create an Y rotation matrix mOut.
*****************************************************************************/
void myMatrixRotationYF(
	float* mOut,
	const float fAngle)
{
	float		fCosine, fSine;

	/* Precompute cos and sin */
#if defined(BUILD_DX9) || defined(BUILD_D3DM) || defined(BUILD_DX10)
	fCosine	= (float)PVRTFCOS(-fAngle);
    fSine	= (float)PVRTFSIN(-fAngle);
#else
	fCosine	= (float)cos(fAngle);
    fSine	= (float)sin(fAngle);
#endif

	/* Create the trigonometric matrix corresponding to Y Rotation */
	mOut[ 0]=fCosine;	mOut[4] =0.0f;	mOut[8]=-fSine;	mOut[12] =0.0f;
	mOut[ 1]=0.0f;		mOut[5] =1.0f;	mOut[9]=0.0f;		mOut[13] =0.0f;
	mOut[ 2]=fSine;	mOut[6] =0.0f;	mOut[10]=fCosine;	mOut[14] =0.0f;
	mOut[ 3]=0.0f;		mOut[7] =0.0f;	mOut[11]=0.0f;		mOut[15] =1.0f;
}



/*!***************************************************************************
 @Function Name		PVRTMatrixRotationZF
 @Output			mOut	Rotation matrix
 @Input				fAngle	Angle of the rotation
 @Description		Create an Z rotation matrix mOut.
*****************************************************************************/
void myMatrixRotationZF(
	float *	mOut,
	const float fAngle)
{
	float		fCosine, fSine;

	/* Precompute cos and sin */
#if defined(BUILD_DX9) || defined(BUILD_D3DM) || defined(BUILD_DX10)
	fCosine =	(float)PVRTFCOS(-fAngle);
    fSine =		(float)PVRTFSIN(-fAngle);
#else
	fCosine =	(float)cos(fAngle);
    fSine =		(float)sin(fAngle);
#endif

	/* Create the trigonometric matrix corresponding to Z Rotation */
	mOut[0] = fCosine;		mOut[4] = fSine;	mOut[8] = 0.0f;		mOut[12] = 0.0f;
	mOut[1] = -fSine;		mOut[5] = fCosine;	mOut[9] = 0.0f;		mOut[13] = 0.0f;
	mOut[2] = 0.0f;		mOut[6] = 0.0f;	mOut[10]= 1.0f;		mOut[14] = 0.0f;
	mOut[3] = 0.0f;		mOut[7] = 0.0f;	mOut[11]= 0.0f;		mOut[15] = 1.0f;
}
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
	 float * mB)
{
	float *mRet; mRet=mOut;


	/* Perform calculation on a dummy matrix (mRet) */
	mRet[0] = mA[0]*mB[0] + mA[1]*mB[4] + mA[2]*mB[8]  + mA[3]*mB[12];
	mRet[1] = mA[0]*mB[1] + mA[1]*mB[5] + mA[2]*mB[9]  + mA[3]*mB[13];
	mRet[2] = mA[0]*mB[2] + mA[1]*mB[6] + mA[2]*mB[10] + mA[3]*mB[14];
	mRet[3] = mA[0]*mB[3] + mA[1]*mB[7] + mA[2]*mB[11] + mA[3]*mB[15];

	mRet[4] = mA[4]*mB[0] + mA[5]*mB[4] + mA[6]*mB[4]  + mA[7]*mB[12];
	mRet[5] = mA[4]*mB[1] + mA[5]*mB[5] + mA[6]*mB[9]  + mA[7]*mB[13];
	mRet[6] = mA[4]*mB[2] + mA[5]*mB[6] + mA[6]*mB[10] + mA[7]*mB[14];
	mRet[7] = mA[4]*mB[3] + mA[5]*mB[7] + mA[6]*mB[11] + mA[7]*mB[15];

	mRet[8]  = mA[8]*mB[0] + mA[9]*mB[4] + mA[10]*mB[8]  + mA[11]*mB[12];
	mRet[9]  = mA[8]*mB[1] + mA[9]*mB[5] + mA[10]*mB[9]  + mA[11]*mB[13];
	mRet[10] = mA[8]*mB[2] + mA[9]*mB[6] + mA[10]*mB[10] + mA[11]*mB[14];
	mRet[11] = mA[8]*mB[3] + mA[9]*mB[7] + mA[10]*mB[11] + mA[11]*mB[15];

	mRet[12] = mA[12]*mB[0] + mA[13]*mB[4] + mA[14]*mB[8] + mA[15]*mB[12];
	mRet[13] = mA[12]*mB[1] +mA[13]*mB[5] + mA[14]*mB[9] + mA[15]*mB[13];
	mRet[14] = mA[12]*mB[2] + mA[13]*mB[6] + mA[14]*mB[10] + mA[15]*mB[14];
	mRet[15] = mA[12]*mB[3] + mA[13]*mB[7] + mA[14]*mB[11] + mA[15]*mB[15];

	/* Copy result in pResultMatrix */
	//mOut = mRet;
}



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
	const bool  bRotate)
{
	float f, n, fRealAspect;

	if (bRotate)
		fRealAspect = 1.0f / fAspect;
	else
		fRealAspect = fAspect;

	// cotangent(a) == 1.0f / tan(a);
	f = 1.0f / (float)tan(fFOVy * 0.5f);
	n = 1.0f / (fFar - fNear);

	mOut[ 0] = f / fRealAspect;
	mOut[ 1] = 0;
	mOut[ 2] = 0;
	mOut[ 3] = 0;

	mOut[ 4] = 0;
	mOut[ 5] = f;
	mOut[ 6] = 0;
	mOut[ 7] = 0;

	mOut[ 8] = 0;
	mOut[ 9] = 0;
	mOut[10] = fFar * n;
	mOut[11] = 1;

	mOut[12] = 0;
	mOut[13] = 0;
	mOut[14] = -fFar * fNear * n;
	mOut[15] = 0;

	if (bRotate)
	{
		float * mRotation, *mTemp = mOut;
		mRotation=(float*)calloc(16,sizeof(float));

		myMatrixRotationZF(mRotation, 90.0f*PVRT_PIf/180.0f);
		myMatrixMultiplyF(mOut, mTemp, mRotation);
		if(mRotation) {free(mRotation); mRotation=NULL;}

	}
}

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
	const bool  bRotate)
{
	float f, n, fRealAspect;

	if (bRotate)
		fRealAspect = 1.0f / fAspect;
	else
		fRealAspect = fAspect;

	// cotangent(a) == 1.0f / tan(a);
	f = 1.0f / (float)tan(fFOVy * 0.5f);
	n = 1.0f / (fNear - fFar);

	mOut[0]=f / fRealAspect;
	mOut[1] = 0;
	mOut[2] = 0;
	mOut[3] = 0;

	mOut[4] = 0;
	mOut[5] = f;
	mOut[6] = 0;
	mOut[7] = 0;

	mOut[8] = 0;
	mOut[9] = 0;
	mOut[10] = (fFar + fNear) * n;
	mOut[11] = -1;

	mOut[12] = 0;
	mOut[13] = 0;
	mOut[14] = (2 * fFar * fNear) * n;
	mOut[15] = 0;

	if (bRotate)
	{
		float  *mRotation, *mTemp; mTemp = mOut;
		mRotation=(float*)calloc(16,sizeof(float));
		myMatrixRotationZF(mRotation, -90.0f*PVRT_PIf/180.0f);
		myMatrixMultiplyF(mOut, mTemp, mRotation);
		if(mRotation) {free(mRotation); mRotation=NULL;}
	}
}
/*!***************************************************************************
 @Function			PVRTMatrixVec3NormalizeF
 @Output			vOut	Normalized vector
 @Input				vIn		Vector to normalize
 @Description		Normalizes the supplied vector.
*****************************************************************************/
void myMatrixVec3NormalizeF(
	PVRTVECTOR3f		&vOut,
	const PVRTVECTOR3f	&vIn)
{
	float	f;
	double temp;

	temp = (double)(vIn.x * vIn.x + vIn.y * vIn.y + vIn.z * vIn.z);
	temp = 1.0 / sqrt(temp);
	f = (float)temp;

	vOut.x = vIn.x * f;
	vOut.y = vIn.y * f;
	vOut.z = vIn.z * f;
}

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
	const PVRTVECTOR3f	&v2)
{
    PVRTVECTOR3f result;

	/* Perform calculation on a dummy VECTOR (result) */
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;

	/* Copy result in pOut */
	vOut = result;
}
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
	const PVRTVECTOR3f	&vUp)
{
	PVRTVECTOR3f f, //from camera to target as new revised z axis direction
		vUpActual, //normalized camera up direction
		s,          //normalized new x axis 
		u;          //normalized new y axis
	float *	t;

	f.x = vAt.x - vEye.x;
	f.y = vAt.y - vEye.y;
	f.z = vAt.z - vEye.z;

	myMatrixVec3NormalizeF(f, f);
	myMatrixVec3NormalizeF(vUpActual, vUp);
	myMatrixVec3CrossProductF(s, f, vUpActual);// new x axis
	myMatrixVec3CrossProductF(u, s, f);

	mOut[0] = s.x;
	mOut[1] = u.x;
	mOut[2] = -f.x;
	mOut[3] = 0;

	mOut[4] = s.y;
	mOut[5] = u.y;
	mOut[6] = -f.y;
	mOut[7] = 0;

	mOut[8]= s.z;
	mOut[9]= u.z;
	mOut[10]= -f.z;
	mOut[11]= 0;

	mOut[12]= 0;
	mOut[13]= 0;
	mOut[14]= 0;
	mOut[15]= 1;
	t=(float*)calloc(16,sizeof(float));
	myMatrixTranslationF(t, -vEye.x, -vEye.y, -vEye.z); //translate the Orginal to camera persition
	myMatrixMultiplyF(mOut, t, mOut);
	if(t) {free(t); t=NULL;}

}

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
	const PVRTVECTOR3f	&vUp)
{
	PVRTVECTOR3f	f, vUpActual, s, u;
	float*		t;

	f.x = vEye.x - vAt.x;
	f.y = vEye.y - vAt.y;
	f.z = vEye.z - vAt.z;

	myMatrixVec3NormalizeF(f, f);
	myMatrixVec3NormalizeF(vUpActual, vUp);
	myMatrixVec3CrossProductF(s, f, vUpActual);
	myMatrixVec3CrossProductF(u, s, f);

	mOut[ 0] = s.x;
	mOut[ 1] = u.x;
	mOut[ 2] = -f.x;
	mOut[ 3] = 0.0f;

	mOut[ 4] = s.y;
	mOut[ 5] = u.y;
	mOut[ 6] = -f.y;
	mOut[ 7] = 0.0f;

	mOut[ 8] = s.z;
	mOut[ 9] = u.z;
	mOut[10] = -f.z;
	mOut[11] = 0.0f;

	mOut[12] = 0.0f;
	mOut[13] = 0.0f;
	mOut[14] = 0.0f;
	mOut[15] = 1.0f;
t=(float*)calloc(16,sizeof(float));

	myMatrixTranslationF(t, -vEye.x, -vEye.y, -vEye.z);
	myMatrixMultiplyF(mOut, t, mOut);
if(t) {free(t); t=NULL;}

}

/*!***************************************************************************
 @Function			PVRTVec3
 @Input				fX	X component of vector
 @Input				fY	Y component of vector
 @Input				fZ	Z component of vector
 @Description		Simple constructor from 3 values.
*****************************************************************************/
	PVRTVec3 myPVRTVec3(VERTTYPE fX, VERTTYPE fY, VERTTYPE fZ)
	{
		PVRTVec3 tv3;
		tv3.x = fX;	tv3.y = fY;	tv3.z = fZ;
		return tv3;
	}


/*!***************************************************************************
 @Function			multiple value constructor
 @Input				fX value of x component
 @Input				fY value of y component
 @Input				fZ value of z component
 @Input				fW value of w component
 @Description		Constructs a PVRTVec4 from 4 separate values
****************************************************************************/
	PVRTVec4 myPVRTVec4(VERTTYPE fX, VERTTYPE fY, VERTTYPE fZ, VERTTYPE fW)
	{
		PVRTVec4 tv4;
		tv4.x = fX;	tv4.y = fY;	tv4.z = fZ;	tv4.w = fW;
		return tv4;;
	}

	///
// esLogMessage()
//
//    Log an error message to the debug output for the platform
//
//void ESUTIL_API eLogMessage ( const char *formatStr, ... )
void eLogMessage ( const char *formatStr, ... )
{
    va_list params;
    char buf[BUFSIZ];

    va_start ( params, formatStr );
    vsprintf_s ( buf, sizeof(buf),  formatStr, params );
    
    printf ( "%s", buf );
    
    va_end ( params );
}

