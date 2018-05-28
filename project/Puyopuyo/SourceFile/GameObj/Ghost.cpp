/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Ghost.h"
#include "Collision.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "DataExchange.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define GHOST_TEX_PASS "data/texture/Ghost.png"		//�Ղ�S�[�X�g�̃e�N�X�`���t�@�C���p�X

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
Ghost::Ghost() 
{
	for (int i = 0; i < PUYO_TYPE_MAX; i++) 
	{
		m_GraphHandle[i] = -1;
	}

	//�X�e�[�W��ʒu������
	m_PosInMap[0] = { -10,-10 };
	m_PosInMap[1] = { -10,-10 };
	m_GuildValue[0] = 0;
	m_GuildValue[1] = 0;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
Ghost::~Ghost() 
{
	
}

/*==========================================================================================
	����������
==========================================================================================*/
bool Ghost::Init(void)
{
	//�擾�����e�N�X�`���j��
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//�e�N�X�`���擾
	int TexGetResult = LoadDivGraph(GHOST_TEX_PASS, 5, 3, 2, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "�Ղ�S�[�X�g�̃e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		ErrorLogFmtAdd("[Ghost.cpp][Function:Init],�n���h��m_GraphHandle,�e�N�X�`���ǂݍ��ݎ��s");
		return false;
	}

	//�X�e�[�W��ʒu������
	m_PosInMap[0] = { -10,-10 };
	m_PosInMap[1] = { -10,-10 };
	m_GuildValue[0] = 0;
	m_GuildValue[1] = 0;

	return true;
}

/*==========================================================================================
	�X�e�[�W��ʒu�̌v�Z
==========================================================================================*/
void Ghost::CalcPosInMap(const Stage* pStage,const Puyo* pPuyo)
{
	//�k���`�F�b�N
	if (pPuyo == nullptr)
	{
		ErrorLogFmtAdd("[Ghost.cpp][Function:CalcPosInMap],����pPuyo�̓k��");
		return;
	}
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Ghost.cpp][Function:CalcPosInMap],����pStage�̓k��");
		return;
	}

	//�ʒu����
	Vector2 PosInMap = pPuyo->GetPuyoPosInMapInt();		//�}�b�v����W�擾
	m_PosInMap[0] = { PosInMap.x + 1 , PosInMap.y + 1};	//��]���S���W�v�Z
	m_GuildValue[0] = pPuyo->GetPuyoMtxValue(1,1);		//��]���S���l�̑��

	//��]�����̈ʒu����
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			if (ColumnNum == 1 && RowNum == 1) 
			{
				continue;
			}

			if (pPuyo->GetPuyoMtxValue(RowNum, ColumnNum) > 1) 
			{
				m_PosInMap[1] = { PosInMap.x + ColumnNum , PosInMap.y + RowNum };
				m_GuildValue[1] = pPuyo->GetPuyoMtxValue(RowNum, ColumnNum);
			}
		}
	}

	bool Up = m_PosInMap[1].y < m_PosInMap[0].y;		//��]���̍��W�͉�]���S�̏�
	bool Down = m_PosInMap[1].y > m_PosInMap[0].y;		//��]���̍��W�͉�]���S�̉�

	//�K�C�h�̍��W�v�Z
	for (int Guild = 0; Guild < 2; Guild++) 
	{
		for (int Y = 0; Y < MAP_BLOCK_NUM_VERTICAL + 2; Y++) 
		{
			int GoldY = m_PosInMap[Guild].y + Y;
			if (GoldY < 0) 
			{
				continue;
			}
			if (pStage->GetStageMtxValue(GoldY, m_PosInMap[Guild].x) >= 1)
			{
				m_PosInMap[Guild].y = GoldY - 1;
				break;
			}
		}
	}

	//�ʒu���炷
	if (Up == true) 
	{
		m_PosInMap[1].y--;
	}

	if (Down == true)
	{
		m_PosInMap[0].y--;
	}
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void Ghost::Draw(const Vector2& MapPos)
{
	//�`��
	for (int Guild = 0; Guild < 2; Guild++)
	{
		//�ʒu�v�Z
		Vector2 Pos;
		Pos.x = MapPos.x + m_PosInMap[Guild].x * PUYO_BLOCK_SIZE;
		Pos.y = MapPos.y + m_PosInMap[Guild].y * PUYO_BLOCK_SIZE;

		//�`��
		DrawMethod::DrawRect2DCenter(
			Pos,
			{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
			m_GraphHandle[m_GuildValue[Guild] - 2]
		);
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void Ghost::Uninit(void)
{
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
		m_GraphHandle[i] = -1;
	}
}

