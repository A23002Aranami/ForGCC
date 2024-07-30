#include "Dancer.h"
#include"Player.h"
#include"Score.h"
Dancer::Dancer()
{
	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();
	mesh->Load("Data/Dancer/Character.mesh");
	mesh->LoadAnimation(0, "Data/Dancer/Dancing.anmx", true);
	animator->SetModel(mesh); // このモデルでアニメーションする
	animator->Play(0);
	animator->SetPlaySpeed(1.0f);
	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	transform.position = VECTOR3(20, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	state = sNormal;
	hitPoint = 20;
	aroundID = 0;
}

Dancer::~Dancer()
{
}

void Dancer::Start()//update()が動く前に一度だけ行われる処理
{
	areaCenter = transform.position;
}

void Dancer::Update()
{
	switch (state)
	{
	case sNormal:
		updateNormal();
		break;
	case sReach:
	{
		updateReach();
		break;
	}
	case sBack:
		updateBack();
		break;

	case sDead:
		updateDead();
		break;
	}
	
}


SphereCollider Dancer::Collider()
{
	SphereCollider col;
	col.center = transform.position;
	col.radius = 0.5f;
	return col;
}

void Dancer::AddDamage(float damage, VECTOR3 pos)
{
	hitPoint -= damage;
	if (hitPoint > 0) {
		//自分から敵への向きのベクトルを求める
		VECTOR3 pushVec = transform.position - pos;
		pushVec.y = 0;
		//ベクトルの長さを1にする
		pushVec = XMVector3Normalize(pushVec);
		//ベクトルに移動させたい距離を掛けてポジションに加算する
		transform.position += pushVec * 0.5;
		transform.rotation.y = atan2f(-pushVec.x, -pushVec.z);
	}
	else
	{
		//自分から敵へのベクトルを求める
		VECTOR3 pushVec = transform.position - pos;
		//求めたベクトルの逆を向かせる
		transform.rotation.y = atan2f(-pushVec.x, -pushVec.z);
		deadTimer = 0;//死亡してからの時間を0で初期化
		state = sDead;//ステータスを死亡状態に
	}
}

void Dancer::updateNormal()
{
	animator->Update();
	//巡回ルート
	std::vector <VECTOR3> around =
	{
		VECTOR3(0,0,0),
		VECTOR3(0,0,2),
		VECTOR3(2,0,2),
		VECTOR3(0,0,2),
	};

	VECTOR3 target = around[aroundID] + areaCenter;
	moveToTarget(target, 0.01, 1);

	if ((target - transform.position).Length() < 0.5f)
	{
		aroundID = (aroundID + 1) % around.size();
	}
	
	//視野に入ったら、プレイヤーに向かって動く
	Player* player = ObjectManager::FindGameObject<Player>();
	VECTOR3 toPlayer = player->Position() - transform.position;//
	if (toPlayer.Length() < 10.0f)//視野の距離
	{
		//自分の正面ベクトルを作る
		VECTOR3 front = VECTOR3(0, 0, 1) * XMMatrixRotationY(transform.rotation.y);
		//自分の右側ベクトルを作る
		VECTOR3 right = VECTOR3(1, 0, 0) * XMMatrixRotationY(transform.rotation.y);
		//プレイヤーへのベクトルの長さ1のもの(方向だけの情報にする)
		VECTOR3 toPlayer1 = XMVector3Normalize(toPlayer);
		//内積をとる↓これがcosΘになる
		float ip = Dot(front, toPlayer1);//Dot = 内積を二つのベクトルの内積を求める関数
		float ipR = Dot(right, toPlayer1);
		if (ip >= cosf(60.0f * DegToRad))//60度以内(cosθが60以内である時)
		{
			//視野に入った時
			state = sReach;
		}
	}

}

void Dancer::updateReach()
{
	Player* player = ObjectManager::FindGameObject<Player>();
	

	moveToTarget(player->Position());

	//エリアの外に出たら、stateをsBuckに変える
	VECTOR3 toAreaCenter = areaCenter - transform.position;
	if (toAreaCenter.Length() > 5.0f)//エリアを超えたら
	{
		state = sBack;
	}
}

void Dancer::updateBack()
{
	//エリアの中心へのベクトル
	VECTOR3 toAreaCenter = areaCenter - transform.position;

	moveToTarget(areaCenter);
	//エリアセンターとの距離が一定以下になったらsNormalに戻す
	if (toAreaCenter.Length() < 1.0f)
	{
		state = sNormal;
	}


}


void Dancer::updateDead()
{
	deadTimer += SceneManager::DeltaTime();//1/60秒加算

	if (deadTimer <= 2.0f)
	{
		//徐々に倒れる
		float rate = deadTimer/2.0f;
		rate = rate * rate*rate;
		transform.rotation.x = -XM_PI / 2 * rate;
	}
	else if(deadTimer >= 4.0f)
	{
		//死亡フラグをtrueにする関数
		sc = ObjectManager::FindGameObject<Score>();
		sc->AddScore(100);
		DestroyMe();
	}
}

void Dancer::moveToTarget(VECTOR3 target)
{
	moveToTarget(target, 0.01f, 0.5f);
}

void Dancer::moveToTarget(VECTOR3 target, float moveSpeed, float rot)
{
	//角度を変換
	rot = rot * DegToRad;
	MATRIX4X4 myRot = XMMatrixRotationY(transform.rotation.y);
	VECTOR3 toTarget = target - transform.position;//
	//自分の正面ベクトルを作る
	VECTOR3 front = VECTOR3(0, 0, 1) * myRot;
	//自分の右側ベクトルを作る
	VECTOR3 right = VECTOR3(1, 0, 0) * myRot;
	//プレイヤーへのベクトルの長さ1のもの(方向だけの情報にする)
	VECTOR3 toTarget1 = XMVector3Normalize(toTarget);

	//内積をとる↓これがcosΘになる
	float ip = Dot(front, toTarget1);//Dot = 内積を二つのベクトルの内積を求める関数
	float ipR = Dot(right, toTarget1);

	transform.position += front * moveSpeed;

	if (ip >= cosf(3.0f * DegToRad))
	{
		transform.rotation.y = atan2f(toTarget.x, toTarget.z);
	}
	else if (ipR >= 0.0f)//プレイヤーが自分の右にいる時
	{
		transform.rotation.y += rot;
	}
	else//プレイヤーが自分の左にいる時
	{
		transform.rotation.y -= rot;
	}
}
