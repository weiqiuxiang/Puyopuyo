/*==========================================================================================
ヘッダファイル説明:
	ステージの全値は1の行にエフェクトをつける
==========================================================================================*/
#ifndef KILL_PUYO_EFFECT_H_
#define KILL_PUYO_EFFECT_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Common.h"
#include "Stage.h"
#include "Puyo.h"

/*==========================================================================================
	クラス定義
==========================================================================================*/
class KillPuyoEffect 
{
public:
	KillPuyoEffect();
	~KillPuyoEffect() {}

	bool Init(void);				//初期化処理
	void Uninit(void);				//終了処理
	void Update(int);				//更新処理
	void Draw(const Vector2& MapPos);	//ライン消すエフェクト描画処理

	//ゲッター
	int GetEFCount(void) const { return m_EffectCount; }

	//セッター
	void SetEFCount(int Value) { m_EffectCount = Value; }
	void SetAlphaValue(int Value) { m_AlphaValue = Value; }
	void SetEffectMtx(int RowNum, int ColumnNum,int Value);		//エフェクトの行列をセット
private:
	int m_EffectMtx[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];
	int m_EffectCount;						//エフェクトの継続時間
	int m_AlphaValue;						//アルファ値
	int m_BlockEFHandle[PUYO_TYPE_MAX];		//ブロックのエフェクトハンドル
};

#endif
