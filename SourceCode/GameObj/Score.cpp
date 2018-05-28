/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Score.h"
#include "DrawMethod.h"
#include "DxLib.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define DIGIT_NUM (8)			//桁数
#define	NUMBER_WIDTH (48)		//数字の幅
#define	NUMBER_HEIGHT (64)		//数字の高さ
#define NUMBER_INTERVAL (48)	//数字の間隔
#define DEFAULT_SCORE_POSX (SCREEN_WIDTH * 0.39f)	//スコアのデフォルトX座標
#define DEFAULT_SCORE_POSY (SCREEN_HEIGHT * 0.9f)	//スコアのデフォルトY座標
#define SCORE_TEX_PASS "data/texture/Score.png"		//スコアのテクスチャのパス

/*==========================================================================================
	const変数
==========================================================================================*/
const int ComboBonus[20] =		//コンボボーナス
{
	0,0,8,16,32,64,96,128,160,192,224,256,288,320,352,384,416,448,480,512
};
const int ConnectBonus[12] =	//連結ボーナス
{
	0,0,0,0,0,2,3,4,5,6,7,10
};
const int ColorBonus[6] =		//色ボーナス
{
	0,0,3,6,12,24
};

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
Score::Score()
{
	m_Score = 0;		//スコア初期化
	m_DrawPos = { (int)DEFAULT_SCORE_POSX , (int)DEFAULT_SCORE_POSY };	//描画位置の初期化

	//テクスチャの読み込み
	for (int i = 0; i < SCORE_TEX_MAX; i++)
	{
		m_GraphHandle[i] = -1;
	}

	StartCalcScore();			//計算用パラメータをリセット
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
Score::~Score()
{

}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool Score::Init(void)
{
	m_Score = 0;		//スコア初期化
	m_DrawPos = { (int)DEFAULT_SCORE_POSX , (int)DEFAULT_SCORE_POSY };	//描画位置の初期化

	for (int i = 0; i < SCORE_TEX_MAX; i++) 
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//テクスチャの読み込み
	int LoadTexResult = LoadDivGraph(SCORE_TEX_PASS,10,10,1,96,128, m_GraphHandle);
	if (LoadTexResult == -1) 
	{
		MessageBox(nullptr,"スコアのテクスチャ読み込み失敗","error",MB_ICONHAND);
		return false;
	}

	StartCalcScore();			//計算用パラメータをリセット

	return true;
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void Score::Draw(void)
{
	//スコア描画
	int Score = m_Score;
	for (int i = 0; i < DIGIT_NUM; i++) 
	{
		//値取得
		int DrawNumValue = Score % 10;
		Score = Score / 10;

		//位置計算
		Vector2 Pos;
		Pos.x = m_DrawPos.x + (DIGIT_NUM - 1 - i) * NUMBER_INTERVAL;
		Pos.y = m_DrawPos.y;

		//描画
		DrawMethod::DrawRect2D(Pos, { NUMBER_WIDTH ,NUMBER_HEIGHT }, m_GraphHandle[DrawNumValue]);
	}
}

/*==========================================================================================
	終了処理
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
	スコアの計算開始(計算用変数の初期化)
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
	色数計算
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
	コンボ
==========================================================================================*/
void Score::AddCombo(void)
{
	m_Combo++;
}

/*==========================================================================================
	スコア計算
	引数説明:
		PuyoNum: 消したぷよ数
		ConnectNum: 連結数
==========================================================================================*/
void Score::CalcScore(int PuyoNum)
{
	//コンボ補正
	int Combo = max(m_Combo,19);
	Combo = min(0, m_Combo);

	//連結数補正
	m_ConnectNum = max(m_ConnectNum, 11);
	m_ConnectNum = min(0, m_ConnectNum);

	//色数
	int ColorNum = 0;
	for (int i = 0; i < PUYO_TYPE_MAX; i++) 
	{
		if (m_KillPuyoColor[i] == true) 
		{
			ColorNum++;
		}
	}

	//ボーナス計算
	int TotalBonus = ComboBonus[Combo] + ConnectBonus[m_ConnectNum] + ColorBonus[ColorNum];
	if (TotalBonus == 0) 
	{
		TotalBonus = 1;
	}

	m_ScoreCalc = m_Score + PuyoNum * TotalBonus * 10;
}