/*==========================================================================================
ヘッダファイル説明:
	プレーヤーが操作できるぷよのヘッダー
==========================================================================================*/
#ifndef PUYO_H_
#define PUYO_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define PUYO_MATRIX_SIZE (3)	//ぷよの行列のサイズ
#define PUYO_TYPE_MAX (5)		//ぷよのタイプ数
#define PUYO_BLOCK_SIZE (48)	//ぷよのサイズ

/*==========================================================================================
	クラス定義
==========================================================================================*/
class Puyo 
{
public:
	Puyo();
	~Puyo();

	bool Init(void);						//初期化処理
	int LoadTexture(const char*);			//テクスチャ取得
	void UpdateAlpha(void);					//アルファの部分の更新
	void Draw(const Vector2& MapPos);		//描画処理(引数でステージ位置を知る必要がある)
	void Uninit(void);						//終了処理

	//セッター
	void SetPuyoMtxValue(int RowNum, int ColumnNum,int Value);		//ぷよの行列に値を設定
	void SetPuyoPosInMap(const Vector2f& pos) { m_PosInMap = pos; }	//ステージ上の座標設定
	void SetPuyoPosXInMap(float PosX) { m_PosInMap.x = PosX; }
	void SetPuyoPosYInMap(float PosY) { m_PosInMap.y = PosY; }

	//ゲッター
	Vector2 GetPuyoPosInMapInt(void) const { return{ (int)m_PosInMap.x,(int)m_PosInMap.y }; }	//ぷよのステージ上の位置(int)
	Vector2f GetPuyoPosInMapFloat(void) const { return m_PosInMap; }		//ぷよのステージ上の位置(float)
	int GetPuyoMtxValue (int RowNum, int ColumnNum) const;		//行列の要素値を返す
	bool GetRotFlag(void) { return m_RotFlag; }					//回転フラグ取得

	//他の関数
	void MovePuyoPosInMapX(int MoveValue);		//水平移動
	void MovePuyoPosInMapY(float MoveValue);	//垂直移動
	void LeftRotate(void);			//左回転
	void RightRotate(void);			//右回転
	void MatrixRorate(int Mtx[PUYO_MATRIX_SIZE][PUYO_MATRIX_SIZE], bool RotDir);	//回転用関数
	bool PuyoAlignOnXAxis(void);								//ぷよはX軸方向に直線揃ってる
	void RotToGoal(void);										//目標に向かって回転

private:
	Vector2f	m_PosInMap;										//ステージ上の位置
	int			m_Mtx[PUYO_MATRIX_SIZE][PUYO_MATRIX_SIZE];		//ぷよの行列
	int			m_GraphHandle[PUYO_TYPE_MAX + 1];				//グラフィックハンドル
	int			m_RotCenterAlpha;								//回転中心のアルファ値
	bool		m_AlphaState;									//アルファ状態(falseは加算、trueは減算)

	Vector2f	m_RotPuyoPos;									//回転部の座標
	Vector2f	m_RotGoalPos;									//回転部の目標座標
	bool		m_RotDir;										//回転方向
	bool		m_RotFlag;										//回転フラグ
};

#endif