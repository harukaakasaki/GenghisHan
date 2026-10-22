#include "Goal.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	constexpr float kGoalZ = 0.0f; // ゴールのZ座標
}

Goal::Goal():
	m_pos(),
	m_radius(30.0f)
{
}

Goal::~Goal()
{
}

void Goal::Init(float x, float minY, float maxY, float radius)
{
	m_pos.x = x;
	m_radius = radius;
	
	// Y座標をランダムにする
	int randY = static_cast<int>(minY - maxY);
	if (randY > 0)
	{
		m_pos.y = minY + static_cast<float>(GetRand(randY));
	}
	else
	{
		m_pos.y = minY; // minYとmaxYが同じ場合はminYを使用
	}
}

void Goal::Update()
{
	// 点滅
	m_blinkAngle += 4.5f;
}

void Goal::Draw()
{
	VECTOR worldPos = VGet(m_pos.x, m_pos.y, kGoalZ);

	// ゴールを点滅させる
	int alpha = static_cast<int>((sinf(m_blinkAngle) * 0.5f + 0.5f) * 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawSphere3D(
		worldPos,
		50,
		8,
		GetColor(0, 255, 0),
		GetColor(0, 0, 0),
		false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	VECTOR screenPos = ConvWorldPosToScreenPos(worldPos);
	if (screenPos.z >= 0.0f && screenPos.z <= 1.0f)
	{
		DrawCircle(
			static_cast<int>(screenPos.x),
			static_cast<int>(screenPos.y),
			static_cast<int>(m_radius), 0xff0000, FALSE);
	}
#endif // _DEBUG
}

bool Goal::isCheckHit(const Vec2& playerPos, float playerRadius, const Vec2& goalPos) const
{
	// 2点間距離の2乗を計算
	float dx = playerPos.x - goalPos.x;
	float dy = playerPos.y - goalPos.y;
	float distance = dx * dx + dy * dy;

	// 互いの半径の合計の2乗を計算
	float radiusSum = playerRadius + m_radius;
	float radiusSumSquared = radiusSum * radiusSum;

	return distance <= radiusSumSquared;
}
