/*==========================================================================================
ヘッダファイル説明:
	テトリスステージのヘッダファイル
==========================================================================================*/
#ifndef STAGE_H_
#define STAGE_H_

/*==========================================================================================
インクルードヘッダ
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define MAP_BLOCK_NUM_HORIZON (8)		//水平方向ステージのぷよ数
#define MAP_BLOCK_NUM_VERTICAL (13)		//垂直方向ステージのぷよ数
#define MAP_TEX_MAX (7)					//ステージのテクスチャの数

/*==========================================================================================
	構造体定義
==========================================================================================*/
typedef struct 
{
	bool	IsCanDrop;			//落下できるフラグ(trueならこのぷよは落下可能)
	int		DropDist;			//落下距離(単位はぷよ)
}DROP_TO_GOAL;

/*==========================================================================================
	先方宣言
==========================================================================================*/
class Score;

/*==========================================================================================
	クラス定義
==========================================================================================*/
class Stage 
{
public:
	Stage();
	~Stage();

	bool Init(void);			//初期化処理
	void Draw(void);			 //描画処理
	void Uninit(void);			 //終了処理

	//セッター
	void SetStageMtxValue(int RowNum, int ColumnNum, int Value);	//ステージの行列の値を設定

	//ゲッター
	Vector2 GetPos(void) const {return m_Pos;}
	int	GetStageMtxValue(int RowNum, int ColumnNum) const;			//ステージの行列の値を返す
	int GetKillMarkMtxValue(int RowNum, int ColumnNum) const;		//消える予定があるぷよマーク行列の値を返す

	//他の関数
	void CheckDropPuyo(void);			//落下ぷよをマークし、落下終了後のステージ行列を作る
	bool DropPuyo(void);				//空中に浮いてるぷよを落下させる
	void DropDataWriteInMap(void);		//落下結果をステージに反映する

	bool CheckAllKillBlock(Score*);		//消す予定があるぷよをマーク
	bool KillBlock(Score*);				//ブロックを消す
private:
	void RecursiveCheckKillBlock(Vector2 Position, int BlockValue, 
		int *pKillCount, int KillBlockCheck[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON]);	//消す予定があるぷよをマーク
private:
	Vector2			m_Pos;					//ステージ左上のぷよの中心座標
	int				m_Map[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];				//ステージのデータ
	Vector2f		m_MapBlockPos[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];		//ぷよの中央座標

	DROP_TO_GOAL	m_MarkDropMtx[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];		//ぷよの落下マーク行列
	int				m_DropOverMap[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];		//落下終了後のステージ行列
	int				m_KillBlockMarkMtx[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON];	//消える予定があるぷよマーク行列(trueなら消す、falseなら消さない)
	int				m_GraphHandle[MAP_TEX_MAX];			//ステージのテクスチャハンドル
	int				m_HandleBG;							//背景のテクスチャハンドル
};

#endif