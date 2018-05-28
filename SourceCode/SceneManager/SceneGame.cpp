/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "SceneGame.h"
#include "Input.h"
#include "Common.h"
#include "DxLib.h"
#include "Collision.h"
#include "DataExchange.h"
#include "Gui.h"
#include "DrawMethod.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define DOWN_INTPUT_SPEED (0.5f)		//入力落下速度
#define DOWN_NATIVE_SPEED (0.02f)		//自然落下速度
#define DROP_DELAY_TIME (10)

#define EFFECT_TIME (20)		//エフェクト継続時間(フレーム数)
#define PUYO_TEX_PASS "data/texture/PuyoColor.png"		//ぷよのテクスチャファイルパス

#define BUTTON_SIZEX (256)				//ボタンの横幅
#define BUTTON_SIZEY (128)				//ボタンの縦幅
#define BUTTON_TEX_PASS "data/texture/SceneGameButton.png"		//ボタンのファイルパス
#define GAME_OVER_MARK_TEX_PASS "data/texture/GameOverMark.png"	//ゲームオーバーマークのファイルパス

/*==========================================================================================
	const変数
==========================================================================================*/
const float SPEED_RATE[5] = 
{
	1.0f,1.1f,1.2f,1.3f,1.4f
};

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
SceneGame::SceneGame()
{
	m_Step = GAME_CONTROL_PUYO;		//最初はぷよコントロール段階の移行
	m_PauseFlag = false;
	m_NextStep = GAME_CREATE_PUYO;
	m_DelayFrame = DROP_DELAY_TIME;

	//ハンドルの初期化
	for (int i = 0; i < 4; i++) 
	{
		m_ButtonHandle[i] = -1;
	}
	m_GameOverHandle = -1;
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
SceneGame::~SceneGame()
{
	
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool SceneGame::Init(void)
{
	//メンバ変数初期化
	m_Step = GAME_CONTROL_PUYO;		//最初はぷよコントロール段階の移行
	m_PauseFlag = false;			//ポーズフラグ初期化
	m_NextStep = GAME_CREATE_PUYO;
	m_DelayFrame = DROP_DELAY_TIME;

	//入力装置初期化
	if (Input::Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]Inputの初期化失敗");
		return false;
	}

	//ステージ初期化
	if (m_Stage.Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Stageの初期化失敗");
		return false;
	}

	//スコア初期化
	if (m_Score.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Scoreの初期化失敗");
		return false;
	}

	//ネクスト初期化
	if (m_NextPuyo.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_NextPuyoの初期化失敗");
		return false;
	}

	//ぷよ初期化
	if (m_Puyo.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Puyoの初期化失敗");
		return false;
	}
	else 
	{
		//最初のぷよ取得
		m_Puyo.SetPuyoPosInMap({ MAP_BLOCK_NUM_HORIZON / 2 - 1, -2 });	//初期位置設定

		//ネクストからぷよを取得
		int NextPuyo[2];
		m_NextPuyo.GetNextPuyo(NextPuyo);
		m_Puyo.SetPuyoMtxValue(0, 1, NextPuyo[0]);
		m_Puyo.SetPuyoMtxValue(1, 1, NextPuyo[1]);

		//ぷよのテクスチャの読み込み
		if (m_Puyo.LoadTexture(PUYO_TEX_PASS) == -1)
		{
			ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Puyoのテクスチャ読み込み失敗");
			return false;
		}
	}

	//ぷよ消すエフェクトの初期化
	if (m_KillPuyoEF.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_KillPuyoEFの初期化失敗");
		return false;
	}

	//ガイド初期化
	if (m_Ghost.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Ghostの初期化失敗");
		return false;
	}

	//ゲームオーバー演出初期化
	if (m_GameOverDirection.Init() == false) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_GameOverDirectionの初期化失敗");
		return false;
	}

	//ヘルプ初期化
	if (m_Help.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_Helpの初期化失敗");
		return false;
	}

	//パーティクルメーカー初期化
	if (m_ParticleMaker.Init() == false)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_ParticleMakerの初期化失敗");
		return false;
	}

	//シーン遷移ボタンのテクスチャの読み込み
	DeleteGraph(m_ButtonHandle[0]);
	DeleteGraph(m_ButtonHandle[1]);
	int LoadTexResult = LoadDivGraph(BUTTON_TEX_PASS, 4, 2, 2, 256, 128, m_ButtonHandle);
	if (LoadTexResult == -1)
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_ButtonHandleの読み込み失敗");
		return false;
	}

	//ゲームオーバーマークのテクスチャ読み込む
	m_GameOverHandle = LoadGraph(GAME_OVER_MARK_TEX_PASS);
	if (m_GameOverHandle == -1) 
	{
		ErrorLogFmtAdd("[SceneGame.cpp][Function:Init]m_GameOverHandleの読み込み失敗");
		return false;
	}
	
	return true;
}

/*==========================================================================================
	更新処理
==========================================================================================*/
void SceneGame::Update(void)
{
	Input::Update();			//入力更新

	if (m_Step != GAME_OVER)
	{
		m_Help.Update();			//ヘルプ更新
		m_ParticleMaker.Update();	//パーティクルメーカー更新
		Pause();					//ポーズ処理
	}

	switch (m_Step)
	{
	case GAME_CREATE_PUYO:
	{
		int NextPuyo[2];
		m_NextPuyo.GetNextPuyo(NextPuyo);

		//ぷよにをネクストの値を入れる
		m_Puyo.Init();
		m_Puyo.SetPuyoMtxValue(0, 1, NextPuyo[0]);
		m_Puyo.SetPuyoMtxValue(1, 1, NextPuyo[1]);

		m_Ghost.CalcPosInMap(&m_Stage, &m_Puyo);	//ガイドの位置の計算
		m_Step = GAME_CONTROL_PUYO;					//ぷよコントロールに移行
		m_DropDelayTime = DROP_DELAY_TIME;			//落下の遅延時間リセット

		m_Score.StartCalcScore();					//スコアのパラメータをリセット
		break;
	}
	case GAME_CONTROL_PUYO:
	{
		//ゲームオーバー判定
		if (IsGameOver() == 0)
		{
			m_GameOverDirection.SetUseFlag(true);		//ゲームオーバー演出開始
			m_Step = GAME_OVER;							//ゲームオーバー段階に移行
			break;
		}

		//ぷよの回転中心点滅エフェクトのアルファ値更新
		m_Puyo.UpdateAlpha();

		//ぷよのコントロールかつステージとの当たり判定を行う
		int SetPuyo = ControlPuyoAndDoCollision();

		//ガイド位置の計算
		m_Ghost.CalcPosInMap(&m_Stage,&m_Puyo);

		//ぷよをステージに置く判定発生
		if (SetPuyo == 0)
		{
			SetPuyoProcessing();	//置く判定後の処理
		}

		break;
	}

	case GAME_DROP_PUYO:
	{
		bool DropPuyoExist = m_Stage.DropPuyo();		//ぷよ落下

		//落下ぷよはない
		if (DropPuyoExist == false)
		{
			m_Stage.DropDataWriteInMap();							//落下結果をステージに反映
			bool MarkExist = m_Stage.CheckAllKillBlock(&m_Score);	//消せるぷよをマーク

			//消せるぷよ存在する
			if (MarkExist)
			{
				m_Step = GAME_KILL_PUYO;				//消す段階に移行
			}
			else
			{
				m_DelayFrame = 10;						//delay時間設定
				m_NextStep = GAME_CREATE_PUYO;			//ぷよ生成段階に戻す
				m_Step = GAME_DELAY;					//delayモードに移行
			}
		}

		break;
	}

	case GAME_KILL_PUYO:
	{
		bool KillResult = m_Stage.KillBlock(&m_Score);		//ぷよを消す

		if (KillResult == true)
		{
			//演出初期化
			SetKillPuyoEffectMtx();					//ぷよ消すエフェクト行列を設定
			m_KillPuyoEF.SetAlphaValue(255);		//アルファ初期値設定
			m_KillPuyoEF.SetEFCount(40);			//演出時間設定
			m_Step = GAME_KILL_PUYO_EFFECT;			//消す演出に移行
			m_Score.AddCombo();						//コンボ
		}
		else 
		{
			m_DelayFrame = 10;						//delay時間設定
			m_NextStep = GAME_CREATE_PUYO;			//ぷよ生成段階に戻す
			m_Step = GAME_DELAY;					//delayモードに移行
		}
		break;
	}

	case GAME_KILL_PUYO_EFFECT:
	{
		m_KillPuyoEF.Update(-5);
		int EfCount = m_KillPuyoEF.GetEFCount();
		if (EfCount <= 0)
		{
			m_Stage.CheckDropPuyo();			//落下ぷよをマーク
			m_Step = GAME_DROP_PUYO;			//落下段階に移行
			m_Score.ApplyCalcScore();			//計算したスコアを反映させる
		}
		break;
	}

	case GAME_PAUSE:
		GamePauseUI();							//ゲームポーズUIの表示
		break;

	case GAME_OVER:
		m_GameOverDirection.Update();

		//演出終了していなげれば
		if (m_GameOverDirection.GetStep() != GAME_OVER_DIRECTION_OVER) 
		{
			m_PauseFlag = false;
			break;
		}

		GameOverUI();							//ゲームオーバーのUIを表示
		break;

	case GAME_DELAY:
		m_DelayFrame--;
		if (m_DelayFrame < 0) 
		{
			m_DelayFrame = 0;
			m_Step = m_NextStep;
		}
		break;
	}
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void SceneGame::Draw(void)
{
	m_Stage.Draw();					//ステージの描画
	DrawGameOverMark();				//ゲームオーバーマークの描画

	if (m_Step == GAME_CONTROL_PUYO)
	{
		m_Ghost.Draw(m_Stage.GetPos());	//ガイドの描画
		m_Puyo.Draw(m_Stage.GetPos());	//ぷよの描画
	}

	if (m_Step == GAME_KILL_PUYO_EFFECT)
	{
		m_KillPuyoEF.Draw(m_Stage.GetPos());
	}
	m_NextPuyo.Draw();				//ネクストの描画
	m_Score.Draw();					//スコアの描画
	m_Help.Draw();					//ヘルプ描画

	//ゲームオーバー演出
	m_GameOverDirection.Draw();

	//パーティクル描画
	m_ParticleMaker.Draw();
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void SceneGame::Uninit(void)
{
	m_Stage.Uninit();				//ステージ解放
	m_Score.Uninit();				//スコア解放
	m_Puyo.Uninit();				//スコアの解放
	m_Ghost.Uninit();				//ガイドの解放
	m_GameOverDirection.Uninit();	//ゲームオーバー演出の解放
	m_Help.Uninit();				//ヘルプを解放

	//シーン遷移ボタンのテクスチャの解放
	for (int i = 0; i < 4; i++) 
	{
		DeleteGraph(m_ButtonHandle[i]);
		m_ButtonHandle[i] = -1;
	}
	DeleteGraph(m_GameOverHandle);
	m_GameOverHandle = -1;
}

/*==========================================================================================
	ゲームのポーズ
==========================================================================================*/
void SceneGame::Pause(void)
{
	if(m_PauseFlag == false)
	{
		Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.7f),(int)(SCREEN_HEIGHT * 0.05) });
		Gui::SetButtonSize({ (int)(BUTTON_SIZEX * 0.4f),(int)(BUTTON_SIZEY * 0.4) });
		if (Gui::Button("MENU", m_ButtonHandle[0]))
		{
			m_PauseFlag = true;
			m_PauseSaveStep = m_Step;
			m_Step = GAME_PAUSE;
		}
	}
}

/*==========================================================================================
	レベルに応じで落下速度を返す
==========================================================================================*/
float SceneGame::GetLevelDownSpeed(void)
{
	unsigned int Score = m_Score.GetScore();
	int Level = 0;

	if (Score > 5000)
	{
		Level = 1;
	}

	else if (Score > 20000)
	{
		Level = 2;
	}

	else if (Score > 50000)
	{
		Level = 3;
	}

	else if (Score > 100000)
	{
		Level = 4;
	}

	return DOWN_NATIVE_SPEED * SPEED_RATE[Level];
}

/*==========================================================================================
	コピーを移動/回転させ、当たり判定をし、当たり判定後の位置を調整する
		戻り値：
			-1: ステージにぷよを置く判定発生しない
			0 : ステージにぷよを置く判定発生する
==========================================================================================*/
int SceneGame::ControlPuyoAndDoCollision(void)
{
	int MoveValueX = 0;		//X軸の移動量
	float MoveValueY = 0;	//Y軸の移動量
	int RotDirValue = 0;	//回転方向値(左回転なら1減少、右回転なら1増加)

	//ぷよの左移動
	if (Input::GetTrigger(KEY_INPUT_A) || Input::GetRepeat(KEY_INPUT_A))
	{
		MoveValueX -= 1;
	}

	//ぷよの右移動
	if (Input::GetTrigger(KEY_INPUT_D) || Input::GetRepeat(KEY_INPUT_D))
	{
		MoveValueX += 1;
	}

	//ぷよの下移動
	if (Input::GetPress(KEY_INPUT_S))		//下移動
	{
		MoveValueY += DOWN_INTPUT_SPEED;	//下移動
	}

	//ぷよ時間経過による下移動
	bool NativeDrop = false;
	if (MoveValueY == 0)
	{
		MoveValueY += GetLevelDownSpeed();	//下移動自然落下
		NativeDrop = true;
	}

	//回転判定
	bool RotFlag = m_Puyo.GetRotFlag();
	if (RotFlag == false)
	{
		//ぷよの左回転
		if (Input::GetTrigger(KEY_INPUT_J))
		{
			RotDirValue -= 1;
		}

		//ぷよの右回転
		if (Input::GetTrigger(KEY_INPUT_K))
		{
			RotDirValue += 1;
		}
	}

	else 
	{
		m_Puyo.RotToGoal();
	}

	//ぷよのインスタンスのコピーを作る
	Puyo PuyoCopyInstance = m_Puyo;	

	//横移動の当たり判定
	if (MoveValueX != 0)
	{
		PuyoCopyInstance.MovePuyoPosInMapX(MoveValueX);	//コピーを横移動する
		PuyoCopyInstance.MovePuyoPosInMapY(1);			//一個下はブロック存在する場当たり判定があるか

		//コピーとステージの当たり判定
		int CollisionResult = Collision::CollisionStageAndPuyo(&m_Stage, &PuyoCopyInstance);

		//一個上に戻る
		PuyoCopyInstance.MovePuyoPosInMapY(-1);

		//判定なしなら
		if (CollisionResult == -1)
		{
			//コピーと同じの横移動量
			m_Puyo.MovePuyoPosInMapX(MoveValueX);
		}
		else
		{
			//コピーを元の位置に戻す
			PuyoCopyInstance.MovePuyoPosInMapX(-MoveValueX);
		}
	}

	//縦移動の当たり判定
	if (MoveValueY != 0) 
	{
		//コピーを落下させる(1マスの下にブロックがあるか、予測判定する)
		PuyoCopyInstance.MovePuyoPosInMapY(MoveValueY + 1);

		//コピーとステージの当たり判定
		int CollisionResult = Collision::CollisionStageAndPuyo(&m_Stage, &PuyoCopyInstance);

		//予測判定位置から戻の位置に戻る
		PuyoCopyInstance.MovePuyoPosInMapY(-MoveValueY - 1);

		//当たり判定発生しない
		if (CollisionResult == -1) 
		{
			//ぷよ縦移動
			m_Puyo.MovePuyoPosInMapY(MoveValueY);
		}
		else 
		{
			//ぷよの置き判定
			if (NativeDrop == true)
			{
				m_DropDelayTime--;
				if (m_DropDelayTime < 0 && m_Puyo.GetRotFlag() == false)
				{
					m_Puyo.MovePuyoPosInMapY(1);		//予測当たりの位置に置く
					return 0;
				}
			}
			else 
			{
				if (m_Puyo.GetRotFlag() == false)
				{
					m_Puyo.MovePuyoPosInMapY(1);		//予測当たりの位置に置く
					return 0;
				}
			}
		}
	}

	//回転の当たり判定
	if (RotDirValue != 0) 
	{
		int RotResult = -1;

		//左回転
		if (RotDirValue == -1) 
		{
			PuyoCopyInstance.LeftRotate();
			RotResult = Collision::MovePuyoPosAfterRot(&m_Stage, &PuyoCopyInstance, false);
		}
		else if (RotDirValue == 1) 
		{
			PuyoCopyInstance.RightRotate();
			RotResult = Collision::MovePuyoPosAfterRot(&m_Stage, &PuyoCopyInstance, true);
		}

		//回転結果で当たり判定する
		if (RotResult == 0) //回転できるなら
		{
			//コピーをぷよに書き込む
			m_Puyo = PuyoCopyInstance;
		}
	}

	return -1;
}

/*==========================================================================================
	ゲームオーバー用関数
		戻り値説明:
			0 :ゲームオーバー判定あり
			-1:ゲームオーバー判定なし
==========================================================================================*/
int SceneGame::IsGameOver(void)
{
	if (m_Stage.GetStageMtxValue(0, 3) >= 1) 
	{
		return 0;
	}

	if (m_Stage.GetStageMtxValue(0, 4) >= 1)
	{
		return 0;
	}

	return -1;
}

/*==========================================================================================
	ハードドロップのパーティクル作る
==========================================================================================*/
void SceneGame::MakeHardDropPaticle(void)
{
	//パーティクルの発生位置計算
	Vector2 PuyoPosInMap = m_Ghost.GetRotCenterPosInMap();	//ぷよのステージ上座標取得
	Vector2 MapPos = m_Stage.GetPos();						//ステージの画面左上の座標
	
	Vector2 ParticlePos;
	ParticlePos.x = MapPos.x + PuyoPosInMap.x * PUYO_BLOCK_SIZE + PUYO_BLOCK_SIZE;
	ParticlePos.y = MapPos.y + PuyoPosInMap.y * PUYO_BLOCK_SIZE + 2 * PUYO_BLOCK_SIZE;

	m_ParticleMaker.PuyoHardDropParticle(ParticlePos, {8,8},-5,0.05f);
}

/*==========================================================================================
	エフェクトの行列設定
==========================================================================================*/
bool SceneGame::SetKillPuyoEffectMtx(void) 
{
	bool MarkPuyoExist = false;

	for (int RowNum = 0; RowNum < MAP_BLOCK_NUM_VERTICAL - 1; RowNum++)
	{
		for (int ColumnNum = 1; ColumnNum < MAP_BLOCK_NUM_HORIZON - 1; ColumnNum++)
		{
			if (m_Stage.GetKillMarkMtxValue(RowNum, ColumnNum) > 1)
			{
				MarkPuyoExist = true;
			}
			m_KillPuyoEF.SetEffectMtx(RowNum,ColumnNum,m_Stage.GetKillMarkMtxValue(RowNum, ColumnNum) );
		}
	}

	return MarkPuyoExist;
}

/*==========================================================================================
	ぷよセット後の処理
==========================================================================================*/
void SceneGame::SetPuyoProcessing(void)
{
	DataExchange::WritePuyoDataInStage(&m_Stage, &m_Puyo);			//ぷよの行列データをステージに書き込む
	m_Stage.CheckDropPuyo();		//落下ぷよをマーク
	m_Step = GAME_DROP_PUYO;		//落下段階に移行
}

/*==========================================================================================
	ゲームポーズUI
==========================================================================================*/
void SceneGame::GamePauseUI(void)
{
	//ゲームに戻るボタン
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.30) });
	Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
	if (Gui::Button("BACK", m_ButtonHandle[1]))
	{
		m_Step = m_PauseSaveStep;		//元のステップに戻す
		m_PauseFlag = false;			//ポーズフラグをfalseに戻る
	}

	//リタイアボタン
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.45) });
	Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
	if (Gui::Button("RETIRE", m_ButtonHandle[2]))
	{
		m_State = SCENE_GAME;
		m_SceneTransitionFlag = true;
	}

	//タイトル遷移ボタン
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.60) });
	Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
	if (Gui::Button("TITLE", m_ButtonHandle[3]))
	{
		m_State = SCENE_TITLE;
		m_SceneTransitionFlag = true;
	}
}

/*==========================================================================================
	ゲームオーバーUI
==========================================================================================*/
void SceneGame::GameOverUI(void)
{
	//リタイアボタン
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.45) });
	Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
	if (Gui::Button("RETIRE", m_ButtonHandle[2]))
	{
		m_State = SCENE_GAME;
		m_SceneTransitionFlag = true;
	}

	//タイトル遷移ボタン
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH * 0.5),(int)(SCREEN_HEIGHT * 0.60) });
	Gui::SetButtonSize({ BUTTON_SIZEX,BUTTON_SIZEY });
	if (Gui::Button("TITLE", m_ButtonHandle[3]))
	{
		m_State = SCENE_TITLE;
		m_SceneTransitionFlag = true;
	}
}

/*==========================================================================================
	ゲームオーバーマーク描画
==========================================================================================*/
void SceneGame::DrawGameOverMark(void)
{
	//ステージ座標取得
	Vector2 StagePos = m_Stage.GetPos();

	//ゲームオーバーマーク描画
	for (int i = 3; i < 5; i++) 
	{
		Vector2 DrawPos;
		DrawPos.x = StagePos.x + i * PUYO_BLOCK_SIZE;
		DrawPos.y = StagePos.y + PUYO_BLOCK_SIZE;
		DrawMethod::DrawRect2DCenter(DrawPos, { PUYO_BLOCK_SIZE ,PUYO_BLOCK_SIZE },m_GameOverHandle);
	}
}