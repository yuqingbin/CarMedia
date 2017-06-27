#ifndef _SCENE_H_
#define _SCENE_H_

#include <iostream>
#include <fstream>
#include "eUtil.h"
#include "ArgDefines.h"
#include "Misc.h"
#include <Windows.h>
#include "TEF6606.h"

typedef struct TGAImage											{
	GLubyte	*imageData;											// 	图像数据
	GLuint	bpp;												// 	像素颜色深度
	GLuint	width;												//      图像宽度
	GLuint	height;												// 	图像高度
	GLuint	texID;																	//	纹理ID
} TGAImage;	


class Scene
{
public:
   
   Scene();// \brief Constructor.
   ~Scene();  // \brief Destructor.

private:

	GLuint   s_progHandle;

	GLuint   *MapTexId;
	

	GLfloat  *vertices;
	GLuint   *indices;
	int      numIndices;
	GLfloat  *normals;

	GLint    offsetLoc;
	GLint    colsetLoc;
	GLint    normalLoc;
	GLuint   mvpLoc;	
	GLint    texCoordLoc;	
	GLint    positionLoc;
	GLint    samplerLoc;		
	GLint    colorLoc;
	GLint    samplerCube;
	GLint    switchLoc;

	float32 m_clearColor[4];

	EMatrix  PerMatrix;
	EMatrix  MvpMatrix;
	BOOL *A1,*A2,*A3,*B1,*B2,*B3,*C1,*C2,*C3,*D1,*D2,*D3,*E1,*E2,*E3;
	BOOL TV_FLAG,BLUETOOTH_FLAG,FM_FLAG,USB_FLAG,NAVIGATE_FLAG;
	int shi;
	int bai;
	//int MenuNum;
	
    void ShutDown ( );

public:
	 GLuint   *BmpTexId;
	 GLuint   *MP3TexId;

	GLuint LoadBMP(TGAImage *texture, TCHAR *filename);
	GLuint LoadBMP(TGAImage *texture, BYTE *buffer,int width,int height,BOOL flg);
	int PicCount;
	bool LoadTextureArray(TCHAR* dataPath,int begin,int end);
	TCHAR * dataPath;
	virtual bool InitView(); 
	int CreateEGLContext( HWND hWnd) ;
	void eDestroyContext ( );
	GLuint TextureCube(GLint COLOR_FLAG );
	GLuint LoadTexture(TCHAR *fileName,int index);
    //动画定义
    void Coordinate();//坐标初始化
	void RadioInit();
	void RadioNum(GLint NUM4,GLint NUM3,GLint NUM2,GLint NUM1 ,RADIOMODE FA_FLAG,BOOL JUMP);
	void RadioMat(GLint NUM_FLAG,EMatrix &Mat,EMatrix &ComMat,GLfloat x);
	void MenuInit( );
	void MenuMat(GLuint TexId,EMatrix &RotMat,EMatrix &ModMat,EMatrix &PerMat,EMatrix &ComMat,GLfloat angle,GLint t,GLfloat ry,GLfloat rdx,GLfloat sx,GLfloat sy,GLfloat x,GLfloat c);
	void MenuStat(GLint MenuNum,GLint BgNUM );
	void MenuRight(GLint MenuNum,GLint BgNUM );
	void MenuLeft(GLint MenuNum,GLint BgNUM );
	void ClockInit( );
	void Clock(GLint angle);

	void DocManageInit( );
	void DocMat(GLuint TexId,EMatrix &PerMat,EMatrix &ModMat,EMatrix &ComMat,GLfloat angle,GLint t,GLfloat sx,GLfloat sy,GLfloat x,GLfloat y);
	void DocManageStat(GLint DocNum);
	void DocManageRight(GLint DocNum);
	void DocManageLeft(GLint DocNum);
	void BluetoothInit();
	void BluetoothMat(GLuint TexId,EMatrix &Mat,EMatrix &ComMat,GLfloat y,GLfloat c);
	void BluetoothCall();
	void BluetoothDial();
	void BluetoothRope();
	void PicInit();
	void PicMultiScan(GLint Cpos,BOOL LISTFLAG);
	void PicMultiScanMove(GLfloat a);
	void PicPackMat(GLuint TexId,EMatrix &Mat,GLfloat x,GLfloat y,GLfloat c,GLfloat angle,GLint t);
	int  PicMultiDown(GLint PicNum,GLint Cpos);
	void PicMultiUp(GLint PicNum,GLint Cpos);
	int  PicSingleScan(GLint PicNum,GLint Cpos );
	void AudioInit( );
	void AudioMat(GLuint TexId,EMatrix &Mat,EMatrix &ComMat,GLfloat tz,GLfloat x,GLfloat y);
	void AudioStat(GLint MP3Num);
	void AudioPrev(GLint MP3Num);
	void AudioNext(GLint MP3Num);
	void glTextShow(int x, int y, WCHAR *string);

 struct EglStruct
{
   EGLDisplay  dsp;
   EGLConfig   cfg;
   EGLContext  cxt;
   EGLSurface  surf;
} g_egl;

 HBITMAP hAxisBackgd;  /*axisBackgd.bmp*/
  
   GLint       width;/// Window width  
   GLint       height;/// Window height
};

#endif // _SCENE_H_
