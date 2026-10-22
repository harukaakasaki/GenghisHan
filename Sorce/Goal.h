#pragma once
#include <DxLib.h>
#include "../Systems/Vec2.h"
class Goal
{
public:
	Goal();
	~Goal();

	void Init(float x, float minY,float maxY,float radius = 30.0f);
	void Update();
	void Draw();

	bool isCheckHit(const Vec2& playerPos, float playerRadius, const Vec2& goalPos)const;

	float GetX()const { return m_pos.x; }
	float GetY()const { return m_pos.y; }
	float GetRadius()const { return m_radius; }

private:
	Vec2 m_pos;
	float m_radius;
	// “_–ÅƒAƒ“ƒOƒ‹
	float m_blinkAngle = 0.0f;
};