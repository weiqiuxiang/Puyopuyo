#include "NextPuyo.h"
#include "DrawMethod.h"
#include "DxLib.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define WINDOW_POSX (SCREEN_WIDTH * 0.402f)
#define WINDOW_POSY (SCREEN_HEIGHT*0.05f + 24)
#define WINDOW_SIZE_X (192)
#define WINDOW_SIZE_Y (240)

#define NEXT_PUYO_1_POSX (WINDOW_POSX + WINDOW_SIZE_X * 0.5f)
#define NEXT_PUYO_1_POSY (WINDOW_POSY + WINDOW_SIZE_Y * 0.5f)
#define NEXT_PUYO_2_POSX (WINDOW_POSX + WINDOW_SIZE_X * 0.5f)
#define NEXT_PUYO_2_POSY (WINDOW_POSY + WINDOW_SIZE_Y * 1.5f)

#define PUYO_TEX_PASS "data/texture/PuyoColor.png"
#define NEXT_WINDOW_TEX_PASS "data/texture/NextWindow.png"

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
NextPuyo::NextPuyo() 
{
	for(int i = 0; i < NEXT_STORAGE; i++)
	{
		m_NextPuyoMtx[i][0] = 0;
		m_NextPuyoMtx[i][1] = 0;
	}
	
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		m_PuyoImageHandle[i] = -1;
	}

	m_WindowImageHandle = -1;
}

/*==========================================================================================
	�f�X�g���N�^
==========================================================================================*/
NextPuyo::~NextPuyo() 
{

}

/*==========================================================================================
	����������
==========================================================================================*/
bool NextPuyo::Init(void)
{
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		m_NextPuyoMtx[i][0] = GetRand(PUYO_TYPE_MAX - 1) + 2;
		m_NextPuyoMtx[i][1] = GetRand(PUYO_TYPE_MAX - 1) + 2;
	}

	//�ǂݍ��񂾃e�N�X�`���n���h���폜
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_PuyoImageHandle[i]);
	}
	DeleteGraph(m_WindowImageHandle);

	//�e�N�X�`���̓ǂݍ���
	int TexGetResult = LoadDivGraph(PUYO_TEX_PASS, 5, 3, 2, 128, 128, m_PuyoImageHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "�Ղ�̃e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		return false;
	}

	m_WindowImageHandle = LoadGraph(NEXT_WINDOW_TEX_PASS);
	if (m_WindowImageHandle == -1) 
	{
		MessageBox(nullptr, "�e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		return false;
	}

	return true;
}


/*==========================================================================================
	�`�揈��
==========================================================================================*/
void NextPuyo::Draw(void)
{
	//�l�N�X�g�E�B���h�E�̕`��
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		DrawMethod::DrawRect2D(
		{ (int)WINDOW_POSX ,(int)WINDOW_POSY + i * WINDOW_SIZE_Y },
		{ WINDOW_SIZE_X ,WINDOW_SIZE_Y },
			m_WindowImageHandle
		);
	}

	//�l�N�X�g�Ղ�1�̕`��
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		DrawMethod::DrawRect2DCenter(
		{ (int)NEXT_PUYO_1_POSX ,(int)NEXT_PUYO_1_POSY + i * PUYO_BLOCK_SIZE },
		{ PUYO_BLOCK_SIZE ,PUYO_BLOCK_SIZE },
			m_PuyoImageHandle[m_NextPuyoMtx[0][i] - 2]
		);
	}

	//�l�N�X�g�Ղ�2�̕`��
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		DrawMethod::DrawRect2DCenter(
		{ (int)NEXT_PUYO_2_POSX ,(int)NEXT_PUYO_2_POSY + i * PUYO_BLOCK_SIZE },
		{ PUYO_BLOCK_SIZE ,PUYO_BLOCK_SIZE },
			m_PuyoImageHandle[m_NextPuyoMtx[1][i] - 2]
		);
	}
}

/*==========================================================================================
	�I������
==========================================================================================*/
void NextPuyo::Uninit(void)
{
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_PuyoImageHandle[i]);
		m_PuyoImageHandle[i] = -1;
	}

	DeleteGraph(m_WindowImageHandle);
	m_WindowImageHandle = -1;
}

/*==========================================================================================
	���̂Ղ�̎擾
==========================================================================================*/
void NextPuyo::GetNextPuyo(int pOut[PUYO_NUM])
{
	//���̂Ղ����
	pOut[0] = m_NextPuyoMtx[0][0];
	pOut[1] = m_NextPuyoMtx[0][1];

	//�s�񂸂炷
	m_NextPuyoMtx[0][0] = m_NextPuyoMtx[1][0];
	m_NextPuyoMtx[0][1] = m_NextPuyoMtx[1][1];

	//���̂Ղ�擾
	m_NextPuyoMtx[1][0] = GetRand(PUYO_TYPE_MAX - 1) + 2;
	m_NextPuyoMtx[1][1] = GetRand(PUYO_TYPE_MAX - 1) + 2;
}