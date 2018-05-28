/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "ControlHelp.h"
#include "DxLib.h"
#include "Gui.h"
#include "Common.h"
#include "DrawMethod.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define HELP_IMAGE_TEX_PASS "data/texture/help.png"
#define HELP_FONT_TEX_PASS "data/texture/HelpFont.png"

#define BUTTON_SIZEX (128)
#define BUTTON_SIZEY (32)

#define HELP_IMAGE_SIZEX (256)
#define HELP_IMAGE_SIZEY (384)

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
ControlHelp::ControlHelp() 
{
	m_HelpImageHandle = -1;
	m_HelpFontHandle = -1;
	m_ShowHelp = false;
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
ControlHelp::~ControlHelp() 
{

}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool ControlHelp::Init(void) 
{
	m_ShowHelp = false;

	//ヘルプ画像のテクスチャの読み込み
	DeleteGraph(m_HelpImageHandle);
	m_HelpImageHandle = LoadGraph(HELP_IMAGE_TEX_PASS);
	if (m_HelpImageHandle == -1) 
	{
		MessageBox(nullptr, "テクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		ErrorLogFmtAdd("[ControlHelp.cpp][Function:Init],ハンドルm_HelpImageHandle,テクスチャ読み込み失敗");
		return false;
	}

	//ヘルプフォントのテクスチャの読み込み
	DeleteGraph(m_HelpFontHandle);
	m_HelpFontHandle = LoadGraph(HELP_FONT_TEX_PASS);
	if (m_HelpFontHandle == -1)
	{
		MessageBox(nullptr, "テクスチャ読み込み失敗", "WARNING", MB_ICONHAND);
		ErrorLogFmtAdd("[ControlHelp.cpp][Function:Init],ハンドルm_HelpFontHandle,テクスチャ読み込み失敗");
		return false;
	}

	return true;
}

/*==========================================================================================
	更新処理
==========================================================================================*/
void ControlHelp::Update(void) 
{
	//ヘルプの 表示/非表示
	Gui::SetButtonPos({(int)(SCREEN_WIDTH * 0.85f),(int)(SCREEN_HEIGHT * 0.05f)});
	Gui::SetButtonSize({ BUTTON_SIZEX ,BUTTON_SIZEY	});
	if (Gui::Button("HELP_BUTTON", m_HelpFontHandle)) 
	{
		if (m_ShowHelp == false) 
		{
			m_ShowHelp = true;
		}
		else 
		{
			m_ShowHelp = false;
		}
	}
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void ControlHelp::Draw(void) 
{
	//ヘルプ画像描画
	if (m_ShowHelp) 
	{
		Vector2 Pos = { (int)(SCREEN_WIDTH * 0.85f), (int)(SCREEN_HEIGHT * 0.36f) };
		DrawMethod::DrawRect2DCenter( Pos ,{ HELP_IMAGE_SIZEX ,HELP_IMAGE_SIZEY } , m_HelpImageHandle);
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void ControlHelp::Uninit(void)
{
	//読み込んだテクスチャを解放
	DeleteGraph(m_HelpImageHandle);
	m_HelpImageHandle = -1;
	DeleteGraph(m_HelpFontHandle);
	m_HelpFontHandle = -1;
}