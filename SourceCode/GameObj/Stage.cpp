/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Stage.h"
#include "DrawMethod.h"
#include "DxLib.h"
#include "Puyo.h"
#include "Score.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define STAGE_DEFAULT_POSX (SCREEN_WIDTH*0.05f)			//�X�e�[�W�̈ʒu�̃f�t�H���gX�l
#define STAGE_DEFAULT_POSY (SCREEN_HEIGHT*0.05f)			//�X�e�[�W�̈ʒu�̃f�t�H���gY�l
#define PUYO_DROP_SPEED (0.1f)			//�Ղ�Ղ�̗������x

#define STAGE_TEX_PASS "data/texture/StageBlockColor.png"	//�Ղ�̃e�N�X�`���t�@�C���p�X
#define STAGEBG_TEX_PASS "data/texture/BG.png"				//�X�e�[�W�̃e�N�X�`���t�@�C���p�X

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
Stage::Stage()
{
	//�X�e�[�W������
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			bool MapLeftEnd = (ColumnNum == 0);							//�X�e�[�W���[
			bool MapRightEnd = (ColumnNum == MAP_BLOCK_NUM_HORIZON -1);	//�X�e�[�W�E�[
			bool MapBottom = (RowNum == MAP_BLOCK_NUM_VERTICAL-1);		//�X�e�[�W�̒�
			if(MapLeftEnd || MapRightEnd || MapBottom)
			{
				m_Map[RowNum][ColumnNum] = 1;			//�X�e�[�W�[�̒l
			}
			else 
			{
				m_Map[RowNum][ColumnNum] = 0;
			}

			//�Ղ�̈ʒu���
			m_MapBlockPos[RowNum][ColumnNum] = { (float)ColumnNum ,(float)RowNum };
		}
	}

	//���W������
	m_Pos = { (int)STAGE_DEFAULT_POSX ,(int)STAGE_DEFAULT_POSY };

	//�e�N�X�`���n���h��������
	for (int i = 0; i < MAP_TEX_MAX; i++)
	{
		m_GraphHandle[i] = -1;
	}
	m_HandleBG = -1;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
Stage::~Stage(){}

/*==========================================================================================
	����������
==========================================================================================*/
bool Stage::Init(void)
{
	//�X�e�[�W������
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			bool MapLeftEnd = (ColumnNum == 0);							//�X�e�[�W���[
			bool MapRightEnd = (ColumnNum == MAP_BLOCK_NUM_HORIZON - 1);	//�X�e�[�W�E�[
			bool MapBottom = (RowNum == MAP_BLOCK_NUM_VERTICAL - 1);		//�X�e�[�W�̒�
			if (MapLeftEnd || MapRightEnd || MapBottom)
			{
				m_Map[RowNum][ColumnNum] = 1;			//�X�e�[�W�[�̒l
			}
			else
			{
				m_Map[RowNum][ColumnNum] = 0;
			}

			//�Ղ�̈ʒu���
			m_MapBlockPos[RowNum][ColumnNum] = { (float)ColumnNum ,(float)RowNum };
		}
	}

	//���W������
	m_Pos = { (int)STAGE_DEFAULT_POSX ,(int)STAGE_DEFAULT_POSY };

	//�e�N�X�`�����
	for (int i = 0; i < MAP_TEX_MAX;i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//�e�N�X�`���擾
	int TexGetResult = LoadDivGraph(STAGE_TEX_PASS, 7, 3, 3, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "�X�e�[�W�̃e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		return false;
	}

	m_HandleBG = LoadGraph(STAGEBG_TEX_PASS);
	if (m_HandleBG == -1)
	{
		MessageBox(nullptr, "�X�e�[�W�̃e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void Stage::Draw(void)
{
	//�X�e�[�W�w�i�̕`��
	Vector2 BGSize;
	BGSize.x = (MAP_BLOCK_NUM_HORIZON - 1) * PUYO_BLOCK_SIZE;
	BGSize.y = (MAP_BLOCK_NUM_VERTICAL - 1) * PUYO_BLOCK_SIZE;

	DrawMethod::DrawRect2D(
	{ (int)STAGE_DEFAULT_POSX + 24,(int)STAGE_DEFAULT_POSY + 24 },BGSize,m_HandleBG);

	//�X�e�[�W�̕`��
	//0�s�ڂ͕`�悵�Ȃ�
	for (int RowNum = 1; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			//�ʒu�v�Z
			Vector2 Pos;
			Pos.x = m_Pos.x + (int)(m_MapBlockPos[RowNum][ColumnNum].x * PUYO_BLOCK_SIZE);
			Pos.y = m_Pos.y + (int)(m_MapBlockPos[RowNum][ColumnNum].y * PUYO_BLOCK_SIZE);

			//�`��
			DrawMethod::DrawRect2DCenter(
				Pos,
				{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
				m_GraphHandle[(int)m_Map[RowNum][ColumnNum]]
			);
		}
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void Stage::Uninit(void)
{
	for (int i = 0; i < MAP_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}
	DeleteGraph(m_HandleBG);
}

/*==========================================================================================
	�X�e�[�W�̍s��̒l��Ԃ�
		��������:
			RowNum:�s
			ColumnNum:��
			Value:�ݒ肷��l
==========================================================================================*/
void Stage::SetStageMtxValue(int RowNum, int ColumnNum, int Value)
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:SetStageMtxValue],����RowNum�͔͈͊O");
		return;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:SetStageMtxValue],����ColumnNum�͔͈͊O");
		return;
	}

	m_Map[RowNum][ColumnNum] = Value;
}

/*==========================================================================================
	�X�e�[�W�̍s��̒l��Ԃ�
	��������:
		RowNum:�s
		ColumnNum:��
==========================================================================================*/
int	Stage::GetStageMtxValue(int RowNum, int ColumnNum) const
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetStageMtxValue],����RowNum�͔͈͊O");
		return -1;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetStageMtxValue],����ColumnNum�͔͈͊O");
		return -1;
	}

	return m_Map[RowNum][ColumnNum];
}

/*==========================================================================================
	�X�e�[�W�̍s��̒l��Ԃ�
	��������:
		RowNum:�s
		ColumnNum:��
==========================================================================================*/
int Stage::GetKillMarkMtxValue(int RowNum, int ColumnNum) const
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetKillMarkMtxValue],����RowNum�͔͈͊O");
		return false;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetKillMarkMtxValue],����ColumnNum�͔͈͊O");
		return false;
	}

	return m_KillBlockMarkMtx[RowNum][ColumnNum];
}

/*==========================================================================================
	�����Ղ���}�[�N���A�����I����̃X�e�[�W�s������
==========================================================================================*/
void Stage::CheckDropPuyo(void) 
{
	//�����猟��
	for (int RowNum = MAP_BLOCK_NUM_VERTICAL - 2; RowNum >= 0; RowNum--)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			m_MapBlockPos[RowNum][ColumnNum] = { (float)ColumnNum ,(float)RowNum };		//�ʒu������

			//�Ղ�ł͂Ȃ�
			if (m_Map[RowNum][ColumnNum] < 1)
			{
				m_MarkDropMtx[RowNum][ColumnNum].IsCanDrop = false;			//�����ł��Ȃ�
				m_DropOverMap[RowNum][ColumnNum] = m_Map[RowNum][ColumnNum];	//������̃X�e�[�W�ɏ�������
				continue;
			}

			//�����ʒu����
			int DistFromBottom = MAP_BLOCK_NUM_VERTICAL - 2 - RowNum;		//��Ƃ̋����𑪂�
			int BlockNum = 0;			//���̂Ղ悩���܂ł̊Ԃɑ��݂���Ղ搔
			for (int i = 1; i <= DistFromBottom; i++) 
			{
				//�Ղ悾
				if (m_Map[RowNum + i][ColumnNum] > 1) 
				{
					BlockNum++;
				}
			}
			m_MarkDropMtx[RowNum][ColumnNum].DropDist = DistFromBottom - BlockNum;	//���������v�Z
			if (m_MarkDropMtx[RowNum][ColumnNum].DropDist == 0)	//����������0
			{
				m_MarkDropMtx[RowNum][ColumnNum].IsCanDrop = false;
				m_DropOverMap[RowNum][ColumnNum] = m_Map[RowNum][ColumnNum];	//������̃X�e�[�W�s��ɏ�������
			}
			else
			{
				m_MarkDropMtx[RowNum][ColumnNum].IsCanDrop = true;
				int Goal = m_MarkDropMtx[RowNum][ColumnNum].DropDist + RowNum;
				m_DropOverMap[RowNum][ColumnNum] = 0;							//���̏��̃f�[�^������
				m_DropOverMap[Goal][ColumnNum] = m_Map[RowNum][ColumnNum];		//�S�[����𗎉���̃X�e�[�W�s��ɏ�������
			}
		}
	}
}

/*==========================================================================================
	�󒆂ɕ����Ă�Ղ�𗎉�������
		�߂�l:
			false :��������Ղ�͂Ȃ�
			true:�������Ղ悩����
==========================================================================================*/
bool Stage::DropPuyo(void)
{
	bool DropExist = false;		//��������Ղ摶�݂���

	//��������Ղ���������A��������(�������猟��)
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			//�����ł��Ȃ��������`�F�b�N
			if (m_MarkDropMtx[RowNum][ColumnNum].IsCanDrop == false)
			{
				continue;
			}

			//����
			int Goal = RowNum + m_MarkDropMtx[RowNum][ColumnNum].DropDist;
			m_MapBlockPos[RowNum][ColumnNum].y += PUYO_DROP_SPEED;

			//�����ڕW�ɓ���
			if (m_MapBlockPos[RowNum][ColumnNum].y > Goal)
			{
				m_MapBlockPos[RowNum][ColumnNum].y = (float)Goal;		//�ʒu���S�[���ɍ��킷
				m_MarkDropMtx[RowNum][ColumnNum].IsCanDrop = false;		//�����s�\�ɐݒ�
			}
			else 
			{
				DropExist = true;
			}
		}
	}

	return DropExist;
}

/*==========================================================================================
	�������ʂ��X�e�[�W�ɔ��f����
==========================================================================================*/
void Stage::DropDataWriteInMap(void)
{
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			m_Map[RowNum][ColumnNum] = m_DropOverMap[RowNum][ColumnNum];
			m_MapBlockPos[RowNum][ColumnNum] = { (float)ColumnNum ,(float)RowNum };
		}
	}
}

/*==========================================================================================
	�����\�肪����Ղ���}�[�N
	�߂�l����:
		true:  �����u���b�N�����݂���
		false: �����u���b�N�����݂��Ȃ�
==========================================================================================*/
bool Stage::CheckAllKillBlock(Score* pScore)
{
	if (pScore == nullptr) 
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:CheckAllKillBlock],����pScore�̓k��");
		return false;
	}

	//�����Ղ�̃t���O�s�񏉊���
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			m_KillBlockMarkMtx[RowNum][ColumnNum] = false;
		}
	}

	bool MarkPuyoExist = false;		//�����Ղ悪���݂���t���O

	//�����Ղ���}�[�N(�E�����猟��)
	for (int RowNum = MAP_BLOCK_NUM_VERTICAL - 2; RowNum >= 0; RowNum--)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			//�Ղ�ł͂Ȃ�
			if (m_Map[RowNum][ColumnNum] < 1) 
			{
				continue;
			}
			
			//�}�[�N�ς�
			if (m_KillBlockMarkMtx[RowNum][ColumnNum] > 1)
			{
				continue;
			}

			//�t���O��ۑ�����\���̂����
			int KillBlockCheck[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];
			for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
			{
				for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
				{
					KillBlockCheck[RowNum][ColumnNum] = 0;
				}
			}

			int KillCount = 0;		//�}�[�N��

			//�}�[�N�J�n(�ċA)
			RecursiveCheckKillBlock({ ColumnNum ,RowNum }, m_Map[RowNum][ColumnNum], &KillCount, KillBlockCheck);

			//�}�[�N����4�ȏ�
			if (KillCount >= 4) 
			{
				for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
				{
					for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
					{
						//�}�[�N������������
						if (KillBlockCheck[RowNum][ColumnNum] > 1) 
						{
							m_KillBlockMarkMtx[RowNum][ColumnNum] = KillBlockCheck[RowNum][ColumnNum];
						}
					}
				}

				MarkPuyoExist = true;			//�����Ղ悪���݂���
			}

			//�X�R�A�ɘA������ǉ�
			pScore->AddConnectNum(KillCount);
		}
	}

	return MarkPuyoExist;
}

/*==========================================================================================
	�����\�肪����Ղ���}�[�N
	��������:
		Position:�`�F�b�N����Ղ�̃X�e�[�W��̈ʒu
		BlockValue:�}�[�N����Ղ�̒l
		Dir:�������ꂽ����
		KillBlockCheck:�}�[�N�s��
==========================================================================================*/
void Stage::RecursiveCheckKillBlock(Vector2 Position, int BlockValue, int *pKillCount,int KillBlockCheck[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON])
{
	//�͈͊O�`�F�b�N
	if (Position.x < 0 || Position.x >= MAP_BLOCK_NUM_HORIZON)
	{
		return;
	}
	if (Position.y < 0 || Position.y >= MAP_BLOCK_NUM_VERTICAL)
	{
		return;
	}
	//�}�[�N�ς݂����O����
	if (KillBlockCheck[Position.y][Position.x] > 1) 
	{
		return;
	}

	//�}�[�N�Ώۂł͂Ȃ�
	if (m_Map[Position.y][Position.x] != BlockValue)
	{
		return;
	}

	KillBlockCheck[Position.y][Position.x] = BlockValue;	//�}�[�N
	*pKillCount = *pKillCount + 1;							//���C���N�������g

	//���Ɍ���
	RecursiveCheckKillBlock({ Position.x - 1,Position.y }, BlockValue, pKillCount, KillBlockCheck);
	//�E�Ɍ���
	RecursiveCheckKillBlock({ Position.x + 1,Position.y }, BlockValue, pKillCount, KillBlockCheck);
	//��Ɍ���
	RecursiveCheckKillBlock({ Position.x,Position.y - 1 }, BlockValue, pKillCount, KillBlockCheck);
	//���Ɍ���
	RecursiveCheckKillBlock({ Position.x,Position.y + 1 }, BlockValue, pKillCount, KillBlockCheck);
}

/*==========================================================================================
	�Ղ������
		�߂�l����:
			true:	�������Ղ搔��1�ȏ゠��
			false:  �������Ղ搔��0
==========================================================================================*/
bool Stage::KillBlock(Score* pScore)
{
	if (pScore == nullptr)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:KillBlock],����pScore�̓k��");
		return false;
	}

	int KillPuyoNum = 0;
	bool KillBlockExist = false;

	//�F���𐔂���
	pScore->CalcColorBonus(m_KillBlockMarkMtx);

	//�Ղ������
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			if (m_KillBlockMarkMtx[RowNum][ColumnNum] > 1)
			{
				m_Map[RowNum][ColumnNum] = 0;
				KillBlockExist = true;
				KillPuyoNum++;
			}
		}
	}

	pScore->CalcScore(KillPuyoNum);			//�X�R�A�v�Z

	return KillBlockExist;
}