#pragma once
#include "GameObject.h"
#include "Score.h"
class ScoreDraw : public GameObject
{
public:
	ScoreDraw();
	~ScoreDraw();
	void Update()override;
	void Draw()override;
	void GetScore();
};