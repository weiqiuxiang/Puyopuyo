/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Score.h"
#include "DrawMethod.h"
#include "DxLib.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define DIGIT_NUM (8)			//����
#define	NUMBER_WIDTH (48)		//�����̕�
#define	NUMBER_HEIGHT (64)		//�����̍���
#define NUMBER_INTERVAL (48)	//�����̊Ԋu
#define DEFAULT_SCORE_POSX (SCREEN_WIDTH * 0.39f)	//�X�R�A�̃f�t�H���gX���W
#define DEFAULT_SCORE_POSY (SCREEN_HEIGHT * 0.9f)	//�X�R�A�̃f�t�H���gY���W
#define SCORE_TEX_PASS "data/texture/Score.png"		//�X�R�A�̃e�N�X�`���̃p�X

/*==========================================================================================
	const�ϐ�
==========================================================================================*/
const int ComboBonus[20] =		//�R���{�{�[�i�X
{
	0,0,8,16,32,64,96,128,160,192,224,256,288,320,352,384,416,448,480,512
};
const int ConnectBonus[12] =	//�A���{�[�i�X
{
	0,0,0,0,0,2,3,4,5,6,7,10
};
const int ColorBonus[6] =		//�F�{�[�i�X
{
	0,0,3,6,12,24
};

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
Score::Score()
{
	m_Score = 0;		//�X�R�A������
	m_DrawPos = { (int)DEFAULT_SCORE_POSX , (int)DEFAULT_SCORE_POSY };	//�`��ʒu�̏�����

	//�e�N�X�`���̓ǂݍ���
	for (int i = 0; i < SCORE_TEX_MAX; i++)
	{
		m_GraphHandle[i] = -1;
	}

	StartCalcScore();			//�v�Z�p�p�����[�^�����Z�b�g
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
Score::~Score()
{

}

/*==========================================================================================
	����������
==========================================================================================*/
bool Score::Init(void)
{
	m_Score = 0;		//�X�R�A������
	m_DrawPos = { (int)DEFAULT_SCORE_POSX , (int)DEFAULT_SCORE_POSY };	//�`��ʒu�̏�����

	for (int i = 0; i < SCORE_TEX_MAX; i++) 
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//�e�N�X�`���̓ǂݍ���
	int LoadTexResult = LoadDivGraph(SCORE_TEX_PASS,10,10,1,96,128, m_GraphHandle);
	if (LoadTexResult == -1) 
	{
		MessageBox(nullptr,"�X�R�A�̃e�N�X�`���ǂݍ��ݎ��s","error",MB_ICONHAND);
		return false;
	}

	StartCalcScore();			//�v�Z�p�p�����[�^�����Z�b�g

	return true;
}

/*==========================================================================================
	�`�揈��
==========================================================================================*/
void Score::Draw(void)
{
	//�X�R�A�`��
	int Score = m_Score;
	for (int i = 0; i < DIGIT_NUM; i++) 
	{
		//�l�擾
		int DrawNumValue = Score % 10;
		Score = Score / 10;

		//�ʒu�v�Z
		Vector2 Pos;
		Pos.x = m_DrawPos.x + (DIGIT_NUM - 1 - i) * NUMBER_INTERVAL;
		Pos.y = m_DrawPos.y;

		//�`��
		DrawMethod::DrawRect2D(Pos, { NUMBER_WIDTH ,NUMBER_HEIGHT }, m_GraphHandle[DrawNumValue]);
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void Score::Uninit(void)
{
	for (int i = 0; i < SCORE_TEX_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
		m_GraphHandle[i] = -1;
	}
}

/*==========================================================================================
	�X�R�A�̌v�Z�J�n(�v�Z�p�ϐ��̏�����)
==========================================================================================*/
void Score::StartCalcScore(void)
{
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		m_KillPuyoColor[i] = false;
	}

	m_Combo = 0;
	m_ConnectNum = 0;
	m_ScoreCalc = 0;
}

/*==========================================================================================
	�F���v�Z
==========================================================================================*/
void Score::CalcColorBonus(const int KillPuyo[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON])
{
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			if (KillPuyo[RowNum][ColumnNum] <= 1) 
			{
				continue;
			}

			m_KillPuyoColor[KillPuyo[RowNum][ColumnNum] - 2] = true;
		}
	}
}

/*==========================================================================================
	�R���{
==========================================================================================*/
void Score::AddCombo(void)
{
	m_Combo++;
}

/*==========================================================================================
	�X�R�A�v�Z
	��������:
		PuyoNum: �������Ղ搔
		ConnectNum: �A����
==========================================================================================*/
void Score::CalcScore(int PuyoNum)
{
	//�R���{�␳
	int Combo = max(m_Combo,19);
	Combo = min(0, m_Combo);

	//�A�����␳
	m_ConnectNum = max(m_ConnectNum, 11);
	m_ConnectNum = min(0, m_ConnectNum);

	//�F��
	int ColorNum = 0;
	for (int i = 0; i < PUYO_TYPE_MAX; i++) 
	{
		if (m_KillPuyoColor[i] == true) 
		{
			ColorNum++;
		}
	}

	//�{�[�i�X�v�Z
	int TotalBonus = ComboBonus[Combo] + ConnectBonus[m_ConnectNum] + ColorBonus[ColorNum];
	if (TotalBonus == 0) 
	{
		TotalBonus = 1;
	}

	m_ScoreCalc = m_Score + PuyoNum * TotalBonus * 10;
}