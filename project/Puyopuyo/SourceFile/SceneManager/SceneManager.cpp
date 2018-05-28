/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "SceneManager.h"
#include "SceneGame.h"
#include "Input.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "DrawMethod.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define FADE_ALPHA_CHANGE_VALUE (5)
#define FADE_IMAGE_PASS "data/texture/fade.png"

/*==========================================================================================
	コンストラクター
==========================================================================================*/
SceneManager::SceneManager()
{
	m_State	= SCENE_TITLE;
	m_SceneFade = SCENE_FADE_IN;
	for (int i = 0; i < SCENE_MAX; i++) {
		m_Scene[i] = nullptr;
	}

	//フェード部分初期化
	m_FadeGraph = -1;
	m_FadeAlpha = 0;
}

/*==========================================================================================
	デストラクター
==========================================================================================*/
SceneManager::~SceneManager()
{
	//シーンオブジェクトの解放
	for (int i = 0; i < SCENE_MAX; i++) {
		if (nullptr != m_Scene[i])
		{
			m_Scene[i]->Uninit();	//シーン終了処理
			delete m_Scene[i];		//シーンオブジェクト解放
		}
	} 
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool SceneManager::Init(void)
{
	//フェード用画像の読み込み
	m_FadeGraph = LoadGraph(FADE_IMAGE_PASS);
	if (m_FadeGraph == -1) 
	{
		MessageBox(nullptr, "テクスチャ読み込み失敗", "error", MB_ICONHAND);
		ErrorLogFmtAdd("[SceneManager.cpp][Function:Init]m_FadeGraphのテクスチャ読み込み失敗");
		return false;
	}

	//シーンオブジェクトのメモリ確保
	for (int i = 0; i < SCENE_MAX; i++) {
		switch(i)
		{
		case SCENE_TITLE:
			if (m_Scene[i] == nullptr)
			{
				m_Scene[i] = new SceneTitle();
				if (m_Scene[i] == nullptr)
				{
					MessageBox(nullptr, "メモリ確保失敗、初期化失敗しました", "error", MB_ICONHAND);
					ErrorLogFmtAdd("[SceneManager.cpp][Function:Init]m_Scene[i]のメモリ確保できなかった");
					return false;
				}
			}
			break;
		case SCENE_GAME:
			if (m_Scene[i] == nullptr)
			{
				m_Scene[i] = new SceneGame();
				if (m_Scene[i] == nullptr) 
				{
					MessageBox(nullptr, "メモリ確保失敗、初期化失敗しました","error",MB_ICONHAND);
					ErrorLogFmtAdd("[SceneManager.cpp][Function:Init]m_Scene[i]のメモリ確保できなかった");
					return false;
				}
			}
			break;
		}
	}

	//シーンオブジェクト初期化
	if (m_Scene[m_State]->Init() == false) 
	{
		ErrorLogFmtAdd("[SceneManager.cpp][Function:Init]m_Scene[m_State]の初期化が失敗した");
		return false;
	}

	return true;
}

/*==========================================================================================
	更新処理
==========================================================================================*/
void SceneManager::Update(void)
{
	switch (m_SceneFade)
	{
	case SCENE_FADE_IN:
		m_FadeAlpha -= FADE_ALPHA_CHANGE_VALUE;
		if (m_FadeAlpha <= 0)
		{
			m_FadeAlpha = 0;
			m_SceneFade = SCENE_UPDATE;		//シーンの更新処理に移行
		}
		break;
	case SCENE_FADE_OUT:
		m_FadeAlpha += FADE_ALPHA_CHANGE_VALUE;
		if (m_FadeAlpha >= 255) 
		{
			m_FadeAlpha = 255;
			SceneTransition();				//シーン遷移処理
			m_SceneFade = SCENE_FADE_IN;	//フェードイン処理に移行
		}
		break;
	case SCENE_UPDATE:
		int FadeFlag = SceneUpdate();		//シーンの更新処理
		if (FadeFlag == -1) 
		{
			m_SceneFade = SCENE_FADE_OUT;	//フェードアウト処理に移行
		}
		break;
	}
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void SceneManager::Draw(void)
{
	//シーンオブジェクトの更新
	if (nullptr != m_Scene[m_State])
	{
		m_Scene[m_State]->Draw();	//シーンオブジェ更新処理
	}
	else
	{
		ErrorLogFmtAdd("[SceneManager.cpp][Function:Draw]m_Scene[m_State]はヌル");
	}

	//フェードの描画
	if (m_SceneFade != SCENE_UPDATE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_FadeAlpha);			//ブレンドモードオン
		int DrawResult = DrawMethod::DrawRect2D({ 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT }, m_FadeGraph);	//フェード描画
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//ブレンドモードをオフ
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void SceneManager::Uninit(void)
{
	//シーンオブジェクトの解放
	for (int i = 0; i < SCENE_MAX; i++) {
		if (nullptr != m_Scene[i])
		{
			m_Scene[i]->Uninit();		//シーン終了処理
			delete m_Scene[i];			//シーンオブジェクト解放
			m_Scene[i] = nullptr;		//ヌルシーンオブジェクトに戻す
		}
	}

	//フェード用テクスチャの解放
	DeleteGraph(m_FadeGraph);
}

/*==========================================================================================
	シーンの更新関数
		戻り値説明:
			-1:シーン遷移発生
			0 :シーン遷移発生しない
==========================================================================================*/
int SceneManager::SceneUpdate(void)
{
	//シーンオブジェクトの更新
	if (nullptr != m_Scene[m_State])
	{
		//シーンオブジェ更新処理
		m_Scene[m_State]->Update();

		if (m_Scene[m_State]->GetSceneTransitionFlag() == true)
		{
			return -1;
		}
	}
	else
	{
		ErrorLogFmtAdd("[SceneManager.cpp][Function:Update]m_Scene[m_State]はヌル");
	}

	return 0;
}

/*==========================================================================================
	シーン遷移
==========================================================================================*/
void SceneManager::SceneTransition(void)
{
	//フラグをfalseに
	m_Scene[m_State]->SetSceneTransitionFlag(false);

	//シーン遷移前クリア処理
	m_Scene[m_State]->Uninit();

	//ステート書き換え
	m_State = m_Scene[m_State]->GetSceneSetting();

	//遷移先シーンの初期化
	m_Scene[m_State]->Init();
}