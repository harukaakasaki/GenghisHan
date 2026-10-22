#include <DxLib.h>
#include "../Systems/Game.h"
#include "../Scenes/SceneController.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウモード設定
	ChangeWindowMode(true);
	// ウィンドウのタイトル変更
	SetMainWindowText(L"moon");
	// 画面のサイズ変更
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	{
		// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
		SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
		// Zバッファを有効にする。
		SetUseZBuffer3D(TRUE);
		// Zバッファへの書き込みを有効にする。
		SetWriteZBuffer3D(TRUE);
	}

	SceneController controller;
	controller.Init();

	while (ProcessMessage() != -1)
	{
		LONGLONG start = GetNowHiPerformanceCount();

		// 前のフレーム描画を消す
		ClearDrawScreen();
		//（ゲーム内容）

		controller.Update();
		controller.Draw();

		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// 内容を画面に反映
		ScreenFlip();

		// ゲーム速度がどのPCでも同じスピードになる
		while (GetNowHiPerformanceCount() - start < 16667)
		{

		}
	}

	//WaitKey();				// キー入力待ち
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}