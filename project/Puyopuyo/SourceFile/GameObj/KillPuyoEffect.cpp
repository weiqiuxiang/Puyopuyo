/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "KillPuyoEffect.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "Puyo.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define BLOCK_EF_TEX_FILEPASS "data/texture/KillPuyoEffect.png"

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
KillPuyoEffect::KillPuyoEffect()
{
	m_EffectCount = 0;		//カウントを0にする
	m_AlphaValue = 0;		//アルファの値を0にする

	//エフェクト配列初期化
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			m_EffectMtx[RowNum][ColumnNum] = 0;
		}
	}

	//ブロックのエフェクトハンドル初期化
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		m_BlockEFHandle[i] = -1;
	}
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool KillPuyoEffect::Init(void)
{
	m_EffectCount = 0;		//カウントを0にする
	m_AlphaValue = 0;		//アルファの値を0にする

	//エフェクト配列初期化
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			m_EffectMtx[RowNum][ColumnNum] = 0;
		}
	}

	//読み込んだテクスチャをクリア
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_BlockEFHandle[i]);
	}

	//テクスチャ読み込む
	int TexGetResult = LoadDivGraph(BLOCK_EF_TEX_FILEPASS, 5, 3, 2, 128, 128, m_BlockEFHandle);
	if (TexGetResult == -1)
	{
		MessageBox(nullptr, "ぷよのテクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	ライン消すエフェクト描画処理
		引数説明:
			MapPos: ステージの左上座標
			pKillPuyoNumber: 消せるラインの行番を記録するための配列アドレス
			KillPuyoNum: 消せるラインの数
==========================================================================================*/
void KillPuyoEffect::Draw(const Vector2& MapPos)
{
	//カウントは0以下なら描画しない
	if (m_EffectCount <= 0)
	{
		return;
	}
	
	//ラインのエフェクト描画
	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			if (m_EffectMtx[RowNum][ColumnNum] <= 1)
			{
				continue;
			}

			//位置計算
			Vector2 Pos;
			Pos.x = MapPos.x + ColumnNum * PUYO_BLOCK_SIZE;
			Pos.y = MapPos.y + RowNum * PUYO_BLOCK_SIZE;

			//描画
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_AlphaValue);		//ブレンドモードオン
			int DrawResult = DrawMethod::DrawRect2DCenter(
				Pos,
				{ PUYO_BLOCK_SIZE, PUYO_BLOCK_SIZE },
				m_BlockEFHandle[m_EffectMtx[RowNum][ColumnNum] - 2]
			);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//ブレンドモードをオフ
		}
	}
}

/*==========================================================================================
	更新処理
	引数説明:
		AlphaDecrementValue: アルファの変化値
==========================================================================================*/
void KillPuyoEffect::Update(int AlphaChangeValue)
{
	m_EffectCount = max(0, m_EffectCount - 1);
	m_AlphaValue = max(0, m_AlphaValue + AlphaChangeValue);
	m_AlphaValue = min(255, m_AlphaValue + AlphaChangeValue);
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void KillPuyoEffect::Uninit(void)
{
	//読み込んだテクスチャをクリア
	for (int i = 0; i < PUYO_TYPE_MAX; i++)
	{
		DeleteGraph(m_BlockEFHandle[i]);
		m_BlockEFHandle[i] = -1;
	}
}

/*==========================================================================================
	エフェクトの行列をセット
==========================================================================================*/
void KillPuyoEffect::SetEffectMtx(int RowNum, int ColumnNum, int Value)
{
	if (RowNum < 0 || RowNum >= MAP_BLOCK_NUM_VERTICAL)
	{
		ErrorLogFmtAdd("[KillPuyoEffect.cpp][Function:SetEffectMtx],引数RowNumは範囲外");
		return;
	}

	if (ColumnNum < 0 || ColumnNum >= MAP_BLOCK_NUM_HORIZON)
	{
		ErrorLogFmtAdd("[KillPuyoEffect.cpp][Function:SetEffectMtx],引数ColumnNumは範囲外");
		return;
	}

	m_EffectMtx[RowNum][ColumnNum] = Value;
}