/*==========================================================================================
�w�b�_�t�@�C������:
	�X�e�[�W�̑S�l��1�̍s�ɃG�t�F�N�g������
==========================================================================================*/
#ifndef KILL_PUYO_EFFECT_H_
#define KILL_PUYO_EFFECT_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"
#include "Stage.h"
#include "Puyo.h"

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class KillPuyoEffect 
{
public:
	KillPuyoEffect();
	~KillPuyoEffect() {}

	bool Init(void);				//����������
	void Uninit(void);				//�I������
	void Update(int);				//�X�V����
	void Draw(const Vector2& MapPos);	//���C�������G�t�F�N�g�`�揈��

	//�Q�b�^�[
	int GetEFCount(void) const { return m_EffectCount; }

	//�Z�b�^�[
	void SetEFCount(int Value) { m_EffectCount = Value; }
	void SetAlphaValue(int Value) { m_AlphaValue = Value; }
	void SetEffectMtx(int RowNum, int ColumnNum,int Value);		//�G�t�F�N�g�̍s����Z�b�g
private:
	int m_EffectMtx[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];
	int m_EffectCount;						//�G�t�F�N�g�̌p������
	int m_AlphaValue;						//�A���t�@�l
	int m_BlockEFHandle[PUYO_TYPE_MAX];		//�u���b�N�̃G�t�F�N�g�n���h��
};

#endif
