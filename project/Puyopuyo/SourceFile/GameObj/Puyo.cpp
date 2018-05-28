/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Puyo.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "Stage.h"
#include <math.h>

/*==========================================================================================
	マクロ
==========================================================================================*/
#define PUYO_TEX_PASS "data/texture/PuyoColor.png"		//ぷよのテクスチャファイルパス
#define ALPHA_CHANGE_VALUE (5)							//アルファの変化値
#define PI (3.14159265359f)								//円周率

#define ROT_CENTER_ALPHA_MIN (50)						//回転中心のアルファ最小値
#define ROT_CENTER_ALPHA_MAX (180)						//回転中心のアルファ最大値
#define ROT_SPEED (0.05f * PI)							//回転速度

#define SCALE_SMALLER_TIME (20)							//スケール拡大にかかるフレーム数
#define SCALE_BIGGER_TIME (20)							//スケール縮小にかかるフレーム数
#define BIGGER_SPEED_Y (0.02f)							//スケール時の拡大速度
#define SMALLER_SPEED_Y (0.02f)							//スケール時の縮小速度
#define RETRUN_SPEED_Y (0.02f)							//スケール時の元サイズに戻る速度

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
Puyo::Puyo()
{
	//ランダムでぷよのタイプを決定
	int Up = GetRand(PUYO_TYPE_MAX - 1);
	int Center = GetRand(PUYO_TYPE_MAX - 1);

	//ぷよ行列の初期化
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			m_Mtx[RowNum][ColumnNum] = 0;
		}
	}

	//回転部分初期化
	m_RotPuyoPos = { 1.0f,  0.0f };
	m_RotGoalPos = m_RotPuyoPos;
	m_RotDir = false;
	m_RotFlag = false;

	//ステージ上位置初期化
	m_PosInMap = { MAP_BLOCK_NUM_HORIZON / 2 - 1, - 2};

	//テクスチャハンドル初期化
	for (int i = 0; i < PUYO_TYPE_MAX + 1; i++)
	{
		m_GraphHandle[i] = -1;
	}

	//アルファ値初期化
	m_RotCenterAlpha = 0;
	m_AlphaState = false;
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
Puyo::~Puyo()
{
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool Puyo::Init(void)
{
	//ランダムでぷよのタイプを決定
	int Up = GetRand(PUYO_TYPE_MAX - 1);
	int Center = GetRand(PUYO_TYPE_MAX - 1);

	//ぷよ行列の初期化
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			m_Mtx[RowNum][ColumnNum] = 0;
		}
	}

	//回転部分初期化
	m_RotPuyoPos = { 1.0f,  0.0f };
	m_RotGoalPos = m_RotPuyoPos;
	m_RotDir = false;
	m_RotFlag = false;

	//ステージ上位置初期化
	m_PosInMap = { MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 };

	//アルファ値初期化
	m_RotCenterAlpha = 0;
	m_AlphaState = false;

	return true;
}

/*==========================================================================================
	テクスチャ取得
==========================================================================================*/
int Puyo::LoadTexture(const char* TexPass)
{
	//読み込んだテクスチャを削除
	for (int i = 0; i < PUYO_TYPE_MAX + 1;i++) 
	{
		DeleteGraph(m_GraphHandle[i]);
	}

	//テクスチャ取得
	int TexGetResult = LoadDivGraph(TexPass, 6, 3, 2, 128, 128, m_GraphHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "ぷよのテクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		return -1;
	}

	return 0;
}

/*==========================================================================================
	アルファ部分の更新
==========================================================================================*/
void Puyo::UpdateAlpha(void)
{
	if (m_AlphaState == false) 
	{
		m_RotCenterAlpha += ALPHA_CHANGE_VALUE;
		if (m_RotCenterAlpha >= ROT_CENTER_ALPHA_MAX)
		{
			m_RotCenterAlpha = ROT_CENTER_ALPHA_MAX;
			m_AlphaState = true;
		}
	}

	else 
	{
		m_RotCenterAlpha -= ALPHA_CHANGE_VALUE;
		if (m_RotCenterAlpha <= 0)
		{
			m_RotCenterAlpha = 0;
			m_AlphaState = false;
		}
	}
}

/*==========================================================================================
	描画処理
	引数説明:
		MapPos:ステージの位置
==========================================================================================*/
void Puyo::Draw(const Vector2& MapPos)
{
	//位置計算
	Vector2 Pos;
	Pos.x = MapPos.x + (int)(m_PosInMap.x * PUYO_BLOCK_SIZE + PUYO_BLOCK_SIZE);
	Pos.y = MapPos.y + (int)(m_PosInMap.y * PUYO_BLOCK_SIZE + PUYO_BLOCK_SIZE);

	//回転中心の描画
	DrawMethod::DrawRect2DCenter(
		Pos,
		{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
		m_GraphHandle[m_Mtx[1][1] - 2]
	);

	//回転中心点滅効果描画
	SetDrawBlendMode(DX_BLENDMODE_ADD, m_RotCenterAlpha);	//ブレンドモードオン
	DrawMethod::DrawRect2DCenter(
		Pos,
		{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
		m_GraphHandle[5]
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//ブレンドモードをオフ

	//回転部の描画
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			if (m_Mtx[RowNum][ColumnNum] <= 1) 
			{
				continue;
			}
			if (ColumnNum == 1 && RowNum == 1) 
			{
				continue;
			}

			Pos.x = MapPos.x + (int)(m_PosInMap.x * PUYO_BLOCK_SIZE + m_RotPuyoPos.x * PUYO_BLOCK_SIZE);
			Pos.y = MapPos.y + (int)(m_PosInMap.y * PUYO_BLOCK_SIZE + m_RotPuyoPos.y * PUYO_BLOCK_SIZE);

			//回転部描画
			DrawMethod::DrawRect2DCenter(
				Pos,
				{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
				m_GraphHandle[m_Mtx[RowNum][ColumnNum] - 2]
			);

			//回転部の目標座標に代入
			m_RotGoalPos = { (float)ColumnNum , (float)RowNum };

			break;
		}
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void Puyo::Uninit(void)
{
	for (int i = 0; i < PUYO_BLOCK_SIZE; i++)
	{
		DeleteGraph(m_GraphHandle[i]);
	}
}

/*==========================================================================================
	ぷよの行列に値を設定
==========================================================================================*/
void Puyo::SetPuyoMtxValue(int RowNum, int ColumnNum, int Value)
{
	if (RowNum < 0 || RowNum >= PUYO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Puyo.cpp][Function:SetPuyoMtxValue],引数RowNumは範囲外");
		return;
	}

	if (ColumnNum <0 || ColumnNum >= PUYO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Puyo.cpp][Function:SetPuyoMtxValue],引数ColumnNumは範囲外");
		return;
	}

	m_Mtx[RowNum][ColumnNum] = Value;
}

/*==========================================================================================
	行列の要素値を返す
		引数説明:
			RowNum:行
			ColumnNum:列
==========================================================================================*/
int Puyo::GetPuyoMtxValue(int RowNum, int ColumnNum) const
{
	if (RowNum < 0 || RowNum >= PUYO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Puyo.cpp][Function:GetPuyoMtxValue],引数RowNumは範囲外");
		return -1;
	}

	if (ColumnNum < 0 || ColumnNum >= PUYO_MATRIX_SIZE)
	{
		ErrorLogFmtAdd("[Puyo.cpp][Function:GetPuyoMtxValue],引数ColumnNumは範囲外");
		return -1;
	}

	return m_Mtx[RowNum][ColumnNum];
}

/*==========================================================================================
	水平移動
==========================================================================================*/
void Puyo::MovePuyoPosInMapX(int MoveValue)
{
	m_PosInMap.x += MoveValue;
}

/*==========================================================================================
	垂直移動
==========================================================================================*/
void Puyo::MovePuyoPosInMapY(float MoveValue)
{
	m_PosInMap.y += MoveValue;
}

/*==========================================================================================
	左回転
==========================================================================================*/
void Puyo::LeftRotate(void)
{
	MatrixRorate(m_Mtx,false);
	m_RotDir = false;
	m_RotFlag = true;
}

/*==========================================================================================
	右回転
==========================================================================================*/
void Puyo::RightRotate(void)
{
	MatrixRorate(m_Mtx, true);
	m_RotDir = true;
	m_RotFlag = true;
}

/*==========================================================================================
	回転用関数
	引数説明:
		Mtx:ぷよ行列
		ElementCount: 行列数(例:値は2なら,処理する行列は2x2タイプ。3なら3x3タイプ)
		RotDir: 値falseの場合は左回転、値trueの場合は右回転
==========================================================================================*/
void Puyo::MatrixRorate(int Mtx[PUYO_MATRIX_SIZE][PUYO_MATRIX_SIZE], bool RotDir)
{
	//配列宣言
	int MtxSave[PUYO_MATRIX_SIZE][PUYO_MATRIX_SIZE];

	//ぷよの回転
	if (RotDir == false)	//左回転
	{
		for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
			{
				MtxSave[PUYO_MATRIX_SIZE - 1 - ColumnNum][RowNum] = Mtx[RowNum][ColumnNum];
			}
		}
	}
	else
	{
		for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
		{
			for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
			{
				MtxSave[ColumnNum][PUYO_MATRIX_SIZE - 1 - RowNum] = Mtx[RowNum][ColumnNum];
			}
		}
	}

	//回転結果を反映
	for (int RowNum = 0; RowNum < PUYO_MATRIX_SIZE; RowNum++)
	{
		for (int ColumnNum = 0; ColumnNum < PUYO_MATRIX_SIZE; ColumnNum++)
		{
			Mtx[RowNum][ColumnNum] = MtxSave[RowNum][ColumnNum];
		}
	}
}

/*==========================================================================================
	ぷよはX軸方向に直線揃ってる
		戻り値説明:
			false: X方向直線揃ってない
			true : X方向直線揃っている
==========================================================================================*/
bool Puyo::PuyoAlignOnXAxis(void)
{
	if (m_Mtx[1][0] > 1) 
	{
		return true;
	}

	if (m_Mtx[1][2] > 1)
	{
		return true;
	}

	return false;
}

/*==========================================================================================
	目標に向かって回転
==========================================================================================*/
void Puyo::RotToGoal(void)
{
	//回転不能状態
	if (m_RotFlag == false)
	{
		return;
	}

	//回転中心位置
	Vector2f RotCenterPos;
	RotCenterPos.x = 1.0f;
	RotCenterPos.y = 1.0f;

	//回転速度代入
	double RotSpeed;
	if (m_RotDir == false) 
	{
		RotSpeed = -ROT_SPEED;
	}
	else 
	{
		RotSpeed = ROT_SPEED;
	}

	//回転中心に沿って回転
	m_RotPuyoPos.x = (m_RotPuyoPos.x - RotCenterPos.x) * (float)cos(RotSpeed) - (m_RotPuyoPos.y - RotCenterPos.y) * (float)sin(RotSpeed) + RotCenterPos.x;
	m_RotPuyoPos.y = (m_RotPuyoPos.x - RotCenterPos.x) * (float)sin(RotSpeed) + (m_RotPuyoPos.y - RotCenterPos.y) * (float)cos(RotSpeed) + RotCenterPos.y;

	//回転目標地点に到着判定
	Vector2f RotCenterToGoalVec;		//回転中心から回転目標地点までのベクトル
	RotCenterToGoalVec.x = m_RotGoalPos.x - RotCenterPos.x;
	RotCenterToGoalVec.y = m_RotGoalPos.y - RotCenterPos.y;

	Vector2f RotCenterToRotPuyo;		//回転中心から回転ぷよまでのベクトル
	RotCenterToRotPuyo.x = m_RotPuyoPos.x - RotCenterPos.x;
	RotCenterToRotPuyo.y = m_RotPuyoPos.y - RotCenterPos.y;

	//外積で方向判定
	float Cross2D = RotCenterToRotPuyo.x * RotCenterToGoalVec.y - RotCenterToRotPuyo.y * RotCenterToGoalVec.x;
	//左回転の終了判定
	if (Cross2D > 0 && m_RotDir == false) 
	{
		m_RotPuyoPos = m_RotGoalPos;	//ゴール座標を代入
		m_RotFlag = false;				//回転終了
		return;
	}
	//右回転の終了判定
	if (Cross2D < 0 && m_RotDir == true)
	{
		m_RotPuyoPos = m_RotGoalPos;	//ゴール座標を代入
		m_RotFlag = false;				//回転終了
		return;
	}
}