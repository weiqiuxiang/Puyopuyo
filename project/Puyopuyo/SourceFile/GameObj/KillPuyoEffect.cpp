/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "KillPuyoEffect.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "Puyo.h"

/*==========================================================================================
	�}�N��
==========================================================================================*/
#define BLOCK_EF_TEX_FILEPASS "data/texture/KillPuyoEffect.png"

/*==========================================================================================
	�R���X�g���N�^
==========================================================================================*/
KillPuyoEffect::KillPuyoEffect()
{
	m_EffectCount = 0;		//�J�E���g��0�ɂ���
	m_AlphaValue = 0;		//�A���t�@�̒l��0�ɂ���

	//�G�t�F�N�g�z�񏉊���
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			m_EffectMtx[RowNum][ColumnNum] = 0;
		}
	}

	//�u���b�N�̃G�t�F�N�g�n���h��������
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		m_BlockEFHandle[i] = -1;
	}
}

/*==========================================================================================
	����������
==========================================================================================*/
bool KillPuyoEffect::Init(void)
{
	m_EffectCount = 0;		//�J�E���g��0�ɂ���
	m_AlphaValue = 0;		//�A���t�@�̒l��0�ɂ���

	//�G�t�F�N�g�z�񏉊���
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			m_EffectMtx[RowNum][ColumnNum] = 0;
		}
	}

	//�ǂݍ��񂾃e�N�X�`�����N���A
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_BlockEFHandle[i]);
	}

	//�e�N�X�`���ǂݍ���
	int TexGetResult = LoadDivGraph(BLOCK_EF_TEX_FILEPASS, 5, 3, 2, 128, 128, m_BlockEFHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "�Ղ�̃e�N�X�`���ǂݍ��ݎ��s", "WARNING", MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	���C�������G�t�F�N�g�`�揈��
		��������:
			MapPos: �X�e�[�W�̍�����W
			pKillPuyoNumber: �����郉�C���̍s�Ԃ��L�^���邽�߂̔z��A�h���X
			KillPuyoNum: �����郉�C���̐�
==========================================================================================*/
void KillPuyoEffect::Draw(const Vector2& MapPos)
{
	//�J�E���g��0�ȉ��Ȃ�`�悵�Ȃ�
	if (m_EffectCount <= 0)
	{
		return;
	}
	
	//���C���̃G�t�F�N�g�`��
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			if (m_EffectMtx[RowNum][ColumnNum] <= 1)
			{
				continue;
			}

			//�ʒu�v�Z
			Vector2 Pos;
			Pos.x = MapPos.x + ColumnNum * PUYO_BLOCK_SIZE;
			Pos.y = MapPos.y + RowNum * PUYO_BLOCK_SIZE;

			//�`��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_AlphaValue);		//�u�����h���[�h�I��
			int DrawResult = DrawMethod::DrawRect2DCenter(
				Pos,
				{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
				m_BlockEFHandle[m_EffectMtx[RowNum][ColumnNum] - 2]
			);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//�u�����h���[�h���I�t
		}
	}
}

/*==========================================================================================
	�X�V����
	��������:
		AlphaDecrementValue: �A���t�@�̕ω��l
==========================================================================================*/
void KillPuyoEffect::Update(int AlphaChangeValue)
{
	m_EffectCount = max(0, m_EffectCount - 1);
	m_AlphaValue = max(0, m_AlphaValue + AlphaChangeValue);
	m_AlphaValue = min(255, m_AlphaValue + AlphaChangeValue);
}

/*==========================================================================================
	�I������
==========================================================================================*/
void KillPuyoEffect::Uninit(void)
{
	//�ǂݍ��񂾃e�N�X�`�����N���A
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_BlockEFHandle[i]);
		m_BlockEFHandle[i] = -1;
	}
}

/*==========================================================================================
	�G�t�F�N�g�̍s����Z�b�g
==========================================================================================*/
void KillPuyoEffect::SetEffectMtx(int RowNum, int ColumnNum, int Value)
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[KillPuyoEffect.cpp][Function:SetEffectMtx],����RowNum�͔͈͊O");
		return;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[KillPuyoEffect.cpp][Function:SetEffectMtx],����ColumnNum�͔͈͊O");
		return;
	}

	m_EffectMtx[RowNum][ColumnNum] = Value;
}