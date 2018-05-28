/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Collision.h"
#include "DxLib.h"
#include "DataExchange.h"
#include "Puyo.h"
#include "Stage.h"

/*==========================================================================================
	回転後ぷよの位置調整
		引数説明:
			pStage: ステージのポインタ
			pPuyo: ぷよのポインタ
			RotDir: 回転方向(falseは左方向、trueは右方向)
		戻り値説明:
			0 :回転後処理終了
			-1:回転できない
==========================================================================================*/
int Collision::MovePuyoPosAfterRot(const Stage* pStage, Puyo* pPuyo, bool RotDir)
{
	//ヌルチェック
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MovePuyoPosAfterRotElement3],引数pStageはヌル");
		return -1;
	}
	if (pPuyo == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:MovePuyoPosAfterRotElement3],引数pPuyoはヌル");
		return -1;
	}

	//インスタンスのコピーを作る
	Puyo PuyoInstance = *pPuyo;

	//コピーの座標を左上に
	PuyoInstance.MovePuyoPosInMapX(-1);

	int HitResult[9];		//当たり判定結果

	//ぷよは,左上、上、右上、中間左、中間、中間右、左下、下、右下に移動する時の当たり判定を行う
	for (int RowNum = 0; RowNum < 3; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < 3; ColumnNum++)
		{
			int HitNum = ColumnNum + RowNum * 3;
			HitResult[HitNum] = CollisionStageAndPuyo(pStage, &PuyoInstance);
			PuyoInstance.MovePuyoPosInMapX(1);	//X軸を右1ずらす
		}
		PuyoInstance.MovePuyoPosInMapX(-3);	//X軸の座標を戻す
		PuyoInstance.MovePuyoPosInMapY(1);	//Yを1下に進む
	}

	//元の位置のの当たり判定結果
	if (HitResult[4] == -1)
	{
		return 0;
	}

	//上、下、左、右に移動した後の当たり判定結果
	if (HitResult[1] == -1)		//上
	{
		pPuyo->MovePuyoPosInMapY(-1);
		return 0;
	}
	if (HitResult[7] == -1)		//下
	{
		pPuyo->MovePuyoPosInMapY(-1);
		return 0;
	}
	if (HitResult[3] == -1)		//左
	{
		pPuyo->MovePuyoPosInMapX(-1);
		return 0;
	}
	if (HitResult[5] == -1)		//右
	{
		pPuyo->MovePuyoPosInMapX(1);
		return 0;
	}

	//左上、右上、左下、右下に移動した後の当たり判定結果
	if (HitResult[0] == -1)		//左上
	{
		pPuyo->MovePuyoPosInMapX(-1);
		pPuyo->MovePuyoPosInMapY(-1);
		return 0;
	}
	if (HitResult[2] == -1)		//右上
	{
		pPuyo->MovePuyoPosInMapX(1);
		pPuyo->MovePuyoPosInMapY(-1);
		return 0;
	}
	if (HitResult[6] == -1)		//左下
	{
		pPuyo->MovePuyoPosInMapX(-1);
		pPuyo->MovePuyoPosInMapY(1);
		return 0;
	}
	if (HitResult[8] == -1)		//右下
	{
		pPuyo->MovePuyoPosInMapX(1);
		pPuyo->MovePuyoPosInMapY(1);
		return 0;
	}

	//X軸一直線に並ぶ場合
	bool AlignXAxis = pPuyo->PuyoAlignOnXAxis();
	if (AlignXAxis == true) 
	{
		//左回転
		if (RotDir == false) 
		{
			pPuyo->LeftRotate();
		}
		else 
		{
			pPuyo->RightRotate();
		}

		//一個下にブロック存在するか
		pPuyo->MovePuyoPosInMapY(1);
		if (CollisionStageAndPuyo(pStage, pPuyo) == -1)
		{
			//一個下ブロック存在しない場合戻の位置に戻り
			pPuyo->MovePuyoPosInMapY(-1);
			return 0;
		}
		else 
		{
			//一個下ブロック存在した場合戻の位置より一個上に戻る
			pPuyo->MovePuyoPosInMapY(-2);
			return 0;
		}
	}

	//ここまで着くと回転不能を判断
	return -1;
}


/*==========================================================================================
	ぷよとステージの当たり判定
		戻り値説明:
			0 :当たり判定あり
			-1:当たり判定なし
==========================================================================================*/
int Collision::CollisionStageAndPuyo(const Stage* pStage, const Puyo* pPuyo)
{
	//ヌルチェック
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:CollisionStageAndPuyo],引数pStageはヌル");
		return -1;
	}
	if (pPuyo == nullptr)
	{
		ErrorLogFmtAdd("[Collision.cpp][Function:CollisionStageAndPuyo],引数pPuyoはヌル");
		return -1;
	}
	
	//当たり判定検出
	Vector2 PuyoPosInMap = pPuyo->GetPuyoPosInMapInt();	//ぷよのステージ上の位置取得
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

			//対応ぷよのX位置がステージ端を超えたら当たり判定発生
			if (BlockPosInMap.x <= 0 || BlockPosInMap.x >= MAP_BLOCK_NUM_HORIZON - 1)
			{
				return 0;
			}

			// 対応ぷよのY位置がステージ外なら飛ばす
			if (BlockPosInMap.y < 0 || BlockPosInMap.y > MAP_BLOCK_NUM_VERTICAL - 1)
			{
				continue;
			}

			//対応ステージのぷよの値は0なら飛ばす
			int StageBlockValue = pStage->GetStageMtxValue(BlockPosInMap.y, BlockPosInMap.x);
			if (StageBlockValue == 0)
			{
				continue;
			}

			//ここまで辿りつけるなら当たり判定あり
			return 0;
		}
	}

	//ここまで辿りつけるなら当たり判定なし
	return -1;
}