#pragma once
//camera.h
#include"Object3D.h"

class Camera : public Object3D
{
public:
	Camera();
	~Camera();

	void Update() override;
	void Draw() override;
private:
	VECTOR3 lookPosition;

	int viewType = 0; //視点切替の番号

	
	float changeTime;
	VECTOR3 changePosStart;//視点移動の開始点
	VECTOR3 changePosGoal;//視点移動の終了点
	VECTOR3 changeLookStart;//視点移動の開始点
	VECTOR3 changeLookGoal;//視点移動の終了点
};
