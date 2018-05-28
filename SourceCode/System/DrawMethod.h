/*==========================================================================================
�w�b�_�t�@�C������:
	�`��֐��̃N���X�̃w�b�_
==========================================================================================*/
#ifndef DRAW_METHOD_H_
#define DRAW_METHOD_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class DrawMethod 
{
public:
	//�l�p�`��
	static int DrawRect2D(const Vector2& pos,const Vector2& Size,const int& GraphHandle);
	static int DrawRect2DCenter(const Vector2& CenterPos, const Vector2& Size, const int& GraphHandle);
	static int DrawRect2DScale(const Vector2& CenterPos, const Vector2& ScaleCenterPos,const Vector2& Size,const Vector2f& ScaleRate,const int& GraphHandle);
};

#endif