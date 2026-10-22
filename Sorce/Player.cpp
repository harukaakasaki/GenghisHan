#include "Player.h"
#include "StageData.h"
#include <DxLib.h>

Player::Player():
	m_jumpPower(0),
	m_pos(),
	m_playerHandle(-1),
	m_velocity(),
	m_gravity(0.98f)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_jumpPower = 2.5f;
	m_pos = Vec2(100.0f, 300.0f);
	m_velocity = Vec2(0.0f, 0.0f);
	m_playerHandle = LoadGraph(L"data/UI/rocket.png");
}

void Player::Update()
{
	m_pos.x += 0.9f;

	m_pos -= m_velocity;

	if(CheckHitKey(KEY_INPUT_SPACE))
	{
		m_velocity.y += m_jumpPower;
	}

	Gravity();

	m_velocity.y *= 1.0f; // 減衰

}

void Player::Draw()
{
	SetUseLighting(false);
	
	DrawRotaGraph(m_pos.x, m_pos.y, 1.0f, 0.0f, m_playerHandle, TRUE);

#ifdef _DEBUG
	DrawCircle(m_pos.x, m_pos.y, 25.0f, 0xff0000, FALSE);
#endif // _DEBUG

	
}

void Player::Gravity()
{
	// ステージによって重力の強さを変えることができるようにするため、重力の強さを変数にする
	m_velocity.y -= m_gravity; // 重力の強さを変数にすることで、ステージによって重力の強さを変えることができる
}
