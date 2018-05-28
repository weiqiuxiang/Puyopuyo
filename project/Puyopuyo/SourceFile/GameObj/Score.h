/*==========================================================================================
�w�b�_�t�@�C������:
	�Q�[���̒��̃X�R�A�N���X
==========================================================================================*/
#ifndef SCORE_H_
#define SCORE_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"
#include "Puyo.h"
#include "Stage.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define SCORE_TEX_MAX (10)		//�X�R�A�̃e�N�X�`����
#define KILL_ONE_LINE (200)		//1���C�����������̓_��
#define KILL_TWO_LINE (800)		//2���C�����������̓_��
#define KILL_THREE_LINE (1800)	//3���C�����������̓_��
#define KILL_FOUR_LINE (2500)	//4���C�����������̓_��

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class Score 
{
public:
	Score();
	~Score();

	bool Init(void);	//����������
	void Draw(void);	//�`�揈��
	void Uninit(void);	//�I������

	void SetPos(const Vector2& pos) { m_DrawPos = pos; }		//�`��ʒu��ݒ�
	int GetScore(void) const { return m_Score; }				//�X�R�A

	void StartCalcScore(void);			//�X�R�A�̌v�Z�J�n(�v�Z�p�ϐ��̏�����)
	void CalcColorBonus(const int KillPuyo[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON]);	//�F���v�Z
	void AddCombo(void);	//�R���{+1
	void AddConnectNum(int Num) { m_ConnectNum += Num; }		//�A������ǉ�
	void CalcScore(int PuyoNum);							//�X�R�A�v�Z
	void ApplyCalcScore(void) {m_Score = m_ScoreCalc;}		//�v�Z�X�R�A�I�u�W�F�̒l���X�R�A�ɓK�p����
private:
	Vector2 m_DrawPos;				//�`��ʒu
	unsigned int m_Score;			//�X�R�A
	unsigned int m_ScoreCalc;		//�v�Z�X�R�A�I�u�W�F

	int		m_GraphHandle[SCORE_TEX_MAX];	//�O���t�B�b�N�n���h��
	int		m_Combo;						//�R���{��
	int		m_ConnectNum;					//�A����
	bool	m_KillPuyoColor[PUYO_TYPE_MAX];	//�������Ղ�̐F��
};

#endif