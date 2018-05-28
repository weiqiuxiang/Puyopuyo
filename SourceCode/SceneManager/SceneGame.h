/*==========================================================================================
ヘッダファイル説明:
	ゲームのシーン
==========================================================================================*/
#ifndef SCENEGAME_H_
#define SCENEGAME_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "SceneBase.h"
#include "Stage.h"
#include "Score.h"
#include "Puyo.h"
#include "Ghost.h"
#include "GameOverDirection.h"
#include "ControlHelp.h"
#include "ParticleMaker.h"
#include "KillPuyoEffect.h"
#include "NextPuyo.h"

/*==========================================================================================
	enum:Gameの実行ステップ
==========================================================================================*/
typedef enum 
{
	GAME_CREATE_PUYO = 0,		//ぷよ生成
	GAME_CONTROL_PUYO,			//プレーヤーはぷよを操作
	GAME_DROP_PUYO,				//ぷよ落下
	GAME_KILL_PUYO,				//ぷよを消す
	GAME_KILL_PUYO_EFFECT,		//ぷよを消すエフェクト
	GAME_PAUSE,					//一時停止
	GAME_OVER,					//ゲームオーバー
	GAME_DELAY					//進行中一時停止
}GAME_STEP;

/*==========================================================================================
	クラス定義
==========================================================================================*/
class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	virtual bool Init(void);		//初期化処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理
	virtual void Uninit(void);		//終了処理

private:
	void Pause(void);						//ゲームのポーズ	
	float GetLevelDownSpeed(void);			//レベルに応じで落下速度を返す
	int ControlPuyoAndDoCollision(void);	//ぷよをコピーし、そのコピーを移動/回転させる
	int IsGameOver(void);					//ゲームオーバー用関数
	void MakeHardDropPaticle(void);			//ハードドロップのパーティクル作る
	bool SetKillPuyoEffectMtx(void);		//エフェクトの行列設定

	void SetPuyoProcessing(void);			//ぷよを置く時の処理
	void GamePauseUI(void);					//ゲームポーズUI
	void GameOverUI(void);					//ゲームオーバーUI
	void DrawGameOverMark(void);			//ゲームオーバーマーク描画
private:
	GAME_STEP		m_Step;				//ゲームの実行手順
	Stage			m_Stage;			//ステージメンバー変数
	Score			m_Score;			//スコア
	Puyo			m_Puyo;				//ぷよ
	KillPuyoEffect	m_KillPuyoEF;		//ぷよを消すエフェクト
	Ghost			m_Ghost;			//落下位置のガイド
	NextPuyo		m_NextPuyo;			//次のぷよ
	GameOverDirection	m_GameOverDirection;		//ゲームオーバー演出
	int				m_ButtonHandle[4];				//シーン遷移ボタンのハンドル
	ControlHelp		m_Help;					//ヘルプ画像
	ParticleMaker	m_ParticleMaker;		//パーティクルメーカー
	bool			m_PauseFlag;			//ポーズフラグ
	GAME_STEP		m_PauseSaveStep;		//ポーズ時、ゲームの実行手順を保存
	GAME_STEP		m_NextStep;				//delayの後に次に行くステップ
	int				m_DelayFrame;			//delayするフレーム数
	int				m_DropDelayTime;		//自然落下した時のdelay時間
	int				m_GameOverHandle;		//ゲームオーバーハンドル
};

#endif
