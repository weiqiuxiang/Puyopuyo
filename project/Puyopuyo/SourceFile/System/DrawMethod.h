/*==========================================================================================
ヘッダファイル説明:
	描画関数のクラスのヘッダ
==========================================================================================*/
#ifndef DRAW_METHOD_H_
#define DRAW_METHOD_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	クラス定義
==========================================================================================*/
class DrawMethod 
{
public:
	//四角描画
	static int DrawRect2D(const Vector2& pos,const Vector2& Size,const int& GraphHandle);
	static int DrawRect2DCenter(const Vector2& CenterPos, const Vector2& Size, const int& GraphHandle);
	static int DrawRect2DScale(const Vector2& CenterPos, const Vector2& ScaleCenterPos,const Vector2& Size,const Vector2f& ScaleRate,const int& GraphHandle);
};

#endif