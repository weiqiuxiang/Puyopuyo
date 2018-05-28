#include "DataExchange.h"
#include "DxLib.h"
#include "Stage.h"
#include "Puyo.h"

/*==========================================================================================
pInぷよのデータをpOutぷよに書き込む
戻り値説明:
0 :データの書き込み成功
-1:データの書き込み失敗
==========================================================================================*/
int DataExchange::WriteDataInPuyo(Puyo* pOut, const Puyo* pIn)
{
	//ヌルチェック
	if (pOut == nullptr)
	{
		ErrorLogFmtAdd("[DataExchange.cpp][Function:WriteDataInPuyo],引数pOutはヌル");
		return -1;
	}
	if (pIn == nullptr)
	{
		ErrorLogFmtAdd("[DataExchange.cpp][Function:WriteDataInPuyo],引数pInはヌル");
		return -1;
	}

	//ステージ上の位置の書き込み
	Vector2f PosInMap = pIn->GetPuyoPosInMapFloat();
	pOut->SetPuyoPosInMap(PosInMap);

	//行列データの書き込み
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
ぷよのデータをステージに書き込む
戻り値説明:
0 :データの書き込み成功
-1:データの書き込み失敗
==========================================================================================*/
int DataExchange::WritePuyoDataInStage(Stage* pStage, const Puyo* pPuyo)
{
	//ヌルチェック
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[DataExchange.cpp][Function:WritePuyoDataInStage],引数pStageはヌル");
		return -1;
	}
	if (pPuyo == nullptr)
	{
		ErrorLogFmtAdd("[DataExchange.cpp][Function:WritePuyoDataInStage],引数pPuyoはヌル");
		return -1;
	}

	//ぷよのステージ上の位置取得
	Vector2 PuyoPosInMap = pPuyo->GetPuyoPosInMapInt();

	//ステージデータに書き込む
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			//ぷよのぷよの値は0なら飛ばす
			int PuyoBlockValue = pPuyo->GetPuyoMtxValue(RowNum, ColumnNum);
			if (PuyoBlockValue == 0)
			{
				continue;
			}

			//対応ステージのぷよの位置
			Vector2 BlockPosInMap;
			BlockPosInMap.x = ColumnNum + PuyoPosInMap.x;
			BlockPosInMap.y = RowNum + PuyoPosInMap.y;

			//対応ぷよの位置がステージ外、もしくはステージ端なら飛ばす
			//(Y軸はステージ上端の所まで許される)
			if (BlockPosInMap.x <= 0 || BlockPosInMap.x >= MAP_BLOCK_NUM_HORIZON - 1)
			{
				continue;
			}
			if (BlockPosInMap.y < 0 || BlockPosInMap.y >= MAP_BLOCK_NUM_VERTICAL - 1)
			{
				continue;
			}

			//ぷよのぷよ値をステージに書き込む
			pStage->SetStageMtxValue(BlockPosInMap.y, BlockPosInMap.x, PuyoBlockValue);
		}
	}

	return 0;
}