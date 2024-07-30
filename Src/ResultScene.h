#pragma once
//ResultScene.h
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;
private: // クラスのメンバー変数はprivateにする
	int positionX;
	int speedX;
};