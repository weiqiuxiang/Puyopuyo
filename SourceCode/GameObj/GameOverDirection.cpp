/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "GameOverDirection.h"
#include "DxLib.h"
#include "DrawMethod.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define BLACK_CURTAIN_ALPHA_MAX_VALUE (150)		//黒い幕のアルファ最大値
#define BLACK_CURTAIN_ALPHA_CHANGE_VALUE (5)	//更新するたびに黒い幕のアルファの変化値
#define BLACK_CURTAIN_TEX_PASS "data/texture/BlackCurtain.png"	//黒い幕のファイルパス

#define FONT_DEFAULT_SIZEX (512)		//フォントの横幅
#define FONT_DEFAULT_SIZEY (128)		//フォントの縦幅
#define FONT_DEFAULT_POSX (SCREEN_WIDTH * 0.5f)			//フォントのX初期位置
#define FONT_DEFAULT_POSY (-SCREEN_HEIGHT * 0.1f)		//フォントの初期Y位置
#define FONT_TARGET_POSY (SCREEN_HEIGHT * 0.3f)			//フォント落ちる目標Y位置
#define FONT_MAX_SIZEX (768)				//フォントは拡大時の最大サイズ
#define FONT_MAX_SIZEY (192)				//フォントは拡大時の最大サイズ
#define FONT_SIZEX_CHANGE_VALUE (32)	//フォント横方向サイズの変化量
#define FONT_SIZEY_CHANGE_VALUE (8)		//フォント縦方向サイズの変化量
#define FONT_GRAVITY (1)				//フォントの疑似重力
#define FONT_TEX_PASS "data/texture/GameOverFont.png"	//フォントのファイルパス

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
GameOverDirection::GameOverDirection()
{
	m_bUse = false;
	m_Step = GAME_OVER_DIRECTION_BLACK_CURTAIN;

	m_BlackCurtainHandle = -1;
	m_BlackCurtainAlpha = 0;

	m_FontHandle = -1;
	m_FontSize = { FONT_DEFAULT_SIZEX , FONT_DEFAULT_SIZEY };
	m_FontPos = { (int)FONT_DEFAULT_POSX ,(int)FONT_DEFAULT_POSY };
	m_FontDropSpeed = 0;
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
GameOverDirection::~GameOverDirection()
{
	
}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool GameOverDirection::Init(void)
{
	m_bUse = false;
	m_Step = GAME_OVER_DIRECTION_BLACK_CURTAIN;

	//黒い幕部分初期化
	m_BlackCurtainAlpha = 0;
	DeleteGraph(m_BlackCurtainHandle);
	m_BlackCurtainHandle = LoadGraph(BLACK_CURTAIN_TEX_PASS);
	if (m_BlackCurtainHandle == -1) 
	{
		MessageBox(nullptr,"黒い幕のテクスチャは読み込み失敗","error",MB_ICONHAND);
		return false;
	}

	//フォント部分初期化
	m_FontSize = { (int)FONT_DEFAULT_SIZEX , (int)FONT_DEFAULT_SIZEY };
	m_FontPos = { (int)FONT_DEFAULT_POSX ,(int)FONT_DEFAULT_POSY };
	m_FontDropSpeed = 0;
	DeleteGraph(m_FontHandle);
	m_FontHandle = LoadGraph(FONT_TEX_PASS);
	if (m_FontHandle == -1)
	{
		MessageBox(nullptr, "フォントのテクスチャは読み込み失敗", "error", MB_ICONHAND);
		return false;
	}

	return true;
}

/*==========================================================================================
	更新処理
==========================================================================================*/
void GameOverDirection::Update(void)
{
	if (m_bUse == false)
	{
		return;
	}

	switch (m_Step)
	{
	case GAME_OVER_DIRECTION_BLACK_CURTAIN:
		m_BlackCurtainAlpha += BLACK_CURTAIN_ALPHA_CHANGE_VALUE;
		if (m_BlackCurtainAlpha >= BLACK_CURTAIN_ALPHA_MAX_VALUE)	//目標アルファ値に着いたら
		{
			m_BlackCurtainAlpha = BLACK_CURTAIN_ALPHA_MAX_VALUE;
			m_Step = GAME_OVER_DIRECTION_FONTDROP;		//次のステップに移行
		}
		break;
	case GAME_OVER_DIRECTION_FONTDROP:
		m_FontPos.y += m_FontDropSpeed;
		m_FontDropSpeed += FONT_GRAVITY;
		if (m_FontPos.y >= FONT_TARGET_POSY)		//落ちる場所に着いたら、次のステップに移行
		{
			m_FontPos.y = (int)FONT_TARGET_POSY;
			m_Step = GAME_OVER_DIRECTION_GUMDOWN;
		}
		break;
	case GAME_OVER_DIRECTION_GUMDOWN:
		m_FontSize.x += FONT_SIZEX_CHANGE_VALUE;
		m_FontSize.y += FONT_SIZEY_CHANGE_VALUE;
		if (m_FontSize.y >= FONT_MAX_SIZEY)			//フォントの拡大量に達したら次のステップに行く
		{
			m_FontSize.y = FONT_MAX_SIZEX;
			m_FontSize.y = FONT_MAX_SIZEY;
			m_Step = GAME_OVER_DIRECTION_GUMUP;
		}
		break;
	case GAME_OVER_DIRECTION_GUMUP:
		m_FontSize.x -= FONT_SIZEX_CHANGE_VALUE;
		m_FontSize.y -= FONT_SIZEY_CHANGE_VALUE;
		if (m_FontSize.y <= FONT_DEFAULT_SIZEY)		//フォントは元のサイズに戻ったら次のステップに行く
		{
			m_FontSize.x = FONT_DEFAULT_SIZEX;
			m_FontSize.y = FONT_DEFAULT_SIZEY;
			m_Step = GAME_OVER_DIRECTION_OVER;
		}
		break;
	case GAME_OVER_DIRECTION_OVER:
		break;
	}
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void GameOverDirection::Draw(void)
{
	if (m_bUse == false)
	{
		return;
	}

	//黒い幕描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_BlackCurtainAlpha);	//ブレンドモードオン
	DrawMethod::DrawRect2D({ 0,0 }, {SCREEN_WIDTH,SCREEN_HEIGHT}, m_BlackCurtainHandle);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);					//ブレンドモードをオフ

	//フォント描画
	DrawMethod::DrawRect2DCenter(m_FontPos, m_FontSize, m_FontHandle);
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void GameOverDirection::Uninit(void)
{
	DeleteGraph(m_BlackCurtainHandle);
	DeleteGraph(m_FontHandle);
}