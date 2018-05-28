/*==========================================================================================
�w�b�_�t�@�C������:
�`��֐��̃N���X�̃w�b�_
==========================================================================================*/
#include "DrawMethod.h"
#include <math.h>
#include "DxLib.h"

/*==========================================================================================
	�l�p�|���S���`��
==========================================================================================*/
int DrawMethod::DrawRect2D(const Vector2& pos, const Vector2& Size, const int& GraphHandle)
{
	int result = DrawExtendGraph(pos.x, pos.y, pos.x + Size.x, pos.y + Size.y , GraphHandle,TRUE);
	return result;
}

/*==========================================================================================
	�l�p�|���S���`��(���S���W�)
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
	�l�p�|���S���̃X�P�[���`��(���S���W�)
	��������:
		CenterPos: �|���S�����S���W
		ScaleCenterPos: �X�P�[�����S���W
		Size: �|���S���T�C�Y
		ScaleRate: �X�P�[����(X����Y��)
		GraphHandle: �O���t�B�b�N�n���h��
==========================================================================================*/
int DrawMethod::DrawRect2DScale(const Vector2& CenterPos, const Vector2& ScaleCenterPos, const Vector2& Size, const Vector2f& ScaleRate, const int& GraphHandle)
{
	//�X�P�[����̒��S���W�ʒu�v�Z
	Vector2 CenterToScaleCenterVec;
	CenterToScaleCenterVec.x = (int)( (ScaleCenterPos.x - CenterPos.x) * ScaleRate.x);
	CenterToScaleCenterVec.y = (int)( (ScaleCenterPos.y - CenterPos.y) * ScaleRate.y);

	Vector2 ChangedCenterPos;
	ChangedCenterPos.x = ScaleCenterPos.x - CenterToScaleCenterVec.x;
	ChangedCenterPos.y = ScaleCenterPos.y - CenterToScaleCenterVec.y;

	//�X�P�[����̃T�C�Y�v�Z
	Vector2 ChangedSize;
	ChangedSize.x = (int)(Size.x * ScaleRate.x);
	ChangedSize.y = (int)(Size.y * ScaleRate.y);

	//�`��
	int DrawResult = DrawRect2D(ChangedCenterPos, ChangedSize, GraphHandle);
	return DrawResult;
}