/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Gui.h"
#include "DxLib.h"
#include <string.h>

/*==========================================================================================
	マクロ
==========================================================================================*/
#define DEFAULT_BUTTON_POSX (100)
#define DEFAULT_BUTTON_POSY (100)
#define DEFAULT_BUTTON_WIDTH (100)
#define DEFAULT_BUTTON_HEIGHT (100)

#define BUTTON_HOVER_BLEND_TEX "data/texture/UIHoverBlending.jpg"
#define BUTTON_CLICK_BLEND_TEX "data/texture/UIClickBlending.jpg"
#define DEFAULT_BUTTON_FILEPASS "data/texture/DefaultBotton.png"

/*==========================================================================================
	static変数
==========================================================================================*/
Vector2	Gui::m_ButtonPos = { DEFAULT_BUTTON_POSX ,DEFAULT_BUTTON_POSY };
bool	Gui::m_SetPosFlag = false;
Vector2	Gui::m_ButtonSize = { DEFAULT_BUTTON_WIDTH ,DEFAULT_BUTTON_HEIGHT };
bool	Gui::m_SetSizeFlag = false;
int		Gui::m_HoverHandle = -1;
int 	Gui::m_ClickHandle = -1;
std::vector<GuiButton*> Gui::m_ButtonList;

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool Gui::Init(void)
{
	//ボタンリストクリア
	for (auto it = m_ButtonList.begin(); it != m_ButtonList.end(); ++it) 
	{
		if((*it) == nullptr)
		{
			continue;
		}
		(*it)->Uninit();
		delete (*it);
	}
	m_ButtonList.clear();		//クリア

	//加算ブレンド用画像の読み込み
	m_HoverHandle = LoadGraph(BUTTON_HOVER_BLEND_TEX);
	m_ClickHandle = LoadGraph(BUTTON_CLICK_BLEND_TEX);

	return true;
}

/*==========================================================================================
	更新処理
==========================================================================================*/
void Gui::Update(void)
{
	Vector2 MousePos;
	int GetResult = GetMousePoint(&MousePos.x, &MousePos.y);

	//ボタンリストクリア
	for (auto it = m_ButtonList.begin(); it != m_ButtonList.end(); ++it)
	{
		if ((*it) == nullptr)
		{
			continue;
		}
		(*it)->Update(MousePos);
	}
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void Gui::Draw(void)
{
	//ボタンリストクリア
	for (auto it = m_ButtonList.begin(); it != m_ButtonList.end(); ++it)
	{
		if ((*it) == nullptr)
		{
			continue;
		}
		(*it)->Draw(m_HoverHandle, m_ClickHandle);
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void Gui::Uninit(void)
{
	//ボタンリストクリア
	for (auto it = m_ButtonList.begin(); it != m_ButtonList.end(); ++it)
	{
		if ((*it) == nullptr)
		{
			continue;
		}
		(*it)->Uninit();
		delete (*it);
	}
	m_ButtonList.clear();		//クリア
}

//ボタン位置
void Gui::SetButtonPos(const Vector2& Pos)
{
	m_ButtonPos = Pos;
	m_SetPosFlag = true;
}

//ボタンサイズ
void Gui::SetButtonSize(const Vector2& Size)
{
	m_ButtonSize = Size;
	m_SetSizeFlag = true;
}

/*==========================================================================================
	ボタンを生成/呼び出す
==========================================================================================*/
bool Gui::Button(const char* Name, int GraphHandle)
{
	//ボタンが存在するフラグを定義
	bool ButtonExist = false;
	GuiButton* pButton = nullptr;

	//このボタンがすでに存在したかを確認
	for(auto it = m_ButtonList.begin(); it != m_ButtonList.end(); ++it) 
	{
		//文字列比較
		int Result = strcmp(Name, (*it)->GetButtonName() );

		//ボタンが存在する
		if (Result == 0) 
		{
			ButtonExist = true;		//存在フラグ立ち
			pButton = (*it);		//ポインタ保存
			break;
		}
	}

	//ボタンが存在する場合
	if (ButtonExist) 
	{
		Vector2 Pos, Size;
		//位置設定フラグはtrue
		if (m_SetPosFlag) 
		{
			Pos = m_ButtonPos;
			m_SetPosFlag = false;
		}
		else
		{
			Pos = { DEFAULT_BUTTON_POSX ,DEFAULT_BUTTON_POSY };
		}

		//サイズ設定フラグはtrue
		if (m_SetSizeFlag)
		{
			Size = m_ButtonSize;
			m_SetSizeFlag = false;
		}
		else
		{
			Size = { DEFAULT_BUTTON_WIDTH ,DEFAULT_BUTTON_HEIGHT };
		}

		//グラフィックハンドル設定
		int ButtonGraphHandle = -1;
		if (GraphHandle == -1)
		{
			ButtonGraphHandle = LoadGraph(DEFAULT_BUTTON_FILEPASS);
		}
		else
		{
			ButtonGraphHandle = GraphHandle;
		}

		pButton->SetPos(Pos);						//位置設定
		pButton->SetSize(Size);						//サイズ設定
		pButton->SetGraphHandle(ButtonGraphHandle);	//グラフィックハンドル設定
	}
	else
	{
		Vector2 Pos,Size;
		//位置設定フラグはtrue
		if (m_SetPosFlag)
		{
			Pos = m_ButtonPos;
			m_SetPosFlag = false;
		}
		else 
		{
			Pos = { DEFAULT_BUTTON_POSX ,DEFAULT_BUTTON_POSY };
		}

		//サイズ設定フラグはtrue
		if (m_SetSizeFlag)
		{
			Size = m_ButtonSize;
			m_SetSizeFlag = false;
		}
		else 
		{
			Size = { DEFAULT_BUTTON_WIDTH ,DEFAULT_BUTTON_HEIGHT };
		}
		
		//グラフィックハンドル設定
		int ButtonGraphHandle = -1;
		if (GraphHandle == -1) 
		{
			ButtonGraphHandle = LoadGraph(DEFAULT_BUTTON_FILEPASS);
		}
		else 
		{
			ButtonGraphHandle = GraphHandle;
		}

		//新しいボタン作る
		pButton = new GuiButton(Name,Pos, Size, ButtonGraphHandle);
		m_ButtonList.push_back(pButton);
	}

	pButton->SetUseFlag(true);			//ボタン使用
	if (pButton->GetState() != BUTTON_CLICK &&
		pButton->GetStateOld() == BUTTON_CLICK) {
		return true;	//クリックされたら
	}
	return false;
}