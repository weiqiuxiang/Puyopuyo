/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Ghost.h"
#include "Collision.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "DataExchange.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define GHOST_TEX_PASS "data/texture/Ghost.png"		//ぷよゴーストのテクスチャファイルパス

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
Ghost::Ghost() 
{
	for (int i = 0; i < PUYO_TYPE_MAX; i++) 
	{
		m_GraphHandle[i] = -1;
	}

	//ステージ上位置初期化
	m_PosInMap[0] = { -10,-10 };
	m_PosInMap[1] = { -10,-10 };
	m_GuildValue[0] = 0;
	m_GuildValue[1] = 0;
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
Ghost::~Ghost() 
{
	
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool Ghost::Init(void)
{
	//取得したテクスチャ破棄
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//テクスチャ取得
	int TexGetResult = LoadDivGraph(GHOST_TEX_PASS, 5, 3, 2, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "ぷよゴーストのテクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		ErrorLogFmtAdd("[Ghost.cpp][Function:Init],ハンドルm_GraphHandle,テクスチャ読み込み失敗");
		return false;
	}

	//ステージ上位置初期化
	m_PosInMap[0] = { -10,-10 };
	m_PosInMap[1] = { -10,-10 };
	m_GuildValue[0] = 0;
	m_GuildValue[1] = 0;

	return true;
}

/*==========================================================================================
	ステージ上位置の計算
==========================================================================================*/
void Ghost::CalcPosInMap(const Stage* pStage,const Puyo* pPuyo)
{
	//ヌルチェック
	if (pPuyo == nullptr)
	{
		ErrorLogFmtAdd("[Ghost.cpp][Function:CalcPosInMap],引数pPuyoはヌル");
		return;
	}
	if (pStage == nullptr)
	{
		ErrorLogFmtAdd("[Ghost.cpp][Function:CalcPosInMap],引数pStageはヌル");
		return;
	}

	//位置検索
	Vector2 PosInMap = pPuyo->GetPuyoPosInMapInt();		//マップ上座標取得
	m_PosInMap[0] = { PosInMap.x + 1 , PosInMap.y + 1};	//回転中心座標計算
	m_GuildValue[0] = pPuyo->GetPuyoMtxValue(1,1);		//回転中心部値の代入

	//回転部分の位置を代入
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

	bool Up = m_PosInMap[1].y < m_PosInMap[0].y;		//回転部の座標は回転中心の上
	bool Down = m_PosInMap[1].y > m_PosInMap[0].y;		//回転部の座標は回転中心の下

	//ガイドの座標計算
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

	//位置ずらす
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
	描画処理
==========================================================================================*/
void Ghost::Draw(const Vector2& MapPos)
{
	//描画
	for (int Guild = 0; Guild < 2; Guild++)
	{
		//位置計算
		Vector2 Pos;
		Pos.x = MapPos.x + m_PosInMap[Guild].x * PUYO_BLOCK_SIZE;
		Pos.y = MapPos.y + m_PosInMap[Guild].y * PUYO_BLOCK_SIZE;

		//描画
		DrawMethod::DrawRect2DCenter(
			Pos,
			{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
			m_GraphHandle[m_GuildValue[Guild] - 2]
		);
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void Ghost::Uninit(void)
{
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
		m_GraphHandle[i] = -1;
	}
}

