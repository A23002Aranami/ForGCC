#pragma once
#include "Object3D.h"
#include "Animator.h"
#include "Score.h"

class Dancer : public Object3D {
public:
	Dancer();
	~Dancer();
	void Start() override;//最初のアップデートの前に呼ばれる関数
	void Update() override;
	SphereCollider Collider() override;
	void AddDamage(float damage, VECTOR3 pos);
private:
	Animator* animator;
	Score* sc;
	float hitPoint;

	enum State {
		sNormal = 0, //通常状態(巡回)
		sReach,//追いかける
		sBack,//帰る
		sDead,//死亡

	};
	State state;
	void updateNormal();
	void updateReach();
	void updateBack();
	void updateDead();

	float deadTimer;//死亡時間


	VECTOR3 areaCenter;//探索エリアの中心

	/// <summary>
	/// ターゲットへ向かって滑らかに移動する関数
	/// </summary>
	/// <param name="ターゲットの位置"></param>
	void moveToTarget(VECTOR3 target);
	
	/// <summary>
	/// ターゲットへ向かって滑らかに移動する関数
	/// </summary>
	/// <param name="ターゲットの位置"></param>
	/// <param name="動くスピード"></param>
	/// <param name="回転角(Deg)"></param>
	void moveToTarget(VECTOR3 target,float moveSpeed, float rot);
	
	int aroundID;
};