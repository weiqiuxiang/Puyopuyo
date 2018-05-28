/*==========================================================================================
ヘッダファイル説明:
描画関数のクラスのヘッダ
==========================================================================================*/
#include "DrawMethod.h"
#include <math.h>
#include "DxLib.h"

/*==========================================================================================
	四角ポリゴン描画
==========================================================================================*/
int DrawMethod::DrawRect2D(const Vector2& pos, const Vector2& Size, const int& GraphHandle)
{
	int result = DrawExtendGraph(pos.x, pos.y, pos.x + Size.x, pos.y + Size.y , GraphHandle,TRUE);
	return result;
}

/*==========================================================================================
	四角ポリゴン描画(中心座標基準)
==========================================================================================*/
int DrawMethod::DrawRect2DCenter(const Vector2& CenterPos, const Vector2& Size, const int& GraphHandle)
{
	int result = DrawExtendGraph(
		(int)((float)CenterPos.x - (float)Size.x*0.5f),
		(int)((float)CenterPos.y - (float)Size.y*0.5f),
		(int)((float)CenterPos.x + (float)Size.x*0.5f),
		(int)((float)CenterPos.y + (float)Size.y*0.5f),
		GraphHandle, 
		TRUE
	);
	return result;
}

/*==========================================================================================
	四角ポリゴンのスケール描画(中心座標基準)
	引数説明:
		CenterPos: ポリゴン中心座標
		ScaleCenterPos: スケール中心座標
		Size: ポリゴンサイズ
		ScaleRate: スケール率(X軸とY軸)
		GraphHandle: グラフィックハンドル
==========================================================================================*/
int DrawMethod::DrawRect2DScale(const Vector2& CenterPos, const Vector2& ScaleCenterPos, const Vector2& Size, const Vector2f& ScaleRate, const int& GraphHandle)
{
	//スケール後の中心座標位置計算
	Vector2 CenterToScaleCenterVec;
	CenterToScaleCenterVec.x = (int)( (ScaleCenterPos.x - CenterPos.x) * ScaleRate.x);
	CenterToScaleCenterVec.y = (int)( (ScaleCenterPos.y - CenterPos.y) * ScaleRate.y);

	Vector2 ChangedCenterPos;
	ChangedCenterPos.x = ScaleCenterPos.x - CenterToScaleCenterVec.x;
	ChangedCenterPos.y = ScaleCenterPos.y - CenterToScaleCenterVec.y;

	//スケール後のサイズ計算
	Vector2 ChangedSize;
	ChangedSize.x = (int)(Size.x * ScaleRate.x);
	ChangedSize.y = (int)(Size.y * ScaleRate.y);

	//描画
	int DrawResult = DrawRect2D(ChangedCenterPos, ChangedSize, GraphHandle);
	return DrawResult;
}