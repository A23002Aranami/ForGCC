#pragma once
// Player.h
#include "Object3D.h"
#include "Animator.h"

class Player : public Object3D {
public:
	Player();
	~Player();
	void Update() override;
	void Draw()override;

	/// <summary>
	/// キーバインドを設定するための関数
	/// </summary>
	/// <param name="前に移動するためのキー設定"></param>
	/// <param name="後ろに移動するためのキー設定"></param>
	/// <param name="右に移動するためのキー設定"></param>
	/// <param name="左に移動するためのキー設定"></param>
	/// <param name="ジャンプするためのキー設定"></param>
	
	void SetKeyBind(DWORD inputForword, DWORD inputBack, DWORD inputRight, DWORD inputLeft, DWORD inputJump );
	SphereCollider Collider() override;
	

private:
	float speedY; // Yの速度

	Animator* animator; // 部品のインスタンスをポインター型で準備
	enum AnimID {
		aIdle = 0,
		aRun = 1,
		aAttack1,
		aAttack2,
		aAttack3,

	};

	//１P２Pで操作の入力を変えるための変数
	DWORD inputForward;
	DWORD inputBack;
	DWORD inputRight;
	DWORD inputLeft;
	DWORD inputJump;

	enum State {
		sOnGround = 0,
		sJump,
	};
	State state;
	void UpdateOnGround();
	void UpdateJump();
	

	int frame; // アニメーションのフレームを数える
};

