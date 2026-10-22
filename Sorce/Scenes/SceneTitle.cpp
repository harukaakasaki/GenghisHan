#include "SceneTitle.h"
#include "../Systems/Game.h"
#include "SceneMain.h"
#include <DxLib.h>

namespace
{
	constexpr int kBGMVol = 100; // ゲームシーンのBGMの大きさ
}

SceneTitle::SceneTitle() :
	m_isEnd(false),
	m_titleHandle(-1),
	m_backgroundHandle(-1),
	m_bgmHandle(-1),
	m_inputStep(0),
	m_sceneType(TitleNextScene::None)
{
}

SceneTitle::~SceneTitle()
{
	StopSoundMem(m_bgmHandle); // BGMの停止
	DeleteSoundMem(m_bgmHandle); // BGMの解放
}

void SceneTitle::Init()
{
	m_titleHandle = LoadGraph(L"data/UI/moon_title.png"); // タイトル画像の読み込み
	m_backgroundHandle = LoadGraph(L"data/UI/background.png"); // 背景画像の読み込み
	// BGM
	m_bgmHandle = LoadSoundMem(L"data/BGM/TitleBGM.mp3");

	ChangeVolumeSoundMem(kBGMVol, m_bgmHandle);
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);

	m_inputStep = 0;
	m_sceneType = TitleNextScene::None;
}

void SceneTitle::Update()
{
	static char prevKeys[256] = { 0 };
	char currentKeys[256];
	GetHitKeyStateAll(currentKeys);

	auto IsKeyTriggerd = [&](int keyInput)
		{
			return (currentKeys[keyInput] && !prevKeys[keyInput]);
		};

	// タイトルシーンの更新処理
	if(IsKeyTriggerd(KEY_INPUT_RETURN))// Enterキーが押されたら
	{
		m_sceneType = TitleNextScene::Main;
		m_isEnd = true; // シーン終了フラグを立てる
	}

	// 点滅
	m_blinkAngle += 0.2f;
}

void SceneTitle::Draw()
{
	// タイトルシーンの描画処理
	DrawGraph(0, 0, m_titleHandle, TRUE);

	// スタートボタンを点滅させる
	int alpha = (sinf(m_blinkAngle) >0.0f) ? 255:0;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawGraph(0, 0, m_backgroundHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
#ifdef _DEBUG
	DrawString(100, 100, L"Title Scene", GetColor(255, 255, 255));
	DrawString(100, 150, L"Press Enter to Start", GetColor(255, 255, 255));
#endif // _DEBUG
}

Scene* SceneTitle::GetNextScene()
{
	return new SceneMain();// 次のシーンへ移行
}