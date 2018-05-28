#include "NextPuyo.h"
#include "DrawMethod.h"
#include "DxLib.h"

/*==========================================================================================
	マクロ
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
	コンストラクタ
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
	デストラクタ
==========================================================================================*/
NextPuyo::~NextPuyo() 
{

}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool NextPuyo::Init(void)
{
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		m_NextPuyoMtx[i][0] = GetRand(PUYO_TYPE_MAX - 1) + 2;
		m_NextPuyoMtx[i][1] = GetRand(PUYO_TYPE_MAX - 1) + 2;
	}

	//読み込んだテクスチャハンドル削除
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_PuyoImageHandle[i]);
	}
	DeleteGraph(m_WindowImageHandle);

	//テクスチャの読み込み
	int TexGetResult = LoadDivGraph(PUYO_TEX_PASS, 5, 3, 2, 128, 128, m_PuyoImageHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "ぷよのテクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		return false;
	}

	m_WindowImageHandle = LoadGraph(NEXT_WINDOW_TEX_PASS);
	if (m_WindowImageHandle == -1) 
	{
		MessageBox(nullptr, "テクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		return false;
	}

	return true;
}


/*==========================================================================================
	描画処理
==========================================================================================*/
void NextPuyo::Draw(void)
{
	//ネクストウィンドウの描画
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		DrawMethod::DrawRect2D(
		{ (int)WINDOW_POSX ,(int)WINDOW_POSY + i * WINDOW_SIZE_Y },
		{ WINDOW_SIZE_X ,WINDOW_SIZE_Y },
			m_WindowImageHandle
		);
	}

	//ネクストぷよ1の描画
	for (int i = 0; i < NEXT_STORAGE; i++)
	{
		DrawMethod::DrawRect2DCenter(
		{ (int)NEXT_PUYO_1_POSX ,(int)NEXT_PUYO_1_POSY + i * PUYO_BLOCK_SIZE },
		{ PUYO_BLOCK_SIZE ,PUYO_BLOCK_SIZE },
			m_PuyoImageHandle[m_NextPuyoMtx[0][i] - 2]
		);
	}

	//ネクストぷよ2の描画
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
	終了処理
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
	次のぷよの取得
==========================================================================================*/
void NextPuyo::GetNextPuyo(int pOut[PUYO_NUM])
{
	//次のぷよを代入
	pOut[0] = m_NextPuyoMtx[0][0];
	pOut[1] = m_NextPuyoMtx[0][1];

	//行列ずらす
	m_NextPuyoMtx[0][0] = m_NextPuyoMtx[1][0];
	m_NextPuyoMtx[0][1] = m_NextPuyoMtx[1][1];

	//次のぷよ取得
	m_NextPuyoMtx[1][0] = GetRand(PUYO_TYPE_MAX - 1) + 2;
	m_NextPuyoMtx[1][1] = GetRand(PUYO_TYPE_MAX - 1) + 2;
}