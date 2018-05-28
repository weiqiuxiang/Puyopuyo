/*==========================================================================================
ヘッダファイル説明:
	ゲームの中のスコアクラス
==========================================================================================*/
#ifndef SCORE_H_
#define SCORE_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Common.h"
#include "Puyo.h"
#include "Stage.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define SCORE_TEX_MAX (10)		//スコアのテクスチャ数
#define KILL_ONE_LINE (200)		//1ラインを消す時の点数
#define KILL_TWO_LINE (800)		//2ラインを消す時の点数
#define KILL_THREE_LINE (1800)	//3ラインを消す時の点数
#define KILL_FOUR_LINE (2500)	//4ラインを消す時の点数

/*==========================================================================================
	クラス定義
==========================================================================================*/
class Score 
{
public:
	Score();
	~Score();

	bool Init(void);	//初期化処理
	void Draw(void);	//描画処理
	void Uninit(void);	//終了処理

	void SetPos(const Vector2& pos) { m_DrawPos = pos; }		//描画位置を設定
	int GetScore(void) const { return m_Score; }				//スコア

	void StartCalcScore(void);			//スコアの計算開始(計算用変数の初期化)
	void CalcColorBonus(const int KillPuyo[MAP_BLOCK_NUM_VERTICAL][MAP_BLOCK_NUM_HORIZON]);	//色数計算
	void AddCombo(void);	//コンボ+1
	void AddConnectNum(int Num) { m_ConnectNum += Num; }		//連結数を追加
	void CalcScore(int PuyoNum);							//スコア計算
	void ApplyCalcScore(void) {m_Score = m_ScoreCalc;}		//計算スコアオブジェの値をスコアに適用する
private:
	Vector2 m_DrawPos;				//描画位置
	unsigned int m_Score;			//スコア
	unsigned int m_ScoreCalc;		//計算スコアオブジェ

	int		m_GraphHandle[SCORE_TEX_MAX];	//グラフィックハンドル
	int		m_Combo;						//コンボ数
	int		m_ConnectNum;					//連結数
	bool	m_KillPuyoColor[PUYO_TYPE_MAX];	//消したぷよの色数
};

#endif