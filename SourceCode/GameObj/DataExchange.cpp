#include "DataExchange.h"
#include "DxLib.h"
#include "Stage.h"
#include "Puyo.h"

/*==========================================================================================
pIn�Ղ�̃f�[�^��pOut�Ղ�ɏ�������
�߂�l����:
0 :�f�[�^�̏������ݐ���
-1:�f�[�^�̏������ݎ��s
==========================================================================================*/
int DataExchange::WriteDataInPuyo(Puyo* pOut, const Puyo* pIn)
{
	//�k���`�F�b�N
	if (pOut == nullptr)
	{
		ErrorLogFmtAdd("[DataExchange.cpp][Function:WriteDataInPuyo],����pOut�̓k��");
		return -1;
	}
	if (pIn == nullptr)
	{
		ErrorLogFmtAdd("[DataExchange.cpp][Function:WriteDataInPuyo],����pIn�̓k��");
		return -1;
	}

	//�X�e�[�W��̈ʒu�̏�������
	Vector2f PosInMap = pIn->GetPuyoPosInMapFloat();
	pOut->SetPuyoPosInMap(PosInMap);

	//�s��f�[�^�̏�������
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			pOut->SetPuyoMtxValue(RowNum, ColumnNum, pIn->GetPuyoMtxValue(RowNum, ColumnNum));
		}
	}

	return 0;
}

/*==========================================================================================
�Ղ�̃f�[�^���X�e�[�W�ɏ�������
�߂�l����:
0 :�f�[�^�̏������ݐ���
-1:�f�[�^�̏������ݎ��s
==========================================================================================*/
int DataExchange::WritePuyoDataInStage(Stage* pStage, const Puyo* pPuyo)
{
	//�k���`�F�b�N
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[DataExchange.cpp][Function:WritePuyoDataInStage],����pStage�̓k��");
		return -1;
	}
	if (pPuyo == nullptr)
	{
		ErrorLogFmtAdd("[DataExchange.cpp][Function:WritePuyoDataInStage],����pPuyo�̓k��");
		return -1;
	}

	//�Ղ�̃X�e�[�W��̈ʒu�擾
	Vector2 PuyoPosInMap = pPuyo->GetPuyoPosInMapInt();

	//�X�e�[�W�f�[�^�ɏ�������
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

			//�Ή��Ղ�̈ʒu���X�e�[�W�O�A�������̓X�e�[�W�[�Ȃ��΂�
			//(Y���̓X�e�[�W��[�̏��܂ŋ������)
			if (BlockPosInMap.x <= 0 || BlockPosInMap.x >= MAP_BLOCK_NUM_HORIZON - 1)
			{
				continue;
			}
			if (BlockPosInMap.y < 0 || BlockPosInMap.y >= MAP_BLOCK_NUM_VERTICAL - 1)
			{
				continue;
			}

			//�Ղ�̂Ղ�l���X�e�[�W�ɏ�������
			pStage->SetStageMtxValue(BlockPosInMap.y, BlockPosInMap.x, PuyoBlockValue);
		}
	}

	return 0;
}