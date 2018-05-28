/*==========================================================================================
�w�b�_�t�@�C������:
	�Q�[���̃V�[��
==========================================================================================*/
#ifndef SCENEGAME_H_
#define SCENEGAME_H_

/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "SceneBase.h"
#include "Stage.h"
#include "Score.h"
#include "Puyo.h"
#include "Ghost.h"
#include "GameOverDirection.h"
#include "ControlHelp.h"
#include "ParticleMaker.h"
#include "KillPuyoEffect.h"
#include "NextPuyo.h"

/*==========================================================================================
	enum:Game�̎��s�X�e�b�v
==========================================================================================*/
typedef enum 
{
	GAME_CREATE_PUYO = 0,		//�Ղ搶��
	GAME_CONTROL_PUYO,			//�v���[���[�͂Ղ�𑀍�
	GAME_DROP_PUYO,				//�Ղ旎��
	GAME_KILL_PUYO,				//�Ղ������
	GAME_KILL_PUYO_EFFECT,		//�Ղ�������G�t�F�N�g
	GAME_PAUSE,					//�ꎞ��~
	GAME_OVER,					//�Q�[���I�[�o�[
	GAME_DELAY					//�i�s���ꎞ��~
}GAME_STEP;

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	virtual bool Init(void);		//����������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��
	virtual void Uninit(void);		//�I������

private:
	void Pause(void);						//�Q�[���̃|�[�Y	
	float GetLevelDownSpeed(void);			//���x���ɉ����ŗ������x��Ԃ�
	int ControlPuyoAndDoCollision(void);	//�Ղ���R�s�[���A���̃R�s�[���ړ�/��]������
	int IsGameOver(void);					//�Q�[���I�[�o�[�p�֐�
	void MakeHardDropPaticle(void);			//�n�[�h�h���b�v�̃p�[�e�B�N�����
	bool SetKillPuyoEffectMtx(void);		//�G�t�F�N�g�̍s��ݒ�

	void SetPuyoProcessing(void);			//�Ղ��u�����̏���
	void GamePauseUI(void);					//�Q�[���|�[�YUI
	void GameOverUI(void);					//�Q�[���I�[�o�[UI
	void DrawGameOverMark(void);			//�Q�[���I�[�o�[�}�[�N�`��
private:
	GAME_STEP		m_Step;				//�Q�[���̎��s�菇
	Stage			m_Stage;			//�X�e�[�W�����o�[�ϐ�
	Score			m_Score;			//�X�R�A
	Puyo			m_Puyo;				//�Ղ�
	KillPuyoEffect	m_KillPuyoEF;		//�Ղ�������G�t�F�N�g
	Ghost			m_Ghost;			//�����ʒu�̃K�C�h
	NextPuyo		m_NextPuyo;			//���̂Ղ�
	GameOverDirection	m_GameOverDirection;		//�Q�[���I�[�o�[���o
	int				m_ButtonHandle[4];				//�V�[���J�ڃ{�^���̃n���h��
	ControlHelp		m_Help;					//�w���v�摜
	ParticleMaker	m_ParticleMaker;		//�p�[�e�B�N�����[�J�[
	bool			m_PauseFlag;			//�|�[�Y�t���O
	GAME_STEP		m_PauseSaveStep;		//�|�[�Y���A�Q�[���̎��s�菇��ۑ�
	GAME_STEP		m_NextStep;				//delay�̌�Ɏ��ɍs���X�e�b�v
	int				m_DelayFrame;			//delay����t���[����
	int				m_DropDelayTime;		//���R������������delay����
	int				m_GameOverHandle;		//�Q�[���I�[�o�[�n���h��
};

#endif
