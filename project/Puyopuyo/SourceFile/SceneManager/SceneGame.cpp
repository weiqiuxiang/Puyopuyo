/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "SceneGame.h"
#include "Input.h"
#include "Common.h"
#include "DxLib.h"
#include "Collision.h"
#include "DataExchange.h"
#include "Gui.h"
#include "DrawMethod.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define DOWN_INTPUT_SPEED (0.5f)		//���͗������x
#define DOWN_NATIVE_SPEED (0.02f)		//���R�������x
#define DROP_DELAY_TIME (10)

#define EFFECT_TIME (20)		//�G�t�F�N�g�p������(�t���[����)
#define PUYO_TEX_PASS "data/texture/PuyoColor.png"		//�Ղ�̃e�N�X�`���t�@�C���p�X

#define BUTTON_SIZEX (256)				//�{�^���̉���
#define BUTTON_SIZEY (128)				//�{�^���̏c��
#define BUTTON_TEX_PASS "data/texture/SceneGameButton.png"		//�{�^���̃t�@�C���p�X
#define GAME_OVER_MARK_TEX_PASS "data/texture/GameOverMark.png"	//�Q�[���I�[�o�[�}�[�N�̃t�@�C���p�X

/*==========================================================================================
	const�ϐ�
==========================================================================================*/
const float SPEED_RATE[5] = 
{
	1.0f,1.1f,1.2f,1.3f,1.4f
};

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
SceneGame::SceneGame()
{
	m_Step = GAME_CONTROL_PUYO;		//�ŏ��͂Ղ�R���g���[���i�K�̈ڍs
	m_PauseFlag = false;
	m_NextStep = GAME_CREATE_PUYO;
	m_DelayFrame = DROP_DELAY_TIME;

	//�n���h���̏�����
	for (int i = 0; i < 4; i++) 
	{
		m_ButtonHandle[i] = -1;
	}
	m_GameOverHandle = -1;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
SceneGame::~SceneGame()
{
	
}

/*==========================================================================================
	����������
==========================================================================================*/
bool SceneGame::Init(void)
{
	//�����o�ϐ�������
	m_Step = GAME_CONTROL_PUYO;		//�ŏ��͂Ղ�R���g���[���i�K�̈ڍs
	m_PauseFlag = false;			//�|�[�Y�t���O������
	m_NextStep = GAME_CREATE_PUYO;
	m_DelayFrame = DROP_DELAY_TIME;

	//���͑��u������
	if (Input::Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]Input�̏��������s");
		return false;
	}

	//�X�e�[�W������
	if (m_Stage.Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Stage�̏��������s");
		return false;
	}

	//�X�R�A������
	if (m_Score.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Score�̏��������s");
		return false;
	}

	//�l�N�X�g������
	if (m_NextPuyo.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_NextPuyo�̏��������s");
		return false;
	}

	//�Ղ揉����
	if (m_Puyo.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Puyo�̏��������s");
		return false;
	}
	else 
	{
		//�ŏ��̂Ղ�擾
		m_Puyo.SetPuyoPosInMap({ MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 });	//�����ʒu�ݒ�

		//�l�N�X�g����Ղ���擾
		int NextPuyo[2];
		m_NextPuyo.GetNextPuyo(NextPuyo);
		m_Puyo.SetPuyoMtxValue(0, 1, NextPuyo[0]);
		m_Puyo.SetPuyoMtxValue(1, 1, NextPuyo[1]);

		//�Ղ�̃e�N�X�`���̓ǂݍ���
		if (m_Puyo.LoadTexture(PUYO_TEX_PASS) == -1)
		{
			ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Puyo�̃e�N�X�`���ǂݍ��ݎ��s");
			return false;
		}
	}

	//�Ղ�����G�t�F�N�g�̏�����
	if (m_KillPuyoEF.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_KillPuyoEF�̏��������s");
		return false;
	}

	//�K�C�h������
	if (m_Ghost.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Ghost�̏��������s");
		return false;
	}

	//�Q�[���I�[�o�[���o������
	if (m_GameOverDirection.Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_GameOverDirection�̏��������s");
		return false;
	}

	//�w���v������
	if (m_Help.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Help�̏��������s");
		return false;
	}

	//�p�[�e�B�N�����[�J�[������
	if (m_ParticleMaker.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_ParticleMaker�̏��������s");
		return false;
	}

	//�V�[���J�ڃ{�^���̃e�N�X�`���̓ǂݍ���
	DeleteGraph(m_ButtonHandle[0]);
	DeleteGraph(m_ButtonHandle[1]);
	int LoadTexResult = LoadDivGraph(BUTTON_TEX_PASS, 4, 2, 2, 256, 128, m_ButtonHandle);
	if (LoadTexResult == -1)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_ButtonHandle�̓ǂݍ��ݎ��s");
		return false;
	}

	//�Q�[���I�[�o�[�}�[�N�̃e�N�X�`���ǂݍ���
	m_GameOverHandle = LoadGraph(GAME_OVER_MARK_TEX_PASS);
	if (m_GameOverHandle == -1) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_GameOverHandle�̓ǂݍ��ݎ��s");
		return false;
	}
	
	return true;
}

/*==========================================================================================
	�X�V����
==========================================================================================*/
void SceneGame::Update(void)
{
	Input::Update();			//���͍X�V

	if (m_Step != GAME_OVER)
	{
		m_Help.Update();			//�w���v�X�V
		m_ParticleMaker.Update();	//�p�[�e�B�N�����[�J�[�X�V
		Pause();					//�|�[�Y����
	}

	switch (m_Step)
	{
	case GAME_CREATE_PUYO:
	{
		int NextPuyo[2];
		m_NextPuyo.GetNextPuyo(NextPuyo);

		//�Ղ�ɂ��l�N�X�g�̒l������
		m_Puyo.Init();
		m_Puyo.SetPuyoMtxValue(0, 1, NextPuyo[0]);
		m_Puyo.SetPuyoMtxValue(1, 1, NextPuyo[1]);

		m_Ghost.CalcPosInMap(&m_Stage, &m_Puyo);	//�K�C�h�̈ʒu�̌v�Z
		m_Step = GAME_CONTROL_PUYO;					//�Ղ�R���g���[���Ɉڍs
		m_DropDelayTime = DROP_DELAY_TIME;			//�����̒x�����ԃ��Z�b�g

		m_Score.StartCalcScore();					//�X�R�A�̃p�����[�^�����Z�b�g
		break;
	}
	case GAME_CONTROL_PUYO:
	{
		//�Q�[���I�[�o�[����
		if (IsGameOver() == 0)
		{
			m_GameOverDirection.SetUseFlag(true);		//�Q�[���I�[�o�[���o�J�n
			m_Step = GAME_OVER;							//�Q�[���I�[�o�[�i�K�Ɉڍs
			break;
		}

		//�Ղ�̉�]���S�_�ŃG�t�F�N�g�̃A���t�@�l�X�V
		m_Puyo.UpdateAlpha();

		//�Ղ�̃R���g���[�����X�e�[�W�Ƃ̓����蔻����s��
		int SetPuyo = ControlPuyoAndDoCollision();

		//�K�C�h�ʒu�̌v�Z
		m_Ghost.CalcPosInMap(&m_Stage,&m_Puyo);

		//�Ղ���X�e�[�W�ɒu�����蔭��
		if (SetPuyo == 0)
		{
			SetPuyoProcessing();	//�u�������̏���
		}

		break;
	}

	case GAME_DROP_PUYO:
	{
		bool DropPuyoExist = m_Stage.DropPuyo();		//�Ղ旎��

		//�����Ղ�͂Ȃ�
		if (DropPuyoExist == false)
		{
			m_Stage.DropDataWriteInMap();							//�������ʂ��X�e�[�W�ɔ��f
			bool MarkExist = m_Stage.CheckAllKillBlock(&m_Score);	//������Ղ���}�[�N

			//������Ղ摶�݂���
			if (MarkExist)
			{
				m_Step = GAME_KILL_PUYO;				//�����i�K�Ɉڍs
			}
			else
			{
				m_DelayFrame = 10;						//delay���Ԑݒ�
				m_NextStep = GAME_CREATE_PUYO;			//�Ղ搶���i�K�ɖ߂�
				m_Step = GAME_DELAY;					//delay���[�h�Ɉڍs
			}
		}

		break;
	}

	case GAME_KILL_PUYO:
	{
		bool KillResult = m_Stage.KillBlock(&m_Score);		//�Ղ������

		if (KillResult == true)
		{
			//���o������
			SetKillPuyoEffectMtx();					//�Ղ�����G�t�F�N�g�s���ݒ�
			m_KillPuyoEF.SetAlphaValue(255);		//�A���t�@�����l�ݒ�
			m_KillPuyoEF.SetEFCount(40);			//���o���Ԑݒ�
			m_Step = GAME_KILL_PUYO_EFFECT;			//�������o�Ɉڍs
			m_Score.AddCombo();						//�R���{
		}
		else 
		{
			m_DelayFrame = 10;						//delay���Ԑݒ�
			m_NextStep = GAME_CREATE_PUYO;			//�Ղ搶���i�K�ɖ߂�
			m_Step = GAME_DELAY;					//delay���[�h�Ɉڍs
		}
		break;
	}

	case GAME_KILL_PUYO_EFFECT:
	{
		m_KillPuyoEF.Update(-5);
		int EfCount = m_KillPuyoEF.GetEFCount();
		if (EfCount <= 0)
		{
			m_Stage.CheckDropPuyo();			//�����Ղ���}�[�N
			m_Step = GAME_DROP_PUYO;			//�����i�K�Ɉڍs
			m_Score.ApplyCalcScore();			//�v�Z�����X�R�A�𔽉f������
		}
		break;
	}

	case GAME_PAUSE:
		GamePauseUI();							//�Q�[���|�[�YUI�̕\��
		break;

	case GAME_OVER:
		m_GameOverDirection.Update();

		//���o�I�����Ă��Ȃ����
		if (m_GameOverDirection.GetStep() != GAME_OVER_DIRECTION_OVER) 
		{
			m_PauseFlag = false;
			break;
		}

		GameOverUI();							//�Q�[���I�[�o�[��UI��\��
		break;

	case GAME_DELAY:
		m_DelayFrame--;
		if (m_DelayFrame < 0) 
		{
			m_DelayFrame = 0;
			m_Step = m_NextStep;
		}
		break;
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void SceneGame::Draw(void)
{
	m_Stage.Draw();					//�X�e�[�W�̕`��
	DrawGameOverMark();				//�Q�[���I�[�o�[�}�[�N�̕`��

	if (m_Step == GAME_CONTROL_PUYO)
	{
		m_Ghost.Draw(m_Stage.GetPos());	//�K�C�h�̕`��
		m_Puyo.Draw(m_Stage.GetPos());	//�Ղ�̕`��
	}

	if (m_Step == GAME_KILL_PUYO_EFFECT)
	{
		m_KillPuyoEF.Draw(m_Stage.GetPos());
	}
	m_NextPuyo.Draw();				//�l�N�X�g�̕`��
	m_Score.Draw();					//�X�R�A�̕`��
	m_Help.Draw();					//�w���v�`��

	//�Q�[���I�[�o�[���o
	m_GameOverDirection.Draw();

	//�p�[�e�B�N���`��
	m_ParticleMaker.Draw();
}

/*==========================================================================================
	�I������
==========================================================================================*/
void SceneGame::Uninit(void)
{
	m_Stage.Uninit();				//�X�e�[�W���
	m_Score.Uninit();				//�X�R�A���
	m_Puyo.Uninit();				//�X�R�A�̉��
	m_Ghost.Uninit();				//�K�C�h�̉��
	m_GameOverDirection.Uninit();	//�Q�[���I�[�o�[���o�̉��
	m_Help.Uninit();				//�w���v�����

	//�V�[���J�ڃ{�^���̃e�N�X�`���̉��
	for (int i = 0; i < 4; i++) 
	{
		DeleteGraph(m_ButtonHandle[i]);
		m_ButtonHandle[i] = -1;
	}
	DeleteGraph(m_GameOverHandle);
	m_GameOverHandle = -1;
}

/*==========================================================================================
	�Q�[���̃|�[�Y
==========================================================================================*/
void SceneGame::Pause(void)
{
	if(m_PauseFlag == false)
	{
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.7f),(int)(SCREEN_HEIGHT * 0.05) });
		Gui::SetButtonSize({ (int)(BUTTON_SIZEX * 0.4f),(int)(BUTTON_SIZEY * 0.4) });
		if (Gui::Button("MENU", m_ButtonHandle[0]))
		{
			m_PauseFlag = true;
			m_PauseSaveStep = m_Step;
			m_Step = GAME_PAUSE;
		}
	}
}

/*==========================================================================================
	���x���ɉ����ŗ������x��Ԃ�
==========================================================================================*/
float SceneGame::GetLevelDownSpeed(void)
{
	unsigned int Score = m_Score.GetScore();
	int Level = 0;

	if (Score > 5000)
	{
		Level = 1;
	}

	else if (Score > 20000)
	{
		Level = 2;
	}

	else if (Score > 50000)
	{
		Level = 3;
	}

	else if (Score > 100000)
	{
		Level = 4;
	}

	return DOWN_NATIVE_SPEED * SPEED_RATE[Level];
}

/*==========================================================================================
	�R�s�[���ړ�/��]�����A�����蔻������A�����蔻���̈ʒu�𒲐�����
		�߂�l�F
			-1: �X�e�[�W�ɂՂ��u�����蔭�����Ȃ�
			0 : �X�e�[�W�ɂՂ��u�����蔭������
==========================================================================================*/
int SceneGame::ControlPuyoAndDoCollision(void)
{
	int MoveValueX = 0;		//X���̈ړ���
	float MoveValueY = 0;	//Y���̈ړ���
	int RotDirValue = 0;	//��]�����l(����]�Ȃ�1�����A�E��]�Ȃ�1����)

	//�Ղ�̍��ړ�
	if (Input::GetTrigger(KEY_INPUT_A) || Input::GetRepeat(KEY_INPUT_A))
	{
		MoveValueX -= 1;
	}

	//�Ղ�̉E�ړ�
	if (Input::GetTrigger(KEY_INPUT_D) || Input::GetRepeat(KEY_INPUT_D))
	{
		MoveValueX += 1;
	}

	//�Ղ�̉��ړ�
	if (Input::GetPress(KEY_INPUT_S))		//���ړ�
	{
		MoveValueY += DOWN_INTPUT_SPEED;	//���ړ�
	}

	//�Ղ掞�Ԍo�߂ɂ�鉺�ړ�
	bool NativeDrop = false;
	if (MoveValueY == 0)
	{
		MoveValueY += GetLevelDownSpeed();	//���ړ����R����
		NativeDrop = true;
	}

	//��]����
	bool RotFlag = m_Puyo.GetRotFlag();
	if (RotFlag == false)
	{
		//�Ղ�̍���]
		if (Input::GetTrigger(KEY_INPUT_J))
		{
			RotDirValue -= 1;
		}

		//�Ղ�̉E��]
		if (Input::GetTrigger(KEY_INPUT_K))
		{
			RotDirValue += 1;
		}
	}

	else 
	{
		m_Puyo.RotToGoal();
	}

	//�Ղ�̃C���X�^���X�̃R�s�[�����
	Puyo PuyoCopyInstance = m_Puyo;	

	//���ړ��̓����蔻��
	if (MoveValueX != 0)
	{
		PuyoCopyInstance.MovePuyoPosInMapX(MoveValueX);	//�R�s�[�����ړ�����
		PuyoCopyInstance.MovePuyoPosInMapY(1);			//����̓u���b�N���݂���ꓖ���蔻�肪���邩

		//�R�s�[�ƃX�e�[�W�̓����蔻��
		int CollisionResult = Collision::CollisionStageAndPuyo(&m_Stage, &PuyoCopyInstance);

		//���ɖ߂�
		PuyoCopyInstance.MovePuyoPosInMapY(-1);

		//����Ȃ��Ȃ�
		if (CollisionResult == -1)
		{
			//�R�s�[�Ɠ����̉��ړ���
			m_Puyo.MovePuyoPosInMapX(MoveValueX);
		}
		else
		{
			//�R�s�[�����̈ʒu�ɖ߂�
			PuyoCopyInstance.MovePuyoPosInMapX(-MoveValueX);
		}
	}

	//�c�ړ��̓����蔻��
	if (MoveValueY != 0) 
	{
		//�R�s�[�𗎉�������(1�}�X�̉��Ƀu���b�N�����邩�A�\�����肷��)
		PuyoCopyInstance.MovePuyoPosInMapY(MoveValueY + 1);

		//�R�s�[�ƃX�e�[�W�̓����蔻��
		int CollisionResult = Collision::CollisionStageAndPuyo(&m_Stage, &PuyoCopyInstance);

		//�\������ʒu����߂̈ʒu�ɖ߂�
		PuyoCopyInstance.MovePuyoPosInMapY(-MoveValueY - 1);

		//�����蔻�蔭�����Ȃ�
		if (CollisionResult == -1) 
		{
			//�Ղ�c�ړ�
			m_Puyo.MovePuyoPosInMapY(MoveValueY);
		}
		else 
		{
			//�Ղ�̒u������
			if (NativeDrop == true)
			{
				m_DropDelayTime--;
				if (m_DropDelayTime < 0 && m_Puyo.GetRotFlag() == false)
				{
					m_Puyo.MovePuyoPosInMapY(1);		//�\��������̈ʒu�ɒu��
					return 0;
				}
			}
			else 
			{
				if (m_Puyo.GetRotFlag() == false)
				{
					m_Puyo.MovePuyoPosInMapY(1);		//�\��������̈ʒu�ɒu��
					return 0;
				}
			}
		}
	}

	//��]�̓����蔻��
	if (RotDirValue != 0) 
	{
		int RotResult = -1;

		//����]
		if (RotDirValue == -1) 
		{
			PuyoCopyInstance.LeftRotate();
			RotResult = Collision::MovePuyoPosAfterRot(&m_Stage, &PuyoCopyInstance, false);
		}
		else if (RotDirValue == 1) 
		{
			PuyoCopyInstance.RightRotate();
			RotResult = Collision::MovePuyoPosAfterRot(&m_Stage, &PuyoCopyInstance, true);
		}

		//��]���ʂœ����蔻�肷��
		if (RotResult == 0) //��]�ł���Ȃ�
		{
			//�R�s�[���Ղ�ɏ�������
			m_Puyo = PuyoCopyInstance;
		}
	}

	return -1;
}

/*==========================================================================================
	�Q�[���I�[�o�[�p�֐�
		�߂�l����:
			0 :�Q�[���I�[�o�[���肠��
			-1:�Q�[���I�[�o�[����Ȃ�
==========================================================================================*/
int SceneGame::IsGameOver(void)
{
	if (m_Stage.GetStageMtxValue(0, 3) >= 1) 
	{
		return 0;
	}

	if (m_Stage.GetStageMtxValue(0, 4) >= 1)
	{
		return 0;
	}

	return -1;
}

/*==========================================================================================
	�n�[�h�h���b�v�̃p�[�e�B�N�����
==========================================================================================*/
void SceneGame::MakeHardDropPaticle(void)
{
	//�p�[�e�B�N���̔����ʒu�v�Z
	Vector2 PuyoPosInMap = m_Ghost.GetRotCenterPosInMap();	//�Ղ�̃X�e�[�W����W�擾
	Vector2 MapPos = m_Stage.GetPos();						//�X�e�[�W�̉�ʍ���̍��W
	
	Vector2 ParticlePos;
	ParticlePos.x = MapPos.x + PuyoPosInMap.x * PUYO_BLOCK_SIZE + PUYO_BLOCK_SIZE;
	ParticlePos.y = MapPos.y + PuyoPosInMap.y * PUYO_BLOCK_SIZE + 2 * PUYO_BLOCK_SIZE;

	m_ParticleMaker.PuyoHardDropParticle(ParticlePos, {8,8},-5,0.05f);
}

/*==========================================================================================
	�G�t�F�N�g�̍s��ݒ�
==========================================================================================*/
bool SceneGame::SetKillPuyoEffectMtx(void) 
{
	bool MarkPuyoExist = false;

	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			if (m_Stage.GetKillMarkMtxValue(RowNum, ColumnNum) > 1)
			{
				MarkPuyoExist = true;
			}
			m_KillPuyoEF.SetEffectMtx(RowNum,ColumnNum,m_Stage.GetKillMarkMtxValue(RowNum, ColumnNum) );
		}
	}

	return MarkPuyoExist;
}

/*==========================================================================================
	�Ղ�Z�b�g��̏���
==========================================================================================*/
void SceneGame::SetPuyoProcessing(void)
{
	DataExchange::WritePuyoDataInStage(&m_Stage, &m_Puyo);			//�Ղ�̍s��f�[�^���X�e�[�W�ɏ�������
	m_Stage.CheckDropPuyo();		//�����Ղ���}�[�N
	m_Step = GAME_DROP_PUYO;		//�����i�K�Ɉڍs
}

/*==========================================================================================
	�Q�[���|�[�YUI
==========================================================================================*/
void SceneGame::GamePauseUI(void)
{
	//�Q�[���ɖ߂�{�^��
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.30) });
	Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
	if (Gui::Button("BACK", m_ButtonHandle[1]))
	{
		m_Step = m_PauseSaveStep;		//���̃X�e�b�v�ɖ߂�
		m_PauseFlag = false;			//�|�[�Y�t���O��false�ɖ߂�
	}

	//���^�C�A�{�^��
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.45) });
	Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
	if (Gui::Button("RETIRE", m_ButtonHandle[2]))
	{
		m_State = SCENE_GAME;
		m_SceneTransitionFlag = true;
	}

	//�^�C�g���J�ڃ{�^��
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.60) });
	Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
	if (Gui::Button("TITLE", m_ButtonHandle[3]))
	{
		m_State = SCENE_TITLE;
		m_SceneTransitionFlag = true;
	}
}

/*==========================================================================================
	�Q�[���I�[�o�[UI
==========================================================================================*/
void SceneGame::GameOverUI(void)
{
	//���^�C�A�{�^��
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.45) });
	Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
	if (Gui::Button("RETIRE", m_ButtonHandle[2]))
	{
		m_State = SCENE_GAME;
		m_SceneTransitionFlag = true;
	}

	//�^�C�g���J�ڃ{�^��
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.60) });
	Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
	if (Gui::Button("TITLE", m_ButtonHandle[3]))
	{
		m_State = SCENE_TITLE;
		m_SceneTransitionFlag = true;
	}
}

/*==========================================================================================
	�Q�[���I�[�o�[�}�[�N�`��
==========================================================================================*/
void SceneGame::DrawGameOverMark(void)
{
	//�X�e�[�W���W�擾
	Vector2 StagePos = m_Stage.GetPos();

	//�Q�[���I�[�o�[�}�[�N�`��
	for (int i = 3; i < 5; i++) 
	{
		Vector2 DrawPos;
		DrawPos.x = StagePos.x + i * PUYO_BLOCK_SIZE;
		DrawPos.y = StagePos.y + PUYO_BLOCK_SIZE;
		DrawMethod::DrawRect2DCenter(DrawPos, { PUYO_BLOCK_SIZE ,PUYO_BLOCK_SIZE },m_GameOverHandle);
	}
}