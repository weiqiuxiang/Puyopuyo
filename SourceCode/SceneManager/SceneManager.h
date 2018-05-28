#ifndef MANAGER_H_
#define MANAGER_H_
/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "SceneBase.h"

/*==========================================================================================
	enum定義 : シーンフェード状態
==========================================================================================*/
typedef enum 
{
	SCENE_FADE_IN = 0,	//シーンフェードイン
	SCENE_FADE_OUT,		//シーンフェードアウト
	SCENE_UPDATE		//シーンの更新処理
}SCENE_fADE;

/*==========================================================================================
	クラス定義
==========================================================================================*/
class SceneManager 
{
public:
	SceneManager();
	~SceneManager();

	bool Init(void);		//初期化処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理
	void Uninit(void);		//終了処理
private:
	int SceneUpdate(void);				//シーンの更新処理
	void SceneTransition(void);			//シーン遷移
private:
	SCENE_STATE			m_State;				//シーンステート
	SCENE_fADE			m_SceneFade;			//シーンのフェード状態
	int					m_FadeGraph;			//フェード用画像ハンドル
	SceneBase*			m_Scene[SCENE_MAX];		//シーンオブジェクト配列
	int					m_FadeAlpha;			//フェードのアルファ値
};

#endif