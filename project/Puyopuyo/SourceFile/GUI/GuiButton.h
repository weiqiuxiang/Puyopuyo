/*==========================================================================================
ヘッダファイル説明:
	GUIのボタンのクラスのヘッダ
==========================================================================================*/
#ifndef GUI_BUTTON_H_
#define GUI_OBJ_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include <stdio.h>
#include "Common.h"
#include "DxLib.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define BUTTON_NOTOUCH	(0x00000001)		//ボタン何も変化していない状態
#define BUTTON_HOVER	(0x00000010)		//ボタン
#define BUTTON_CLICK	(0x00000100)

#define DEFAULT_BUTTON_NAME "NO_NAMEING_BUTTON"

/*==========================================================================================
	クラス定義
==========================================================================================*/
class GuiButton
{
public:
	GuiButton();
	GuiButton(const char* Name,const Vector2& Pos, const Vector2& Size, int GraphHandle = -1);
	~GuiButton();

	void Update(const Vector2& MousePos);	//更新処理
	void Draw(const int GraphHandleHover,const int GraphHandleClick);	//描画処理
	void Uninit(void);						//終了処理

	void SetUseFlag(bool Use) { m_Use = Use; }					//フラグ設定
	void SetPos(const Vector2& Pos) { m_Pos = Pos; }			//位置設定
	void SetSize(const Vector2& Size) { m_Size = Size; }		//サイズ設定
	void SetGraphHandle(const int Handle) { m_GraphHandle = Handle; }

	const char* GetButtonName(void) const { return m_ButtonName; }	//ボタンの名前を返す
	unsigned int GetState(void) const { return m_StateFlag; }		//状態取得
	unsigned int GetStateOld(void) const { return m_StateOldFlag; }	//前状態取得
private:
	unsigned int m_StateOldFlag;//前状態フラグ
	unsigned int m_StateFlag;	//状態フラグ
	char	m_ButtonName[256];	//ボタンの名前
	int		m_GraphHandle;		//グラフィックハンドル
	Vector2 m_Pos;				//位置
	Vector2 m_Size;				//サイズ
	bool	m_Use;				//使用フラグ()
};

#endif
