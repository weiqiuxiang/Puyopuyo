/*==========================================================================================
�w�b�_�t�@�C������:
	�e�g���X�X�e�[�W�̃w�b�_�t�@�C��
==========================================================================================*/
#ifndef STAGE_H_
#define STAGE_H_

/*==========================================================================================
�C���N���[�h�w�b�_
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define MAP_BLOCK_NUM_HORIZON (8)		//���������X�e�[�W�̂Ղ搔
#define MAP_BLOCK_NUM_VERTICAL (13)		//���������X�e�[�W�̂Ղ搔
#define MAP_TEX_MAX (7)					//�X�e�[�W�̃e�N�X�`���̐�

/*==========================================================================================
	�\���̒�`
==========================================================================================*/
typedef struct 
{
	bool	IsCanDrop;			//�����ł���t���O(true�Ȃ炱�̂Ղ�͗����\)
	int		DropDist;			//��������(�P�ʂ͂Ղ�)
}DROP_TO_GOAL;

/*==========================================================================================
	����錾
==========================================================================================*/
class Score;

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class Stage 
{
public:
	Stage();
	~Stage();

	bool Init(void);			//����������
	void Draw(void);			 //�`�揈��
	void Uninit(void);			 //�I������

	//�Z�b�^�[
	void SetStageMtxValue(int RowNum, int ColumnNum, int Value);	//�X�e�[�W�̍s��̒l��ݒ�

	//�Q�b�^�[
	Vector2 GetPos(void) const {return m_Pos;}
	int	GetStageMtxValue(int RowNum, int ColumnNum) const;			//�X�e�[�W�̍s��̒l��Ԃ�
	int GetKillMarkMtxValue(int RowNum, int ColumnNum) const;		//������\�肪����Ղ�}�[�N�s��̒l��Ԃ�

	//���̊֐�
	void CheckDropPuyo(void);			//�����Ղ���}�[�N���A�����I����̃X�e�[�W�s������
	bool DropPuyo(void);				//�󒆂ɕ����Ă�Ղ�𗎉�������
	void DropDataWriteInMap(void);		//�������ʂ��X�e�[�W�ɔ��f����

	bool CheckAllKillBlock(Score*);		//�����\�肪����Ղ���}�[�N
	bool KillBlock(Score*);				//�u���b�N������
private:
	void RecursiveCheckKillBlock(Vector2 Position, int BlockValue, 
		int *pKillCount, int KillBlockCheck[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON]);	//�����\�肪����Ղ���}�[�N
private:
	Vector2			m_Pos;					//�X�e�[�W����̂Ղ�̒��S���W
	int				m_Map[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];				//�X�e�[�W�̃f�[�^
	Vector2f		m_MapBlockPos[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];		//�Ղ�̒������W

	DROP_TO_GOAL	m_MarkDropMtx[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];		//�Ղ�̗����}�[�N�s��
	int				m_DropOverMap[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];		//�����I����̃X�e�[�W�s��
	int				m_KillBlockMarkMtx[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];	//������\�肪����Ղ�}�[�N�s��(true�Ȃ�����Afalse�Ȃ�����Ȃ�)
	int				m_GraphHandle[MAP_TEX_MAX];			//�X�e�[�W�̃e�N�X�`���n���h��
	int				m_HandleBG;							//�w�i�̃e�N�X�`���n���h��
};

#endif