/*==========================================================================================
	�C���N���[�h�w�b�_
==========================================================================================*/
#include "Collision.h"
#include "DxLib.h"
#include "DataExchange.h"
#include "Puyo.h"
#include "Stage.h"

/*==========================================================================================
	��]��Ղ�̈ʒu����
		��������:
			pStage: �X�e�[�W�̃|�C���^
			pPuyo: �Ղ�̃|�C���^
			RotDir: ��]����(false�͍������Atrue�͉E����)
		�߂�l����:
			0 :��]�㏈���I��
			-1:��]�ł��Ȃ�
==========================================================================================*/
int Collision::MovePuyoPosAfterRot(const Stage* pStage, Puyo* pPuyo, bool RotDir)
{
	//�k���`�F�b�N
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MovePuyoPosAfterRotElement3],����pStage�̓k��");
		return -1;
	}
	if (pPuyo == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MovePuyoPosAfterRotElement3],����pPuyo�̓k��");
		return -1;
	}

	//�C���X�^���X�̃R�s�[�����
	Puyo PuyoInstance = *pPuyo;

	//�R�s�[�̍��W�������
	PuyoInstance.MovePuyoPosInMapX(-1);

	int HitResult[9];		//�����蔻�茋��

	//�Ղ��,����A��A�E��A���ԍ��A���ԁA���ԉE�A�����A���A�E���Ɉړ����鎞�̓����蔻����s��
	for (int RowNum = 0; RowNum < 3; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < 3; ColumnNum++)
		{
			int HitNum = ColumnNum + RowNum * 3;
			HitResult[HitNum] = CollisionStageAndPuyo(pStage, &PuyoInstance);
			PuyoInstance.MovePuyoPosInMapX(1);	//X�����E1���炷
		}
		PuyoInstance.MovePuyoPosInMapX(-3);	//X���̍��W��߂�
		PuyoInstance.MovePuyoPosInMapY(1);	//Y��1���ɐi��
	}

	//���̈ʒu�̂̓����蔻�茋��
	if (HitResult[4] == -1)
	{
		return 0;
	}

	//��A���A���A�E�Ɉړ�������̓����蔻�茋��
	if (HitResult[1] == -1)		//��
	{
		pPuyo->MovePuyoPosInMapY(-1);
		return 0;
	}
	if (HitResult[7] == -1)		//��
	{
		pPuyo->MovePuyoPosInMapY(-1);
		return 0;
	}
	if (HitResult[3] == -1)		//��
	{
		pPuyo->MovePuyoPosInMapX(-1);
		return 0;
	}
	if (HitResult[5] == -1)		//�E
	{
		pPuyo->MovePuyoPosInMapX(1);
		return 0;
	}

	//����A�E��A�����A�E���Ɉړ�������̓����蔻�茋��
	if (HitResult[0] == -1)		//����
	{
		pPuyo->MovePuyoPosInMapX(-1);
		pPuyo->MovePuyoPosInMapY(-1);
		return 0;
	}
	if (HitResult[2] == -1)		//�E��
	{
		pPuyo->MovePuyoPosInMapX(1);
		pPuyo->MovePuyoPosInMapY(-1);
		return 0;
	}
	if (HitResult[6] == -1)		//����
	{
		pPuyo->MovePuyoPosInMapX(-1);
		pPuyo->MovePuyoPosInMapY(1);
		return 0;
	}
	if (HitResult[8] == -1)		//�E��
	{
		pPuyo->MovePuyoPosInMapX(1);
		pPuyo->MovePuyoPosInMapY(1);
		return 0;
	}

	//X���꒼���ɕ��ԏꍇ
	bool AlignXAxis = pPuyo->PuyoAlignOnXAxis();
	if (AlignXAxis == true) 
	{
		//����]
		if (RotDir == false) 
		{
			pPuyo->LeftRotate();
		}
		else 
		{
			pPuyo->RightRotate();
		}

		//����Ƀu���b�N���݂��邩
		pPuyo->MovePuyoPosInMapY(1);
		if (CollisionStageAndPuyo(pStage, pPuyo) == -1)
		{
			//����u���b�N���݂��Ȃ��ꍇ�߂̈ʒu�ɖ߂�
			pPuyo->MovePuyoPosInMapY(-1);
			return 0;
		}
		else 
		{
			//����u���b�N���݂����ꍇ�߂̈ʒu�����ɖ߂�
			pPuyo->MovePuyoPosInMapY(-2);
			return 0;
		}
	}

	//�����܂Œ����Ɖ�]�s�\�𔻒f
	return -1;
}


/*==========================================================================================
	�Ղ�ƃX�e�[�W�̓����蔻��
		�߂�l����:
			0 :�����蔻�肠��
			-1:�����蔻��Ȃ�
==========================================================================================*/
int Collision::CollisionStageAndPuyo(const Stage* pStage, const Puyo* pPuyo)
{
	//�k���`�F�b�N
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:CollisionStageAndPuyo],����pStage�̓k��");
		return -1;
	}
	if (pPuyo == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:CollisionStageAndPuyo],����pPuyo�̓k��");
		return -1;
	}
	
	//�����蔻�茟�o
	Vector2 PuyoPosInMap = pPuyo->GetPuyoPosInMapInt();	//�Ղ�̃X�e�[�W��̈ʒu�擾
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			//�Ղ�̂Ղ�̒l��0�Ȃ��΂�
			int PuyoBlockValue = pPuyo->GetPuyoMtxValue(RowNum, ColumnNum);
			if (PuyoBlockValue == 0)
			{
				continue;
			}

			//�Ή��X�e�[�W�̂Ղ�̈ʒu
			Vector2 BlockPosInMap;
			BlockPosInMap.x = ColumnNum + PuyoPosInMap.x;
			BlockPosInMap.y = RowNum + PuyoPosInMap.y;

			//�Ή��Ղ��X�ʒu���X�e�[�W�[�𒴂����瓖���蔻�蔭��
			if (BlockPosInMap.x <= 0 || BlockPosInMap.x >= MAP_BLOCK_NUM_HORIZON - 1)
			{
				return 0;
			}

			// �Ή��Ղ��Y�ʒu���X�e�[�W�O�Ȃ��΂�
			if (BlockPosInMap.y < 0 || BlockPosInMap.y > MAP_BLOCK_NUM_VERTICAL - 1)
			{
				continue;
			}

			//�Ή��X�e�[�W�̂Ղ�̒l��0�Ȃ��΂�
			int StageBlockValue = pStage->GetStageMtxValue(BlockPosInMap.y, BlockPosInMap.x);
			if (StageBlockValue == 0)
			{
				continue;
			}

			//�����܂ŒH�����Ȃ瓖���蔻�肠��
			return 0;
		}
	}

	//�����܂ŒH�����Ȃ瓖���蔻��Ȃ�
	return -1;
}