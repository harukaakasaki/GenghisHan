#pragma once
#include "Scene.h"
class SceneController
{
public:
	void Init();
	void Update();
	void Draw();

private:
	Scene* m_pScene = nullptr;

	bool m_isFade = false;
	bool m_isFadeOut = true;
	int m_fadeAlpha = 0;
	Scene* m_pNextScene = nullptr;
};

