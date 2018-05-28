/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Puyo.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "Stage.h"
#include <math.h>

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define PUYO_TEX_PASS "data/texture/PuyoColor.png"		//�Ղ�̃e�N�X�`���t�@�C���p�X
#define ALPHA_CHANGE_VALUE (5)							//�A���t�@�̕ω��l
#define PI (3.14159265359f)								//�~����

#define ROT_CENTER_ALPHA_MIN (50)						//��]���S�̃A���t�@�ŏ��l
#define ROT_CENTER_ALPHA_MAX (180)						//��]���S�̃A���t�@�ő�l
#define ROT_SPEED (0.05f * PI)							//��]���x

#define SCALE_SMALLER_TIME (20)							//�X�P�[���g��ɂ�����t���[����
#define SCALE_BIGGER_TIME (20)							//�X�P�[���k���ɂ�����t���[����
#define BIGGER_SPEED_Y (0.02f)							//�X�P�[�����̊g�呬�x
#define SMALLER_SPEED_Y (0.02f)							//�X�P�[�����̏k�����x
#define RETRUN_SPEED_Y (0.02f)							//�X�P�[�����̌��T�C�Y�ɖ߂鑬�x

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
Puyo::Puyo()
{
	//�����_���łՂ�̃^�C�v������
	int Up = GetRand(PUYO_TYPE_MAX - 1);
	int Center = GetRand(PUYO_TYPE_MAX - 1);

	//�Ղ�s��̏�����
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			m_Mtx[RowNum][ColumnNum] = 0;
		}
	}

	//��]����������
	m_RotPuyoPos = { 1.0f,  0.0f };
	m_RotGoalPos = m_RotPuyoPos;
	m_RotDir = false;
	m_RotFlag = false;

	//�X�e�[�W��ʒu������
	m_PosInMap = { MAP_BLOCK_NUM_HORIZON / 2 - 1, - 2};

	//�e�N�X�`���n���h��������
	for (int i = 0; i < PUYO_TYPE_MAX + 1; i++)
	{
		m_GraphHandle[i] = -1;
	}

	//�A���t�@�l������
	m_RotCenterAlpha = 0;
	m_AlphaState = false;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
Puyo::~Puyo()
{
}

/*==========================================================================================
	����������
==========================================================================================*/
bool Puyo::Init(void)
{
	//�����_���łՂ�̃^�C�v������
	int Up = GetRand(PUYO_TYPE_MAX - 1);
	int Center = GetRand(PUYO_TYPE_MAX - 1);

	//�Ղ�s��̏�����
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			m_Mtx[RowNum][ColumnNum] = 0;
		}
	}

	//��]����������
	m_RotPuyoPos = { 1.0f,  0.0f };
	m_RotGoalPos = m_RotPuyoPos;
	m_RotDir = false;
	m_RotFlag = false;

	//�X�e�[�W��ʒu������
	m_PosInMap = { MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 };

	//�A���t�@�l������
	m_RotCenterAlpha = 0;
	m_AlphaState = false;

	return true;
}

/*==========================================================================================
	�e�N�X�`���擾
==========================================================================================*/
int Puyo::LoadTexture(const char* TexPass)
{
	//�ǂݍ��񂾃e�N�X�`�����폜
	for (int i = 0; i < PUYO_TYPE_MAX + 1;i++) 
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//�e�N�X�`���擾
	int TexGetResult = LoadDivGraph(TexPass, 6, 3, 2, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "�Ղ�̃e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		return -1;
	}

	return 0;
}

/*==========================================================================================
	�A���t�@�����̍X�V
==========================================================================================*/
void Puyo::UpdateAlpha(void)
{
	if (m_AlphaState == false) 
	{
		m_RotCenterAlpha += ALPHA_CHANGE_VALUE;
		if (m_RotCenterAlpha >= ROT_CENTER_ALPHA_MAX)
		{
			m_RotCenterAlpha = ROT_CENTER_ALPHA_MAX;
			m_AlphaState = true;
		}
	}

	else 
	{
		m_RotCenterAlpha -= ALPHA_CHANGE_VALUE;
		if (m_RotCenterAlpha <= 0)
		{
			m_RotCenterAlpha = 0;
			m_AlphaState = false;
		}
	}
}

/*==========================================================================================
	�`�揈��
	��������:
		MapPos:�X�e�[�W�̈ʒu
==========================================================================================*/
void Puyo::Draw(const Vector2& MapPos)
{
	//�ʒu�v�Z
	Vector2 Pos;
	Pos.x = MapPos.x + (int)(m_PosInMap.x * PUYO_BLOCK_SIZE + PUYO_BLOCK_SIZE);
	Pos.y = MapPos.y + (int)(m_PosInMap.y * PUYO_BLOCK_SIZE + PUYO_BLOCK_SIZE);

	//��]���S�̕`��
	DrawMethod::DrawRect2DCenter(
		Pos,
		{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
		m_GraphHandle[m_Mtx[1][1] - 2]
	);

	//��]���S�_�Ō��ʕ`��
	SetDrawBlendMode(DX_BLENDMODE_ADD, m_RotCenterAlpha);	//�u�����h���[�h�I��
	DrawMethod::DrawRect2DCenter(
		Pos,
		{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
		m_GraphHandle[5]
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//�u�����h���[�h���I�t

	//��]���̕`��
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			if (m_Mtx[RowNum][ColumnNum] <= 1) 
			{
				continue;
			}
			if (ColumnNum == 1 && RowNum == 1) 
			{
				continue;
			}

			Pos.x = MapPos.x + (int)(m_PosInMap.x * PUYO_BLOCK_SIZE + m_RotPuyoPos.x * PUYO_BLOCK_SIZE);
			Pos.y = MapPos.y + (int)(m_PosInMap.y * PUYO_BLOCK_SIZE + m_RotPuyoPos.y * PUYO_BLOCK_SIZE);

			//��]���`��
			DrawMethod::DrawRect2DCenter(
				Pos,
				{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
				m_GraphHandle[m_Mtx[RowNum][ColumnNum] - 2]
			);

			//��]���̖ڕW���W�ɑ��
			m_RotGoalPos = { (float)ColumnNum , (float)RowNum };

			break;
		}
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void Puyo::Uninit(void)
{
	for (int i = 0; i < PUYO_BLOCK_SIZE; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}
}

/*==========================================================================================
	�Ղ�̍s��ɒl��ݒ�
==========================================================================================*/
void Puyo::SetPuyoMtxValue(int RowNum, int ColumnNum, int Value)
{
	if (RowNum < 0 || RowNum >= PUYO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Puyo.cpp][Function:SetPuyoMtxValue],����RowNum�͔͈͊O");
		return;
	}

	if (ColumnNum <0 || ColumnNum >= PUYO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Puyo.cpp][Function:SetPuyoMtxValue],����ColumnNum�͔͈͊O");
		return;
	}

	m_Mtx[RowNum][ColumnNum] = Value;
}

/*==========================================================================================
	�s��̗v�f�l��Ԃ�
		��������:
			RowNum:�s
			ColumnNum:��
==========================================================================================*/
int Puyo::GetPuyoMtxValue(int RowNum, int ColumnNum) const
{
	if (RowNum < 0 || RowNum >= PUYO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Puyo.cpp][Function:GetPuyoMtxValue],����RowNum�͔͈͊O");
		return -1;
	}

	if (ColumnNum < 0 || ColumnNum >= PUYO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Puyo.cpp][Function:GetPuyoMtxValue],����ColumnNum�͔͈͊O");
		return -1;
	}

	return m_Mtx[RowNum][ColumnNum];
}

/*==========================================================================================
	�����ړ�
==========================================================================================*/
void Puyo::MovePuyoPosInMapX(int MoveValue)
{
	m_PosInMap.x += MoveValue;
}

/*==========================================================================================
	�����ړ�
==========================================================================================*/
void Puyo::MovePuyoPosInMapY(float MoveValue)
{
	m_PosInMap.y += MoveValue;
}

/*==========================================================================================
	����]
==========================================================================================*/
void Puyo::LeftRotate(void)
{
	MatrixRorate(m_Mtx,false);
	m_RotDir = false;
	m_RotFlag = true;
}

/*==========================================================================================
	�E��]
==========================================================================================*/
void Puyo::RightRotate(void)
{
	MatrixRorate(m_Mtx, true);
	m_RotDir = true;
	m_RotFlag = true;
}

/*==========================================================================================
	��]�p�֐�
	��������:
		Mtx:�Ղ�s��
		ElementCount: �s��(��:�l��2�Ȃ�,��������s���2x2�^�C�v�B3�Ȃ�3x3�^�C�v)
		RotDir: �lfalse�̏ꍇ�͍���]�A�ltrue�̏ꍇ�͉E��]
==========================================================================================*/
void Puyo::MatrixRorate(int Mtx[PUYO_MATRIX_SIZE][PUYO_MATRIX_SIZE], bool RotDir)
{
	//�z��錾
	int MtxSave[PUYO_MATRIX_SIZE][PUYO_MATRIX_SIZE];

	//�Ղ�̉�]
	if (RotDir == false)	//����]
	{
		for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
			{
				MtxSave[PUYO_MATRIX_SIZE - 1 - ColumnNum][RowNum] = Mtx[RowNum][ColumnNum];
			}
		}
	}
	else
	{
		for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
			{
				MtxSave[ColumnNum][PUYO_MATRIX_SIZE - 1 - RowNum] = Mtx[RowNum][ColumnNum];
			}
		}
	}

	//��]���ʂ𔽉f
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			Mtx[RowNum][ColumnNum] = MtxSave[RowNum][ColumnNum];
		}
	}
}

/*==========================================================================================
	�Ղ��X�������ɒ��������Ă�
		�߂�l����:
			false: X�������������ĂȂ�
			true : X�������������Ă���
==========================================================================================*/
bool Puyo::PuyoAlignOnXAxis(void)
{
	if (m_Mtx[1][0] > 1) 
	{
		return true;
	}

	if (m_Mtx[1][2] > 1)
	{
		return true;
	}

	return false;
}

/*==========================================================================================
	�ڕW�Ɍ������ĉ�]
==========================================================================================*/
void Puyo::RotToGoal(void)
{
	//��]�s�\���
	if (m_RotFlag == false)
	{
		return;
	}

	//��]���S�ʒu
	Vector2f RotCenterPos;
	RotCenterPos.x = 1.0f;
	RotCenterPos.y = 1.0f;

	//��]���x���
	double RotSpeed;
	if (m_RotDir == false) 
	{
		RotSpeed = -ROT_SPEED;
	}
	else 
	{
		RotSpeed = ROT_SPEED;
	}

	//��]���S�ɉ����ĉ�]
	m_RotPuyoPos.x = (m_RotPuyoPos.x - RotCenterPos.x) * (float)cos(RotSpeed) - (m_RotPuyoPos.y - RotCenterPos.y) * (float)sin(RotSpeed) + RotCenterPos.x;
	m_RotPuyoPos.y = (m_RotPuyoPos.x - RotCenterPos.x) * (float)sin(RotSpeed) + (m_RotPuyoPos.y - RotCenterPos.y) * (float)cos(RotSpeed) + RotCenterPos.y;

	//��]�ڕW�n�_�ɓ�������
	Vector2f RotCenterToGoalVec;		//��]���S�����]�ڕW�n�_�܂ł̃x�N�g��
	RotCenterToGoalVec.x = m_RotGoalPos.x - RotCenterPos.x;
	RotCenterToGoalVec.y = m_RotGoalPos.y - RotCenterPos.y;

	Vector2f RotCenterToRotPuyo;		//��]���S�����]�Ղ�܂ł̃x�N�g��
	RotCenterToRotPuyo.x = m_RotPuyoPos.x - RotCenterPos.x;
	RotCenterToRotPuyo.y = m_RotPuyoPos.y - RotCenterPos.y;

	//�O�ςŕ�������
	float Cross2D = RotCenterToRotPuyo.x * RotCenterToGoalVec.y - RotCenterToRotPuyo.y * RotCenterToGoalVec.x;
	//����]�̏I������
	if (Cross2D > 0 && m_RotDir == false) 
	{
		m_RotPuyoPos = m_RotGoalPos;	//�S�[�����W����
		m_RotFlag = false;				//��]�I��
		return;
	}
	//�E��]�̏I������
	if (Cross2D < 0 && m_RotDir == true)
	{
		m_RotPuyoPos = m_RotGoalPos;	//�S�[�����W����
		m_RotFlag = false;				//��]�I��
		return;
	}
}