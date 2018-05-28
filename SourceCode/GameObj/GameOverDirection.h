/*==========================================================================================
ヘッダファイル説明:
	ゲームオーバーの演出
==========================================================================================*/
#ifndef GAME_OVER_DIRECTION_H_
#define GAME_OVER_DIRECTION_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	enumの列挙型宣言
==========================================================================================*/
typedef enum 
{
	GAME_OVER_DIRECTION_BLACK_CURTAIN = 0,		//黒い幕を翳す
	GAME_OVER_DIRECTION_FONTDROP,				//ゲームオーバーフォントが落ちる
	GAME_OVER_DIRECTION_GUMDOWN,				//フォントが縮む
	GAME_OVER_DIRECTION_GUMUP,					//フォントが伸びる
	GAME_OVER_DIRECTION_OVER					//演出終了
}GAME_OVER_DIRECTION_STEP;

/*==========================================================================================
	クラス定義
==========================================================================================*/
class GameOverDirection
{
public:
	GameOverDirection();
	~GameOverDirection();

	bool Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);

	void SetUseFlag(bool Use) { m_bUse = Use; }
	GAME_OVER_DIRECTION_STEP GetStep(void) const { return m_Step; }
private:

private:
	GAME_OVER_DIRECTION_STEP m_Step;	//実行ステップ
	bool	m_bUse;						//使用フラグ(falseの状態ならUpdateとDrawを実行しない)

	int		m_BlackCurtainHandle;		//黒い幕のハンドル
	int		m_BlackCurtainAlpha;		//黒い幕のアルファ値

	int		m_FontHandle;				//フォントのハンドル
	Vector2 m_FontSize;					//フォントサイズ
	Vector2 m_FontPos;					//フォント位置
	int		m_FontDropSpeed;			//フォント落下速度
};

#endif