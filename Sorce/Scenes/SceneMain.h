#pragma once
#include <DxLib.h>
#include <vector>
#include <memory>
#include <array>
#include "Scene.h"

class Player;
class Goal;

class SceneMain :public Scene
{
public:
	SceneMain();
	~SceneMain();
	void Init();
	void Update();
	void Draw();

	void InitStage(int stageNumber);

	bool IsEnd()const override { return m_isEnd; }
	Scene* GetNextScene() override;

private:

	enum class GameState
	{
		Level,
		Ready,
		Go,
		LevelClear,
		GameOver,
		GameClear
	};

	GameState m_gameState = GameState::Level;

	// 画面外にプレイヤーが出るとゲームオーバー
	void CheckGameOver();
	// プレイヤーのゴールチェック
	void CheckGameClear();

	// 背景のハンドル
	int m_backgroundHandle;

	// UI系
	static constexpr int kMaxLevel = 5;
	std::array<int, kMaxLevel> m_levelHandles;

	int m_readyHandle;
	int m_goHandle;
	int m_gameOverHandle;
	int m_gameClearHandle;
	int m_yaruyanHandle;

	// SE系
	std::array<int, kMaxLevel> m_levelSEHandle;

	bool m_isLevelSEPlayer = false;

	int m_rocketSEHandle;

	int m_readySEHandle;
	int m_goSEHandle;
	int m_gameOverSEHandle;

	// BGMのハンドル
	int m_bgmHandle;
	int m_bombSEHandle;
	int m_overBombSEHandle;

	// 現在のステージ番号
	int m_stageNumber = 1;

	// 点滅アングル
	float m_blinkAngle = 0.0f;

	// シーンが終わったか
	bool m_isEnd = false;

	// ゲームオーバーフラグ
	bool m_isGameOver = false;

	// ステートタイマー
	float m_stateTimer = 0.0f;

	// プレイヤー
	std::unique_ptr<Player> m_player;
	std::unique_ptr<Goal> m_goal;
};

