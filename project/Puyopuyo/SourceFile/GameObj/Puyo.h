/*==========================================================================================
�w�b�_�t�@�C������:
	�v���[���[������ł���Ղ�̃w�b�_�[
==========================================================================================*/
#ifndef PUYO_H_
#define PUYO_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define PUYO_MATRIX_SIZE (3)	//�Ղ�̍s��̃T�C�Y
#define PUYO_TYPE_MAX (5)		//�Ղ�̃^�C�v��
#define PUYO_BLOCK_SIZE (48)	//�Ղ�̃T�C�Y

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class Puyo 
{
public:
	Puyo();
	~Puyo();

	bool Init(void);						//����������
	int LoadTexture(const char*);			//�e�N�X�`���擾
	void UpdateAlpha(void);					//�A���t�@�̕����̍X�V
	void Draw(const Vector2& MapPos);		//�`�揈��(�����ŃX�e�[�W�ʒu��m��K�v������)
	void Uninit(void);						//�I������

	//�Z�b�^�[
	void SetPuyoMtxValue(int RowNum, int ColumnNum,int Value);		//�Ղ�̍s��ɒl��ݒ�
	void SetPuyoPosInMap(const Vector2f& pos) { m_PosInMap = pos; }	//�X�e�[�W��̍��W�ݒ�
	void SetPuyoPosXInMap(float PosX) { m_PosInMap.x = PosX; }
	void SetPuyoPosYInMap(float PosY) { m_PosInMap.y = PosY; }

	//�Q�b�^�[
	Vector2 GetPuyoPosInMapInt(void) const { return{ (int)m_PosInMap.x,(int)m_PosInMap.y }; }	//�Ղ�̃X�e�[�W��̈ʒu(int)
	Vector2f GetPuyoPosInMapFloat(void) const { return m_PosInMap; }		//�Ղ�̃X�e�[�W��̈ʒu(float)
	int GetPuyoMtxValue (int RowNum, int ColumnNum) const;		//�s��̗v�f�l��Ԃ�
	bool GetRotFlag(void) { return m_RotFlag; }					//��]�t���O�擾

	//���̊֐�
	void MovePuyoPosInMapX(int MoveValue);		//�����ړ�
	void MovePuyoPosInMapY(float MoveValue);	//�����ړ�
	void LeftRotate(void);			//����]
	void RightRotate(void);			//�E��]
	void MatrixRorate(int Mtx[PUYO_MATRIX_SIZE][PUYO_MATRIX_SIZE], bool RotDir);	//��]�p�֐�
	bool PuyoAlignOnXAxis(void);								//�Ղ��X�������ɒ��������Ă�
	void RotToGoal(void);										//�ڕW�Ɍ������ĉ�]

private:
	Vector2f	m_PosInMap;										//�X�e�[�W��̈ʒu
	int			m_Mtx[PUYO_MATRIX_SIZE][PUYO_MATRIX_SIZE];		//�Ղ�̍s��
	int			m_GraphHandle[PUYO_TYPE_MAX + 1];				//�O���t�B�b�N�n���h��
	int			m_RotCenterAlpha;								//��]���S�̃A���t�@�l
	bool		m_AlphaState;									//�A���t�@���(false�͉��Z�Atrue�͌��Z)

	Vector2f	m_RotPuyoPos;									//��]���̍��W
	Vector2f	m_RotGoalPos;									//��]���̖ڕW���W
	bool		m_RotDir;										//��]����
	bool		m_RotFlag;										//��]�t���O
};

#endif