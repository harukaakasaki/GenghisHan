#pragma once
#include "Scene.h"

enum class TitleNextScene
{
	None,
	Main,  // エンターキー
};

class SceneTitle : public Scene
{
public:
	SceneTitle();
	virtual~SceneTitle();
	void Init() override;
	void Update() override;
	void Draw() override;

	bool IsEnd()const override { return m_isEnd; }
	Scene* GetNextScene() override;

private:
	// シーンが終わったか
	bool m_isEnd = false;
	// タイトルハンドル
	int m_titleHandle;
	// タイトル背景のハンドル
	int m_backgroundHandle;
	// BGMのハンドル
	int m_bgmHandle;
	// 点滅アングル
	float m_blinkAngle = 0.0f;

	int m_inputStep = 0;// コマンド用

	TitleNextScene m_sceneType = TitleNextScene::None;// 次のシーンの種類
};

