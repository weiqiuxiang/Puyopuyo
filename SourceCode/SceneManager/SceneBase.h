/*==========================================================================================
ヘッダファイル説明:
	シーンのベースインタフェース
==========================================================================================*/
#ifndef SCENEBASE_H_
#define SCENEBASE_H_

/*==========================================================================================
	列挙型:SCENE_STATE
==========================================================================================*/
typedef enum {
	SCENE_TITLE = 0,
	SCENE_GAME,
	SCENE_MAX
}SCENE_STATE;

/*==========================================================================================
	クラス定義
==========================================================================================*/
class SceneBase 
{
public:
	inline SceneBase() {
		m_State = SCENE_GAME;
		m_SceneTransitionFlag = false;
	}
	~SceneBase() {}
	virtual bool Init(void) = 0;		//初期化処理
	virtual void Update(void) = 0;		//更新処理
	virtual void Draw(void) = 0;		//描画処理
	virtual void Uninit(void) = 0;		//終了処理
	
	//シーン遷移予約
	inline void SetSceneTransition(const SCENE_STATE state) {
		m_State = state;
		m_SceneTransitionFlag = true;
	}

	//セッター
	void SetSceneTransitionFlag(bool TraFlag) { m_SceneTransitionFlag = TraFlag; }	//シーン遷移フラグ設定

	//ゲッター
	bool GetSceneTransitionFlag(void) const { return m_SceneTransitionFlag; }	//シーン遷移フラグ取得
	SCENE_STATE GetSceneSetting(void) const { return m_State; }					//予約されたシーン取得
protected:
	SCENE_STATE	m_State;					//シーンステート
	bool		m_SceneTransitionFlag;		//シーン遷移フラグ
};
#endif