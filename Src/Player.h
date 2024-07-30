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
	/// �L�[�o�C���h��ݒ肷�邽�߂̊֐�
	/// </summary>
	/// <param name="�O�Ɉړ����邽�߂̃L�[�ݒ�"></param>
	/// <param name="���Ɉړ����邽�߂̃L�[�ݒ�"></param>
	/// <param name="�E�Ɉړ����邽�߂̃L�[�ݒ�"></param>
	/// <param name="���Ɉړ����邽�߂̃L�[�ݒ�"></param>
	/// <param name="�W�����v���邽�߂̃L�[�ݒ�"></param>
	
	void SetKeyBind(DWORD inputForword, DWORD inputBack, DWORD inputRight, DWORD inputLeft, DWORD inputJump );
	SphereCollider Collider() override;
	

private:
	float speedY; // Y�̑��x

	Animator* animator; // ���i�̃C���X�^���X���|�C���^�[�^�ŏ���
	enum AnimID {
		aIdle = 0,
		aRun = 1,
		aAttack1,
		aAttack2,
		aAttack3,

	};

	//�PP�QP�ő���̓��͂�ς��邽�߂̕ϐ�
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
	

	int frame; // �A�j���[�V�����̃t���[���𐔂���
};

