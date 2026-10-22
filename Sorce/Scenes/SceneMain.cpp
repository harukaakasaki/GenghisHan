#include "SceneMain.h"
#include "SceneTitle.h"
#include "../Systems/Game.h"
#include "../Player.h"
#include "../StageData.h"
#include "../Goal.h"
#include <cmath>
#include <algorithm>

namespace
{
	constexpr float kFloorY = -100.0f;	// 床のY座標
	constexpr float kCeilingY = 350.0f;	// 天井のY座標

	constexpr int kBGMVol = 50;         // ゲームシーンのBGMの大きさ
	constexpr int kVoiceSEVol = 100;    // 声のSEの大きさ
	constexpr int kRocketSEVol = 80;    // rocketSEの大きさ
}

SceneMain::SceneMain():
	m_backgroundHandle(-1),
	m_blinkAngle(0.0f),
	m_isEnd(false),
	m_isGameOver(false),
	m_stageNumber(1),
	m_bgmHandle(-1),
	m_bombSEHandle(-1),
	m_readySEHandle(-1),
	m_goSEHandle(-1),
	m_gameOverSEHandle(-1),
	m_isLevelSEPlayer(false),
	m_rocketSEHandle(-1),
	m_overBombSEHandle(-1)
{
	m_levelHandles.fill(-1);
	m_levelSEHandle.fill(-1);

	m_backgroundHandle = LoadGraph(L"data/UI/background_2.png"); // 背景画像の読み込み

	for (int i = 0; i < kMaxLevel; i++)
	{
		wchar_t filePath[64];
		swprintf_s(filePath, L"data/UI/Level%d.png", i + 1);
		m_levelHandles[i] = LoadGraph(filePath);
	}
	for (int i = 0; i < kMaxLevel; i++)
	{
		wchar_t filePath[64];
		swprintf_s(filePath, L"data/SE/%d.mp3", i + 1);
		m_levelSEHandle[i] = LoadSoundMem(filePath);
	}

	m_readyHandle = LoadGraph(L"data/UI/Ready.png");
	m_goHandle = LoadGraph(L"data/UI/Go.png");
	m_gameOverHandle = LoadGraph(L"data/UI/GameOver.png");
	m_gameClearHandle = LoadGraph(L"data/UI/GameClear.png");
	m_yaruyanHandle = LoadGraph(L"data/UI/yaruyan.png");

	m_player = std::make_unique<Player>();
	m_goal = std::make_unique<Goal>();

}

SceneMain::~SceneMain()
{
	StopSoundMem(m_bgmHandle); // BGMの停止
	DeleteSoundMem(m_bgmHandle); // BGMの解放
	StopSoundMem(m_bombSEHandle); // SEの停止
	DeleteSoundMem(m_bombSEHandle); // SEの解放
	StopSoundMem(m_overBombSEHandle); // SEの停止
	DeleteSoundMem(m_overBombSEHandle); // SEの解放
	StopSoundMem(m_readySEHandle); // SEの停止
	DeleteSoundMem(m_readySEHandle); // SEの解放
	StopSoundMem(m_goSEHandle); // SEの停止
	DeleteSoundMem(m_goSEHandle); // SEの解放
	StopSoundMem(m_gameOverSEHandle); // SEの停止
	DeleteSoundMem(m_gameOverSEHandle); // SEの解放
	StopSoundMem(m_rocketSEHandle); // SEの停止
	DeleteSoundMem(m_rocketSEHandle); // SEの解放

	for (int i = 0; i < kMaxLevel; i++)
	{
		StopSoundMem(m_levelSEHandle[i]);
		DeleteSoundMem(m_levelSEHandle[i]);
	}
}

void SceneMain::Init()
{
	// BGM
	m_bgmHandle = LoadSoundMem(L"data/BGM/GameBGM.mp3");
	m_bombSEHandle = LoadSoundMem(L"data/SE/Bomb.mp3");
	m_overBombSEHandle = LoadSoundMem(L"data/SE/overBomb.mp3");
	m_rocketSEHandle = LoadSoundMem(L"data/SE/rocketSE_3.mp3");
	

	m_readySEHandle = LoadSoundMem(L"data/SE/Ready.mp3");
	m_goSEHandle = LoadSoundMem(L"data/SE/Go.mp3");
	m_gameOverSEHandle = LoadSoundMem(L"data/SE/GameOver.mp3");

	InitStage(m_stageNumber);

	m_player->Init();
	m_goal->Init(1100.0f, kFloorY + 400.0f, kCeilingY - 400.0f, 30.0f);

	SetupCamera_Ortho(720.0f);

	SetCameraPositionAndTarget_UpVecY(VGet(600.0f, 360.0f, -600.0f), VGet(640.0f, 360.0f, 0.0f));
}

void SceneMain::Update()
{
	switch (m_gameState)
	{
	case GameState::Level:
		m_stateTimer--;

		if (!m_isLevelSEPlayer)
		{
			if (m_stageNumber >= 1 && m_stageNumber <= kMaxLevel)
			{
				int index = m_stageNumber - 1;
				ChangeVolumeSoundMem(kVoiceSEVol, m_levelSEHandle[index]);
				PlaySoundMem(m_levelSEHandle[index], DX_PLAYTYPE_BACK, TRUE);
			}
			m_isLevelSEPlayer = true;
		}

		if (CheckSoundMem(m_levelSEHandle[m_stageNumber - 1]) == 1)
		{
			break;
		}

		m_stateTimer--;

		if (m_stateTimer <= 0)
		{
			m_gameState = GameState::Ready;
			m_stateTimer =120.0f; // 2秒間の準備時間
			ChangeVolumeSoundMem(kVoiceSEVol, m_readySEHandle);
			PlaySoundMem(m_readySEHandle, DX_PLAYTYPE_BACK, TRUE);
		}
		break;

	case GameState::Ready:
		m_stateTimer--;
		if (m_stateTimer <= 0)
		{
			m_gameState = GameState::Go;
			m_stateTimer = 120.0f; // 2秒間のGO表示
			ChangeVolumeSoundMem(kVoiceSEVol, m_goSEHandle);
			PlaySoundMem(m_goSEHandle, DX_PLAYTYPE_BACK, TRUE);
			ChangeVolumeSoundMem(kBGMVol, m_bgmHandle);
			PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP,TRUE);
		}
		break;

	case GameState::Go:
	{
		if (m_stateTimer > 0.0f)
		{
			m_stateTimer--;
		}
		m_player->Update();
		m_goal->Update();

		float playerY = m_player->GetPos().y;

		float totalRange = kCeilingY - kFloorY;
		float currentRange = playerY - kFloorY;

		float t = std::clamp(currentRange / totalRange, 0.0f, 1.0f);

		constexpr int kMinHz = 22050;
		constexpr int kMaxHz = 44100;

		int targetHz = static_cast<int>(kMinHz + (1.0f - t) * (kMaxHz - kMinHz));

		SetFrequencySoundMem(targetHz, m_rocketSEHandle);

		static int seFrameCounter = 0;
		seFrameCounter++;

		if (seFrameCounter % 5 == 0)
		{
			ChangeVolumeSoundMem(kRocketSEVol, m_rocketSEHandle);
			PlaySoundMem(m_rocketSEHandle, DX_PLAYTYPE_LOOP, TRUE);
		}
		
		CheckGameOver();
		CheckGameClear();
	}	
		break;

	case GameState::LevelClear:
		StopSoundMem(m_bgmHandle); // BGMの停止
		StopSoundMem(m_rocketSEHandle);// ロケットSEの停止

		m_stateTimer--;
		if (m_stateTimer <= 0)
		{
			m_stageNumber++;
			if (m_stageNumber > 5)
			{
				m_gameState = GameState::GameClear;
			}
			else
			{
				m_player->Init();
				m_goal->Init(1100.0f, kFloorY + 400.0f, kCeilingY - 400.0f, 30.0f);
				InitStage(m_stageNumber);

				m_gameState = GameState::Level;
				m_stateTimer = 60.0f; // 1秒間のレベル表示

				m_isLevelSEPlayer = false;
			}
		}
		break;

	case GameState::GameOver:
		StopSoundMem(m_bgmHandle); // BGMの停止
		StopSoundMem(m_rocketSEHandle);// ロケットSEの停止
		m_stateTimer--;
		if (m_stateTimer <= 0)
		{
			m_isEnd = true; // シーン終了フラグを立てる
			m_stateTimer = 60.0f; // 1秒間のゲームオーバー表示
		}
		
		break;

	case GameState::GameClear:

		m_isEnd = true; // シーン終了フラグを立てる
		break;
	}

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_RETURN))// Enterキーが押されたら
	{
		m_isEnd = true; // シーン終了フラグを立てる
	}
#endif // _DEBUG
	
// 点滅
	m_blinkAngle += 4.0f;
	
}

void SceneMain::Draw()
{
	m_player->Draw();
	m_goal->Draw();

#ifdef _DEBUG
	// 文字列の描画
	DrawString(100, 100, L"SceneMain", GetColor(255, 255, 255));
	DrawString(100, 200, L"Enter to SceneTitle", GetColor(255, 255, 255));
#endif // _DEBUG

	switch (m_gameState)
	{
		case GameState::Level:
			if (m_stageNumber >= 1 && m_stageNumber <= kMaxLevel)
			{
				DrawGraph(0, 0, m_levelHandles[m_stageNumber - 1], true);
			}
			break;

		case GameState::Ready:
			DrawGraph(0, 0, m_readyHandle, true);
			break;

		case GameState::Go:
			if (m_stateTimer > 0.0f)
			{
				DrawGraph(0, 0, m_goHandle, true);
			}
			break;

		case GameState::GameOver:
			DrawGraph(0, 0, m_gameOverHandle, true);
			break;

		case GameState::GameClear:
			DrawGraph(0, 0, m_yaruyanHandle, true);
			break;
	}

	// スタートボタンを点滅させる
	int alpha = static_cast<int>((sinf(m_blinkAngle) * 0.5f + 0.5f) * 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawGraph(0, 0, m_backgroundHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneMain::InitStage(int stageNumber)
{
	StageParameters params;

	if (stageNumber == 1)
	{
		params.gravity = 0.98f; // 重力
		params.jumpPower = 2.5f; // ジャンプ力
	}
	else if (stageNumber == 2)
	{
		params.gravity = 1.68f; // 重力
		params.jumpPower = 3.5f; // ジャンプ力
	}
	else if (stageNumber == 3)
	{
		params.gravity = 1.98f; // 重力
		params.jumpPower = 3.5f; // ジャンプ力
	}
	else if (stageNumber == 4)
	{
		params.gravity = 2.38f; // 重力
		params.jumpPower = 4.5f; // ジャンプ力
	}
	else if (stageNumber == 5)
	{
		params.gravity = 2.98f; // 重力
		params.jumpPower = 5.5f; // ジャンプ力
	}

	m_player->ApplyStageParameters(params);
}

Scene* SceneMain::GetNextScene()
{
	return new SceneTitle();// 次のシーンへ移行
}

void SceneMain::CheckGameOver()
{
	Vec2 playerPos = m_player->GetPos();

	if(playerPos.x < 0 || playerPos.x > 1280 || playerPos.y < 0 || playerPos.y > 700)
	{
		m_gameState = GameState::GameOver;

		m_stateTimer = 180.0f;

		ChangeVolumeSoundMem(kVoiceSEVol, m_overBombSEHandle);
		PlaySoundMem(m_overBombSEHandle, DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(kVoiceSEVol, m_gameOverSEHandle);
		PlaySoundMem(m_gameOverSEHandle, DX_PLAYTYPE_BACK, TRUE);
	}
}

void SceneMain::CheckGameClear()
{
	Vec2 playerPos = m_player->GetPos();
	float playerRadius = 25.0f; // プレイヤーの半径

	// ゴールとの当たり判定をチェック
	VECTOR goalWorldPos = VGet(m_goal->GetX(), m_goal->GetY(), 0.0f);

	// ゴールのワールド座標をスクリーン座標に変換
	VECTOR goalScreenPos = ConvWorldPosToScreenPos(goalWorldPos);

	Vec2 goalPos(goalScreenPos.x, goalScreenPos.y);

	if(m_goal->isCheckHit(playerPos,playerRadius,goalPos))
	{
		ChangeVolumeSoundMem(kVoiceSEVol, m_bombSEHandle);
		PlaySoundMem(m_bombSEHandle, DX_PLAYTYPE_BACK, TRUE);
		m_gameState = GameState::LevelClear;
		m_stateTimer = 120.0f; // 1秒間のレベルクリア表示
	}
}
