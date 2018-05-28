/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "SceneTitle.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include "Gui.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define TITLE_LOGO_TEX "data/texture/TitleLogo.png"
#define START_BUTTON_TEX "data/texture/StartButton.png"

#define LOGO_SIZEX (1024)
#define LOGO_SIZEY (384)

#define BUTTON_SIZEX (256)
#define BUTTON_SIZEY (64)

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
SceneTitle:: SceneTitle()
{
	m_TitleLogoHandle = -1;
	m_ButtonHandle = -1;
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
SceneTitle::~SceneTitle()
{

}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool SceneTitle::Init(void)
{
	//ロードする前にテクスチャを解放
	DeleteGraph(m_TitleLogoHandle);
	DeleteGraph(m_ButtonHandle);

	//タイトルロゴとスタートボタンのテクスチャの読み込み
	m_TitleLogoHandle = LoadGraph(TITLE_LOGO_TEX);
	m_ButtonHandle = LoadGraph(START_BUTTON_TEX);

	return true;
}

/*==========================================================================================
	更新処理
==========================================================================================*/
void SceneTitle::Update(void)
{
	//ゲームシーンに遷移するボタン
	Gui::SetButtonPos({ (int)(SCREEN_WIDTH*0.5),(int)(SCREEN_HEIGHT*0.7) });
	Gui::SetButtonSize({ BUTTON_SIZEX , BUTTON_SIZEY });
	if (Gui::Button("START_BUTTON", m_ButtonHandle))
	{
		m_State = SCENE_GAME;
		m_SceneTransitionFlag = true;
	}
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void SceneTitle::Draw(void)	
{
	//タイトルロゴの描画
	DrawMethod::DrawRect2DCenter({ (int)(SCREEN_WIDTH*0.5),(int)(SCREEN_HEIGHT*0.3) }, { LOGO_SIZEX ,LOGO_SIZEY }, m_TitleLogoHandle);
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void SceneTitle::Uninit(void)
{
	//ロードする前にテクスチャを解放
	DeleteGraph(m_TitleLogoHandle);
	m_TitleLogoHandle = -1;
	DeleteGraph(m_ButtonHandle);
	m_ButtonHandle = -1;
}