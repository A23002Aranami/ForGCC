#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Dancer.h"
#include "Door.h"

namespace { // ����cpp�ȊO�ł͎g���Ȃ�
	static const float Gravity = 0.025f; // �d�͉����x(���̒l)
	// C++�̒萔��`�i�^���t���j
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // ��]���x(�x)
	static const float MoveSpeed = 0.1f;
	
};

Player::Player()
{
	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();
	mesh->Load("Data/Player/PlayerChara.mesh");
	mesh->LoadAnimation(aIdle, "Data/Player/Idle.anmx", true);//true�͌J��Ԃ�
	mesh->LoadAnimation(aRun, "Data/Player/Run.anmx", true);
	mesh->LoadAnimation(aAttack1,"Data/Player/attack1.anmx", false);//false�͌J��Ԃ��Ȃ�
	mesh->LoadAnimation(aAttack2,"Data/Player/attack2.anmx", false);
	mesh->LoadAnimation(aAttack3,"Data/Player/attack3.anmx", false);
	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(aRun);
	animator->SetPlaySpeed(1.0f);

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	state = sOnGround;
	speedY = 0;
}

Player::~Player()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
	if (animator != nullptr) {
		delete animator;
		animator = nullptr;
	}
}

void Player::Update()
{
	animator->Update(); // ���t���[���AUpdate���Ă�

	switch (state) {
	case sOnGround:
		UpdateOnGround();
		break;
	case sJump:
		UpdateJump();
		break;
	}
	ImGui::Begin("Sample");
	ImGui::InputInt("State", (int*)(&state));
	ImGui::InputFloat("SP", &speedY);
	ImGui::End();


	// Dancer�ɂ߂荞�܂Ȃ��悤�ɂ���
	// �����̍��W�́Atransform.position
	// Dancer�̍��W��m��
	std::list<Dancer*> dancers = 
			ObjectManager::FindGameObjects<Dancer>();
	// �@�C�e���[�^�[��
	//for (std::list<Dancer*>::iterator itr = dancers.begin();
	//				itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// �A�C�e���[�^�̕ύX
	//for (auto itr = dancers.begin(); itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// �Bfor(:)�ŉ�
	for (Dancer* dancer : dancers) {
	// �ȏ�R�o�[�W����
		SphereCollider tCol = dancer->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
			// �������Ă�
			// �����o��������pushVec
			// �����o�����������߂�
			float pushLen = rSum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVec�̒������P�ɂ���
			transform.position += pushVec * pushLen;
		}
	}

	//�w�肳�ꂽ�^�O�̃I�u�W�F�N�g��T������
	std::list<Object3D*> objs = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
	int size = objs.size();//�v�f�����擾�ł���
#if 0
	//�C�e���[�^��end�Ɠ����ɂȂ�܂ŌJ��Ԃ�
	//for (std::list<Door*>::iterator itr = doors.begin(); itr != doors.end(); itr++)
	for (auto itr = doors.begin(); itr != doors.end(); itr++)
	{
		Door* door = *itr;//�C�e���[�^�̗v�f��door�ɂƂ�

	}
#endif
#if 1
	for (auto obj : objs)
	{
		//�J��Ԃ����тɃX�t�B�A�𐶐����Ă���
		SphereCollider coll;
		coll.center = transform.position + VECTOR3(0,1.0f,0);//���̍��������̂���ɏグ�Ă���
		coll.radius = 0.5f;//���̎��͂��炢�̑傫��
		VECTOR3 push;
		//HitSphereToMesh���I�u�W�F�N�g3D�ɂ���֐��ŁA�I�u�W�F�N�g�̃��b�V���ƃX�t�B�A�̓����蔻������Ă����
		if (obj->HitSphereToMesh (coll, &push))//�h�A�̃|���S�����X�t�B�A�R���C�_�[�ɓ���������
		{
			transform.position += push;
		}
	}
#endif

}

void Player::Draw()
{
	Object3D::Draw();//�p�����̊֐����Ă�
	CSprite spr;
	

	//MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140f);
	MATRIX4X4 mat = transform.matrix();//���[���h���W�ł̌����ƈʒu��mat�ɑ��
	MATRIX4X4 bone = mesh->GetFrameMatrices(34);//�v���C���[�̌��_������̈ʒu���擾���� ��34�͎��̔ԍ�
	//�x�N�g���Ƀv���C���[��Transform�̃}�g���N�X���|���邱�ƂŁA�v���C���[�̈ʒu�Ɉړ�������
	spr.DrawLine3D(VECTOR3(0, 0, 0) * bone * mat, tipPos * bone * mat, RGB(255, 0, 0));
	//�v���C���[����0,0,0�̏ꏊ//�v���C���[����0,3,0�̂Ƃ���
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;
}


void Player::SetKeyBind(DWORD Forword, DWORD Back, DWORD Right, DWORD Left, DWORD Jump)
{
	inputForward = Forword;
	inputBack = Back;
	inputRight = Right;
	inputLeft = Left;
	inputJump = Jump;
}

SphereCollider Player::Collider()
{
	SphereCollider col;
	col.center = transform.position;
	col.radius = 0.5f;
	return col;
}

void Player::UpdateOnGround()
{
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, inputForward)) {//�O�{�^��
		// �O�p�֐��ł��ꍇ
//		position.z += cosf(rotation.y) * 0.1;
//		position.x += sinf(rotation.y) * 0.1;
		// �s��ł��ꍇ
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		VECTOR3 move = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		transform.position += move;
		animator->MergePlay(aRun);
	} else if (GameDevice()->m_pDI->CheckKey(KD_DAT, inputBack)) {//��{�^��
		// �O�p�֐��ł��ꍇ
//		position.z -= cosf(rotation.y) * 0.1;
//		position.x -= sinf(rotation.y) * 0.1;
// 
// 
		// �s��ł��ꍇ
		//top �L�����N�^�[�̏��
		VECTOR3 top = VECTOR3(0, MoveSpeed, 0);
		//forward �L�����N�^�[�̐��ʂ����߂Ă���
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		VECTOR3 move = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		transform.position -= move;
		animator->MergePlay(aRun);
	}
	else {
		animator->MergePlay(aIdle);
	}
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, inputLeft)) {//���{�^��
		transform.rotation.y -= RotationSpeed / 180.0f * XM_PI;
	}
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, inputRight)) {//�E�{�^��
		transform.rotation.y += RotationSpeed / 180.0f * XM_PI;
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG,inputJump)) {//�W�����v�{�^��
		speedY = JumpPower;
		state = sJump;
	}
	
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_R))//�uR�v
	{
		SceneManager::ChangeScene("ResultScene");
	}

}

void Player::UpdateJump()
{
	transform.position.y += speedY;
	speedY -= Gravity;
	if (transform.position.y <= 0) {
		// �W�����v�I��
		transform.position.y = 0;
		state = sOnGround;
	}
}

/*
void Player::UpdateAttack()
{
	
	//�G�ɍU�������Ă�
	std::list<Dancer*> dancers = ObjectManager::FindGameObjects<Dancer>();//�G�I�u�W�F�N�g��T��
	for (Dancer* d : dancers)
	{
		//MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
		VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140f);
		MATRIX4X4 mat = transform.matrix();//���[���h���W�ł̌����ƈʒu��mat�ɑ��
		MATRIX4X4 bone = mesh->GetFrameMatrices(34);//�v���C���[�̌��_������̈ʒu���擾���� ��34�͎��̔ԍ�
		
		//�x�N�g���Ƀv���C���[��Transform�̃}�g���N�X���|���邱�ƂŁA�v���C���[�̈ʒu�Ɉړ�������
						//�v���C���[����0,0,0�̏ꏊ//�v���C���[����0,3,0�̂Ƃ���
		VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
		VECTOR3 end = tipPos * bone * mat;
		
		//��_�Ԃ̐��ƃ��b�V���̓����蔻����擾����
		if (d->HitLineToMesh(start,end))
		{
			//�G�ɍU���𓖂Ă�
			d->AddDamage( 10, transform.position );
		}
	}
	if (animator->Finished())
	{
		state = sOnGround;
	}
}
*/

