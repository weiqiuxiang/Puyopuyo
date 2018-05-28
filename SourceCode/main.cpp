/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "DxLib.h"
#include "Input.h"
#include "SceneManager.h"
#include "FPS.h"
#include "Gui.h"
#include "Common.h"
#include <time.h>

/*==========================================================================================
	マクロ
==========================================================================================*/
#define FRAME_RATE (60)			//フレームレート

/*==========================================================================================
	WINAPIメインループ
==========================================================================================*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//必要変数初期化
	SceneManager sceneManager;
	FPS	*fps = new FPS(FRAME_RATE);
	if (nullptr == fps)
	{
		return -1;	//初期化失敗
	}

	//DXLib初期設定
	ChangeWindowMode(TRUE);	
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16);
	if (DxLib_Init() == -1) {
		return -1;			
	}
	
	//ダブルバッファ設定
	SetDrawScreen(DX_SCREEN_BACK);	

	//乱数の初期値設定
	SRand((unsigned)time(NULL));

	//初期化処理
	if (sceneManager.Init() == false) 
	{
		ErrorLogFmtAdd("[main.cpp][Function:WinMain]sceneManagerの初期化失敗");
		return -1;
	}
	if(Gui::Init() == false)
	{
		ErrorLogFmtAdd("[main.cpp][Function:WinMain]Guiの初期化失敗");
		return -1;
	}

	 // スタートタイム
    int FrameStartTime = GetNowCount() ;

	//ゲームループ
	while (1)
	{
		//ウィンドウメッセージが来る時
		if (ProcessMessage() == -1)
		{
			break;
		}
		//ウィンドウメッセージが来ないならゲームループを実行する
		else
		{
			int TimeNow = GetNowCount();				//現在時刻取得
			int TookTime = TimeNow - FrameStartTime;	//差分

			if (TookTime >= (1000.0f / fps->GetSetFrameRate()))
			{
				//フレームスタートタイプを上書き
				FrameStartTime = TimeNow;

				// 画面クリア
				ClearDrawScreen();

				//FPSの更新と描画
				if (nullptr != fps)
				{
					fps->Update(TookTime);	//更新処理
#ifdef _DEBUG
					fps->Draw();			//描画処理
#endif
				}

				//ゲームループ
				sceneManager.Update();
				Gui::Update();
				sceneManager.Draw();
				Gui::Draw();

				// 裏画面の内容を表画面に反映させる
				ScreenFlip();
			}
		}
	}

	//終了処理
	Gui::Uninit();
	sceneManager.Uninit();
	if (nullptr != fps) {
		delete fps;
	}

	//DXライブラリ終了
	DxLib_End();
	return 0;
}