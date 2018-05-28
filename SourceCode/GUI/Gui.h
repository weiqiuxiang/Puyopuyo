#ifndef GUI_H_
#define GUI_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Common.h"
#include "GuiButton.h"
#include <vector>

/*==========================================================================================
	クラス定義
==========================================================================================*/
class Gui 
{
public:
	static bool Init(void);		//初期化処理
	static void Update(void);	//更新処理
	static void Draw(void);		//描画処理
	static void Uninit(void);	//終了処理

	static void SetButtonPos(const Vector2& Pos);	//ボタン位置設定
	static void SetButtonSize(const Vector2& Size);	//ボタンサイズ設定
	static bool Button(const char* Name,int GraphHandle = -1);	//ボタンを生成/呼び出す
private:
	static Vector2	m_ButtonPos;				//SetButtonPosで設定するボタン位置
	static bool		m_SetPosFlag;				//SetButtonPosが呼び出されるとtrue
	static Vector2	m_ButtonSize;				//SetButtonSizeで設定するボタンサイズ
	static bool		m_SetSizeFlag;				//SetButtonSizeが呼び出されるとtrue
	static int		m_HoverHandle;				//マウスはボタンに翳している時の加算ブレンド画像
	static int		m_ClickHandle;				//マウスはボタンにクリック時の加算ブレンド画像
	static std::vector<GuiButton*> m_ButtonList;//ボタンの保存庫
};
#endif