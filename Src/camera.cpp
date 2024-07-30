#include "camera.h"
#include"Player.h"

                                     //三人称視点		//真上視点
static const VECTOR3 CameraPos[] = { VECTOR3(0, 2, -5), VECTOR3(0, 10, -0.5) };
static const VECTOR3 LookPos[]   = { VECTOR3(0, 1,  5), VECTOR3(0,  1,    1) };
static const float ChangeTimeLimit = 0.5f;

Camera::Camera()
{
	//Drawを呼ぶ順番を決める関数(数値が小さい程先に処理される？)
	SetDrawOrder(-1000 );
	changeTime = ChangeTimeLimit;//切り替えない
}

Camera::~Camera()
{
}

void Camera::Update()
{	
	//二つの視点を'L'キーによって切り替える
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_L))
	{
		//要素の最初のデータサイズで配列全体を割ることで、要素数を求めることができる
		changePosStart = CameraPos[viewType];
		changeLookStart = LookPos[viewType];
		viewType++;
		if (viewType >= sizeof(CameraPos)/sizeof(CameraPos[0]))
		{
			viewType = 0;
		}
		changePosGoal = CameraPos[viewType];
		changeLookGoal = LookPos[viewType];
		changeTime = 0.0f;

	}
	// プレイヤーの行列を求める
	//プレイヤーオブジェクトを取得
	Player* player = ObjectManager::FindGameObject<Player>();
	//プレイヤーのローテーションを行列に登録
	MATRIX4X4 rotY = XMMatrixRotationY(player->Rotation().y);
	//プレイヤーのポジションを行列に登録
	MATRIX4X4 trans = XMMatrixTranslation(
		player->Position().x, 0.0f, player->Position().z);
	MATRIX4X4 m = rotY * trans;
	// プレイヤーが回転・移動してない時のカメラ位置に
	// プレイヤーの回転・移動行列を掛けると、
	if (changeTime >= ChangeTimeLimit)//視点切替完了時処理
	{
		transform.position = CameraPos[viewType] * m;
		lookPosition = LookPos[viewType] * m;
	}
	else//切替中処理
	{
		changeTime += 1.0f / 60.0f;//60分の1秒加算される
		float timeRate = changeTime / ChangeTimeLimit;//0.0～0.1
		float rate = timeRate*timeRate;
		VECTOR3 position = (changePosGoal - changePosStart) * rate + changePosStart;
		VECTOR3 look = (changeLookGoal - changeLookStart) * rate + changeLookStart;
		transform.position = position*m;
		lookPosition = look*m;
	}

	//カメラがめり込まないようにする処理
	VECTOR3 start = player->Position()+VECTOR3(0,1.5f,0);
	
	VECTOR3 end = transform.position;
	VECTOR3 push = end-start;
	//当たり判定を0.2m分余分にとっている
	push = XMVector3Normalize(push)*(push.Length() + 0.2f);

	std::list<Object3D*> objs = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
	
	for (auto obj : objs) {
		VECTOR3 hit;
		if (obj->HitLineToMesh(start, end, &hit))
		{
			//当たっている際の処理
			//endを壁の場所にする
			end = hit;
		}
		
	}
	//(end-start).Length() = 壁とキャラの頭の距離
	//カメラの位置変更(壁からPLの頭までの距離を求めて、距離から0.2を引いたのを求めて、始点をスタートに合わせる)
	transform.position = XMVector3Normalize(push)*((end-start).Length()-0.2f)+start;
}

void Camera::Draw()
{

	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position,
		lookPosition,
		VECTOR3(0, 1, 0));
}
