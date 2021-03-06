/*==========================================================================================
ヘッダファイル説明:
	置き場所を教えるためのガイドのクラス
==========================================================================================*/
#ifndef GHOST_H_
#define GHOST_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Stage.h"
#include "Puyo.h"

/*==========================================================================================
	クラス定義
==========================================================================================*/
class Ghost
{
public:
	Ghost();
	~Ghost();

	bool Init(void);			//初期化処理
	void CalcPosInMap(const Stage*,const Puyo*);		//ステージ上の位置計算
	void Draw(const Vector2& MapPos);		//描画処理
	void Uninit(void);

	Vector2 GetRotCenterPosInMap(void) const { return{ m_PosInMap[0].x - 1, m_PosInMap[0].y - 1}; }	//左上座標を返す
private:
	Vector2		m_PosInMap[2];									//ステージ上の位置
	int			m_GuildValue[2];								//ガイドの所の値
	int			m_GraphHandle[PUYO_TYPE_MAX];					//グラフィックハンドル
};
#endif