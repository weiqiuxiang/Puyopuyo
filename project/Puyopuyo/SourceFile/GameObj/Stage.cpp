/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Stage.h"
#include "DrawMethod.h"
#include "DxLib.h"
#include "Puyo.h"
#include "Score.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define STAGE_DEFAULT_POSX (SCREEN_WIDTH*0.05f)			//ステージの位置のデフォルトX値
#define STAGE_DEFAULT_POSY (SCREEN_HEIGHT*0.05f)			//ステージの位置のデフォルトY値
#define PUYO_DROP_SPEED (0.1f)			//ぷよぷよの落下速度

#define STAGE_TEX_PASS "data/texture/StageBlockColor.png"	//ぷよのテクスチャファイルパス
#define STAGEBG_TEX_PASS "data/texture/BG.png"				//ステージのテクスチャファイルパス

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
Stage::Stage()
{
	//ステージ初期化
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			bool MapLeftEnd = (ColumnNum == 0);							//ステージ左端
			bool MapRightEnd = (ColumnNum == MAP_BLOCK_NUM_HORIZON -1);	//ステージ右端
			bool MapBottom = (RowNum == MAP_BLOCK_NUM_VERTICAL-1);		//ステージの底
			if(MapLeftEnd || MapRightEnd || MapBottom)
			{
				m_Map[RowNum][ColumnNum] = 1;			//ステージ端の値
			}
			else 
			{
				m_Map[RowNum][ColumnNum] = 0;
			}

			//ぷよの位置代入
			m_MapBlockPos[RowNum][ColumnNum] = { (float)ColumnNum ,(float)RowNum };
		}
	}

	//座標初期化
	m_Pos = { (int)STAGE_DEFAULT_POSX ,(int)STAGE_DEFAULT_POSY };

	//テクスチャハンドル初期化
	for (int i = 0; i < MAP_TEX_MAX; i++)
	{
		m_GraphHandle[i] = -1;
	}
	m_HandleBG = -1;
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
Stage::~Stage(){}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool Stage::Init(void)
{
	//ステージ初期化
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			bool MapLeftEnd = (ColumnNum == 0);							//ステージ左端
			bool MapRightEnd = (ColumnNum == MAP_BLOCK_NUM_HORIZON - 1);	//ステージ右端
			bool MapBottom = (RowNum == MAP_BLOCK_NUM_VERTICAL - 1);		//ステージの底
			if (MapLeftEnd || MapRightEnd || MapBottom)
			{
				m_Map[RowNum][ColumnNum] = 1;			//ステージ端の値
			}
			else
			{
				m_Map[RowNum][ColumnNum] = 0;
			}

			//ぷよの位置代入
			m_MapBlockPos[RowNum][ColumnNum] = { (float)ColumnNum ,(float)RowNum };
		}
	}

	//座標初期化
	m_Pos = { (int)STAGE_DEFAULT_POSX ,(int)STAGE_DEFAULT_POSY };

	//テクスチャ解放
	for (int i = 0; i < MAP_TEX_MAX;i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//テクスチャ取得
	int TexGetResult = LoadDivGraph(STAGE_TEX_PASS, 7, 3, 3, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "ステージのテクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		return false;
	}

	m_HandleBG = LoadGraph(STAGEBG_TEX_PASS);
	if (m_HandleBG == -1)
	{
		MessageBox(nullptr, "ステージのテクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void Stage::Draw(void)
{
	//ステージ背景の描画
	Vector2 BGSize;
	BGSize.x = (MAP_BLOCK_NUM_HORIZON - 1) * PUYO_BLOCK_SIZE;
	BGSize.y = (MAP_BLOCK_NUM_VERTICAL - 1) * PUYO_BLOCK_SIZE;

	DrawMethod::DrawRect2D(
	{ (int)STAGE_DEFAULT_POSX + 24,(int)STAGE_DEFAULT_POSY + 24 },BGSize,m_HandleBG);

	//ステージの描画
	//0行目は描画しない
	for (int RowNum = 1; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			//位置計算
			Vector2 Pos;
			Pos.x = m_Pos.x + (int)(m_MapBlockPos[RowNum][ColumnNum].x * PUYO_BLOCK_SIZE);
			Pos.y = m_Pos.y + (int)(m_MapBlockPos[RowNum][ColumnNum].y * PUYO_BLOCK_SIZE);

			//描画
			DrawMethod::DrawRect2DCenter(
				Pos,
				{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
				m_GraphHandle[(int)m_Map[RowNum][ColumnNum]]
			);
		}
	}
}

/*==========================================================================================
	終了処理
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
	ステージの行列の値を返す
		引数説明:
			RowNum:行
			ColumnNum:列
			Value:設定する値
==========================================================================================*/
void Stage::SetStageMtxValue(int RowNum, int ColumnNum, int Value)
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:SetStageMtxValue],引数RowNumは範囲外");
		return;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:SetStageMtxValue],引数ColumnNumは範囲外");
		return;
	}

	m_Map[RowNum][ColumnNum] = Value;
}

/*==========================================================================================
	ステージの行列の値を返す
	引数説明:
		RowNum:行
		ColumnNum:列
==========================================================================================*/
int	Stage::GetStageMtxValue(int RowNum, int ColumnNum) const
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetStageMtxValue],引数RowNumは範囲外");
		return -1;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetStageMtxValue],引数ColumnNumは範囲外");
		return -1;
	}

	return m_Map[RowNum][ColumnNum];
}

/*==========================================================================================
	ステージの行列の値を返す
	引数説明:
		RowNum:行
		ColumnNum:列
==========================================================================================*/
int Stage::GetKillMarkMtxValue(int RowNum, int ColumnNum) const
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetKillMarkMtxValue],引数RowNumは範囲外");
		return false;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:GetKillMarkMtxValue],引数ColumnNumは範囲外");
		return false;
	}

	return m_KillBlockMarkMtx[RowNum][ColumnNum];
}

/*==========================================================================================
	落下ぷよをマークし、落下終了後のステージ行列を作る
==========================================================================================*/
void Stage::CheckDropPuyo(void) 
{
	//下から検索
	for (int RowNum = MAP_BLOCK_NUM_VERTICAL - 2; RowNum >= 0; RowNum--)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			m_MapBlockPos[RowNum][ColumnNum] = { (float)ColumnNum ,(float)RowNum };		//位置初期化

			//ぷよではない
			if (m_Map[RowNum][ColumnNum] < 1)
			{
				m_MarkDropMtx[RowNum][ColumnNum].IsCanDrop = false;			//落下できない
				m_DropOverMap[RowNum][ColumnNum] = m_Map[RowNum][ColumnNum];	//落下後のステージに書き込む
				continue;
			}

			//落下位置検索
			int DistFromBottom = MAP_BLOCK_NUM_VERTICAL - 2 - RowNum;		//底との距離を測る
			int BlockNum = 0;			//そのぷよから底までの間に存在するぷよ数
			for (int i = 1; i <= DistFromBottom; i++) 
			{
				//ぷよだ
				if (m_Map[RowNum + i][ColumnNum] > 1) 
				{
					BlockNum++;
				}
			}
			m_MarkDropMtx[RowNum][ColumnNum].DropDist = DistFromBottom - BlockNum;	//落下距離計算
			if (m_MarkDropMtx[RowNum][ColumnNum].DropDist == 0)	//落下距離は0
			{
				m_MarkDropMtx[RowNum][ColumnNum].IsCanDrop = false;
				m_DropOverMap[RowNum][ColumnNum] = m_Map[RowNum][ColumnNum];	//落下後のステージ行列に書き込む
			}
			else
			{
				m_MarkDropMtx[RowNum][ColumnNum].IsCanDrop = true;
				int Goal = m_MarkDropMtx[RowNum][ColumnNum].DropDist + RowNum;
				m_DropOverMap[RowNum][ColumnNum] = 0;							//元の所のデータを消す
				m_DropOverMap[Goal][ColumnNum] = m_Map[RowNum][ColumnNum];		//ゴール先を落下後のステージ行列に書き込む
			}
		}
	}
}

/*==========================================================================================
	空中に浮いてるぷよを落下させる
		戻り値:
			false :動かせるぷよはない
			true:動かすぷよかある
==========================================================================================*/
bool Stage::DropPuyo(void)
{
	bool DropExist = false;		//落下するぷよ存在する

	//動かせるぷよを検索し、落下させ(したから検索)
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			//落下できない部分をチェック
			if (m_MarkDropMtx[RowNum][ColumnNum].IsCanDrop == false)
			{
				continue;
			}

			//落下
			int Goal = RowNum + m_MarkDropMtx[RowNum][ColumnNum].DropDist;
			m_MapBlockPos[RowNum][ColumnNum].y += PUYO_DROP_SPEED;

			//落下目標に到着
			if (m_MapBlockPos[RowNum][ColumnNum].y > Goal)
			{
				m_MapBlockPos[RowNum][ColumnNum].y = (float)Goal;		//位置をゴールに合わす
				m_MarkDropMtx[RowNum][ColumnNum].IsCanDrop = false;		//落下不能に設定
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
	落下結果をステージに反映する
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
	消す予定があるぷよをマーク
	戻り値説明:
		true:  消すブロックが存在する
		false: 消すブロックが存在しない
==========================================================================================*/
bool Stage::CheckAllKillBlock(Score* pScore)
{
	if (pScore == nullptr) 
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:CheckAllKillBlock],引数pScoreはヌル");
		return false;
	}

	//消すぷよのフラグ行列初期化
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
		{
			m_KillBlockMarkMtx[RowNum][ColumnNum] = false;
		}
	}

	bool MarkPuyoExist = false;		//消すぷよが存在するフラグ

	//消すぷよをマーク(右下から検索)
	for (int RowNum = MAP_BLOCK_NUM_VERTICAL - 2; RowNum >= 0; RowNum--)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			//ぷよではない
			if (m_Map[RowNum][ColumnNum] < 1) 
			{
				continue;
			}
			
			//マーク済み
			if (m_KillBlockMarkMtx[RowNum][ColumnNum] > 1)
			{
				continue;
			}

			//フラグを保存する構造体を作る
			int KillBlockCheck[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];
			for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL; RowNum++)
			{
				for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON; ColumnNum++)
				{
					KillBlockCheck[RowNum][ColumnNum] = 0;
				}
			}

			int KillCount = 0;		//マーク数

			//マーク開始(再帰)
			RecursiveCheckKillBlock({ ColumnNum ,RowNum }, m_Map[RowNum][ColumnNum], &KillCount, KillBlockCheck);

			//マーク数は4以上
			if (KillCount >= 4) 
			{
				for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
				{
					for (int ColumnNum = 0; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
					{
						//マークした部分を代入
						if (KillBlockCheck[RowNum][ColumnNum] > 1) 
						{
							m_KillBlockMarkMtx[RowNum][ColumnNum] = KillBlockCheck[RowNum][ColumnNum];
						}
					}
				}

				MarkPuyoExist = true;			//消すぷよが存在する
			}

			//スコアに連結数を追加
			pScore->AddConnectNum(KillCount);
		}
	}

	return MarkPuyoExist;
}

/*==========================================================================================
	消す予定があるぷよをマーク
	引数説明:
		Position:チェックするぷよのステージ上の位置
		BlockValue:マークするぷよの値
		Dir:検索された方向
		KillBlockCheck:マーク行列
==========================================================================================*/
void Stage::RecursiveCheckKillBlock(Vector2 Position, int BlockValue, int *pKillCount,int KillBlockCheck[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON])
{
	//範囲外チェック
	if (Position.x < 0 || Position.x >= MAP_BLOCK_NUM_HORIZON)
	{
		return;
	}
	if (Position.y < 0 || Position.y >= MAP_BLOCK_NUM_VERTICAL)
	{
		return;
	}
	//マーク済みを除外する
	if (KillBlockCheck[Position.y][Position.x] > 1) 
	{
		return;
	}

	//マーク対象ではない
	if (m_Map[Position.y][Position.x] != BlockValue)
	{
		return;
	}

	KillBlockCheck[Position.y][Position.x] = BlockValue;	//マーク
	*pKillCount = *pKillCount + 1;							//数インクリメント

	//左に検索
	RecursiveCheckKillBlock({ Position.x - 1,Position.y }, BlockValue, pKillCount, KillBlockCheck);
	//右に検索
	RecursiveCheckKillBlock({ Position.x + 1,Position.y }, BlockValue, pKillCount, KillBlockCheck);
	//上に検索
	RecursiveCheckKillBlock({ Position.x,Position.y - 1 }, BlockValue, pKillCount, KillBlockCheck);
	//下に検索
	RecursiveCheckKillBlock({ Position.x,Position.y + 1 }, BlockValue, pKillCount, KillBlockCheck);
}

/*==========================================================================================
	ぷよを消す
		戻り値説明:
			true:	消したぷよ数は1以上ある
			false:  消したぷよ数は0
==========================================================================================*/
bool Stage::KillBlock(Score* pScore)
{
	if (pScore == nullptr)
	{
		ErrorLogFmtAdd("[Stage.cpp][Function:KillBlock],引数pScoreはヌル");
		return false;
	}

	int KillPuyoNum = 0;
	bool KillBlockExist = false;

	//色数を数える
	pScore->CalcColorBonus(m_KillBlockMarkMtx);

	//ぷよを消す
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

	pScore->CalcScore(KillPuyoNum);			//スコア計算

	return KillBlockExist;
}