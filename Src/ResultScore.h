#pragma once
#include "GameObject.h"
#include "Score.h"
class ResultScore : public GameObject
{
public:
	ResultScore();
	~ResultScore();
	void Update()override;
	void Draw()override;
	void GetScore();

private:
	int viewScore;
	float endCntUp;
	float resultTimer;
	float strX ;
	float strY;
	float Row;
	float deray;
};