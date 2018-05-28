/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "GuiButton.h"
#include <string.h>
#include "DrawMethod.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define DEFAULT_POSX (100)
#define DEFAULT_POSY (100)
#define DEFAULT_WIDTH (100)
#define DEFAULT_HEIGHT (100)
#define DEFAULT_BUTTON_FILEPASS "data/texture/DefaultBotton.png"

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
GuiButton::GuiButton()
{
	m_StateFlag = BUTTON_NOTOUCH;					//状態フラグ
	m_StateOldFlag = BUTTON_NOTOUCH;				//前状態フラグ
	strcpy_s(m_ButtonName, DEFAULT_BUTTON_NAME);	//ボタンの名前
	m_GraphHandle = LoadGraph(DEFAULT_BUTTON_FILEPASS);	//グラフィックハンドル

	//位置
	m_Pos.x = DEFAULT_POSX;
	m_Pos.y = DEFAULT_POSY;
	//サイズ
	m_Size.x = DEFAULT_WIDTH;
	m_Size.y = DEFAULT_HEIGHT;

	//使用フラグ
	m_Use = false;
}

/*==========================================================================================
	コンストラクタ
	引数説明:
	Name     : ボタンの名前
	Pos      : 位置
	Size     : サイズ
	FilePass : テクスチャファイルパス
==========================================================================================*/
GuiButton::GuiButton(const char* Name, const Vector2& Pos, const Vector2& Size, int GraphHandle)
{
	m_StateFlag = BUTTON_NOTOUCH;					//状態フラグ
	m_StateOldFlag = BUTTON_NOTOUCH;				//前状態フラグ

	//ボタンの名前
	if (nullptr == Name)
	{
		strcpy_s(m_ButtonName, DEFAULT_BUTTON_NAME);
	}
	else
	{
		strcpy_s(m_ButtonName, Name);
	}

	m_GraphHandle = GraphHandle;	//グラフィックハンドル

	//位置
	m_Pos.x = Pos.x;
	m_Pos.y = Pos.y;
	//サイズ
	m_Size.x = Size.x;
	m_Size.y = Size.y;

	//使用フラグ
	m_Use = false;
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
GuiButton::~GuiButton()
{
	
}

/*==========================================================================================
	更新処理
==========================================================================================*/
void GuiButton::Update(const Vector2& MousePos)
{
	//使用フラグ確認
	if (false == m_Use) 
	{
		return;
	}

	m_StateOldFlag = m_StateFlag;				//前状態保存

	//ボタンが押されている状態なら
	if (m_StateFlag == BUTTON_CLICK) 
	{
		//マウス左ボタンが押されている
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			return;			//終了し、状態変化しない
		}
	}

	//マウスとの当たり判定
	bool bHitX = (MousePos.x > m_Pos.x - (int)(m_Size.x * 0.5f)) && (MousePos.x < m_Pos.x + (int)(m_Size.x * 0.5f));
	bool bHitY = (MousePos.y > m_Pos.y - (int)(m_Size.y * 0.5f)) && (MousePos.y < m_Pos.y + (int)(m_Size.y * 0.5f));

	//判定あり
	if (bHitX && bHitY)
	{
		m_StateFlag = BUTTON_HOVER;		//マウスが翳している状態

		//マウス左ボタンが押されている
		if ( (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) 
		{
			m_StateFlag = BUTTON_CLICK;	//マウスに押されている
		}
	}
	else 
	{
		m_StateFlag = BUTTON_NOTOUCH;	//マウスが翳してない状態
		return;
	}
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void GuiButton::Draw(const int GraphHandleHover, const int GraphHandleClick)
{
	//使用フラグ確認
	if (false == m_Use)
	{
		return;
	}

	DrawMethod::DrawRect2DCenter(m_Pos, m_Size, m_GraphHandle);	//ボタン描画

	switch (m_StateFlag)
	{
	case BUTTON_NOTOUCH:	//NoTouch状態
		break;
	case BUTTON_HOVER:		//マウスに翳されている
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);				//ブレンドモードオン
		DrawMethod::DrawRect2DCenter(m_Pos, m_Size, GraphHandleHover);//ブレンドマスク描画
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//ブレンドモードをオフ
		break;
	case BUTTON_CLICK:		//マウスにクリックされている
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);				//ブレンドモードオン
		DrawMethod::DrawRect2DCenter(m_Pos, m_Size, GraphHandleClick);//ブレンドマスク描画
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//ブレンドモードをオフ
		break;
	}

	m_Use = false;
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void GuiButton::Uninit(void)
{
	DeleteGraph(m_GraphHandle);
}