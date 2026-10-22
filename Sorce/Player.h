#pragma once
#include "../Systems/Vec2.h"
#include "StageData.h"
class StageParameters;

class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw();

	// ステージによっての重力を設定する関数
	void SetGravity(float gravity) { m_gravity = gravity; }

	void ApplyStageParameters(const StageParameters& params)
	{
		m_gravity = params.gravity;
		m_jumpPower = params.jumpPower;
	}

	Vec2 GetPos()const { return m_pos; }

private:
	// 重力
	void Gravity();
	// ジャンプ力
	int m_jumpPower;
	// プレイヤーハンドル
	int m_playerHandle;
	// 重力
    float m_gravity;
	// プレイヤーの座標
	Vec2 m_pos;
	// プレイヤーの移動量
	Vec2 m_velocity;
};

