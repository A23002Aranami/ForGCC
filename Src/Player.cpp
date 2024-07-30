#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Dancer.h"
#include "Door.h"

namespace { // このcpp以外では使えない
	static const float Gravity = 0.025f; // 重力加速度(正の値)
	// C++の定数定義（型が付く）
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // 回転速度(度)
	static const float MoveSpeed = 0.1f;
	
};

Player::Player()
{
	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();
	mesh->Load("Data/Player/PlayerChara.mesh");
	mesh->LoadAnimation(aIdle, "Data/Player/Idle.anmx", true);//trueは繰り返す
	mesh->LoadAnimation(aRun, "Data/Player/Run.anmx", true);
	mesh->LoadAnimation(aAttack1,"Data/Player/attack1.anmx", false);//falseは繰り返さない
	mesh->LoadAnimation(aAttack2,"Data/Player/attack2.anmx", false);
	mesh->LoadAnimation(aAttack3,"Data/Player/attack3.anmx", false);
	animator->SetModel(mesh); // このモデルでアニメーションする
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
	animator->Update(); // 毎フレーム、Updateを呼ぶ

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


	// Dancerにめり込まないようにする
	// 自分の座標は、transform.position
	// Dancerの座標を知る
	std::list<Dancer*> dancers = 
			ObjectManager::FindGameObjects<Dancer>();
	// ①イテレーター版
	//for (std::list<Dancer*>::iterator itr = dancers.begin();
	//				itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// ②イテレータの変更
	//for (auto itr = dancers.begin(); itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// ③for(:)で回す
	for (Dancer* dancer : dancers) {
	// 以上３バージョン
		SphereCollider tCol = dancer->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
			// 当たってる
			// 押し出す方向はpushVec
			// 押し出す長さを求める
			float pushLen = rSum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVecの長さを１にする
			transform.position += pushVec * pushLen;
		}
	}

	//指定されたタグのオブジェクトを探索する
	std::list<Object3D*> objs = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
	int size = objs.size();//要素数を取得できる
#if 0
	//イテレータがendと同じになるまで繰り返す
	//for (std::list<Door*>::iterator itr = doors.begin(); itr != doors.end(); itr++)
	for (auto itr = doors.begin(); itr != doors.end(); itr++)
	{
		Door* door = *itr;//イテレータの要素をdoorにとる

	}
#endif
#if 1
	for (auto obj : objs)
	{
		//繰り返すたびにスフィアを生成している
		SphereCollider coll;
		coll.center = transform.position + VECTOR3(0,1.0f,0);//腰の高さ分球体を上に上げている
		coll.radius = 0.5f;//腰の周囲くらいの大きさ
		VECTOR3 push;
		//HitSphereToMeshがオブジェクト3Dにある関数で、オブジェクトのメッシュとスフィアの当たり判定をしてくれる
		if (obj->HitSphereToMesh (coll, &push))//ドアのポリゴンがスフィアコライダーに当たった時
		{
			transform.position += push;
		}
	}
#endif

}

void Player::Draw()
{
	Object3D::Draw();//継承元の関数を呼ぶ
	CSprite spr;
	

	//MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140f);
	MATRIX4X4 mat = transform.matrix();//ワールド座標での向きと位置をmatに代入
	MATRIX4X4 bone = mesh->GetFrameMatrices(34);//プレイヤーの原点から手首の位置を取得する ※34は手首の番号
	//ベクトルにプレイヤーのTransformのマトリクスを掛けることで、プレイヤーの位置に移動させる
	spr.DrawLine3D(VECTOR3(0, 0, 0) * bone * mat, tipPos * bone * mat, RGB(255, 0, 0));
	//プレイヤーから0,0,0の場所//プレイヤーから0,3,0のところ
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
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, inputForward)) {//前ボタン
		// 三角関数でやる場合
//		position.z += cosf(rotation.y) * 0.1;
//		position.x += sinf(rotation.y) * 0.1;
		// 行列でやる場合
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		VECTOR3 move = forward * rotY; // キャラの向いてる方への移動量
		transform.position += move;
		animator->MergePlay(aRun);
	} else if (GameDevice()->m_pDI->CheckKey(KD_DAT, inputBack)) {//後ボタン
		// 三角関数でやる場合
//		position.z -= cosf(rotation.y) * 0.1;
//		position.x -= sinf(rotation.y) * 0.1;
// 
// 
		// 行列でやる場合
		//top キャラクターの上面
		VECTOR3 top = VECTOR3(0, MoveSpeed, 0);
		//forward キャラクターの正面を求めている
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		VECTOR3 move = forward * rotY; // キャラの向いてる方への移動量
		transform.position -= move;
		animator->MergePlay(aRun);
	}
	else {
		animator->MergePlay(aIdle);
	}
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, inputLeft)) {//左ボタン
		transform.rotation.y -= RotationSpeed / 180.0f * XM_PI;
	}
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, inputRight)) {//右ボタン
		transform.rotation.y += RotationSpeed / 180.0f * XM_PI;
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG,inputJump)) {//ジャンプボタン
		speedY = JumpPower;
		state = sJump;
	}
	
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_R))//「R」
	{
		SceneManager::ChangeScene("ResultScene");
	}

}

void Player::UpdateJump()
{
	transform.position.y += speedY;
	speedY -= Gravity;
	if (transform.position.y <= 0) {
		// ジャンプ終了
		transform.position.y = 0;
		state = sOnGround;
	}
}

/*
void Player::UpdateAttack()
{
	
	//敵に攻撃をあてる
	std::list<Dancer*> dancers = ObjectManager::FindGameObjects<Dancer>();//敵オブジェクトを探す
	for (Dancer* d : dancers)
	{
		//MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
		VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140f);
		MATRIX4X4 mat = transform.matrix();//ワールド座標での向きと位置をmatに代入
		MATRIX4X4 bone = mesh->GetFrameMatrices(34);//プレイヤーの原点から手首の位置を取得する ※34は手首の番号
		
		//ベクトルにプレイヤーのTransformのマトリクスを掛けることで、プレイヤーの位置に移動させる
						//プレイヤーから0,0,0の場所//プレイヤーから0,3,0のところ
		VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
		VECTOR3 end = tipPos * bone * mat;
		
		//二点間の線とメッシュの当たり判定を取得する
		if (d->HitLineToMesh(start,end))
		{
			//敵に攻撃を当てた
			d->AddDamage( 10, transform.position );
		}
	}
	if (animator->Finished())
	{
		state = sOnGround;
	}
}
*/

