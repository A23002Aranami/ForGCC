#include "Door.h"

Door::Door()
{
	collider = nullptr;
	//タグを設定
	SetTag("STAGEOBJ");
	mesh = new CFbxMesh();
	mesh->Load("Data/MapItem/DOOR1.mesh");
	//collider = new MeshCollider;
	//collider->MakeFromMesh(mesh);//meshから当たり判定情報を作る

	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);

}

Door::~Door()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}

bool Door::CollisionSphere(SphereCollider sphere, VECTOR3* push)
{

	//VECTOR3 pushVec = VECTOR3(0, 0, 0);//最終的に押し返すベクトル

	//std::list < MeshCollider::CollInfo> ret =
	//	collider->CheckCollisionSphereList(matrix, sphere.center, sphere.radius);
	////複数のポリゴンと接触している場合のために拡張for文であたった情報を取得している
	//if(ret.size() > 0){
	//	for(MeshCollider::CollInfo info: ret)
	//	{
	//		//Doorのモデルと当たった?infoに情報が入る
	//		float pushLen = sphere.radius - (info.hitPosition - sphere.center).Length();
	//		//法線方向に食い込んだ分だけ押し戻す
	//		pushVec += info.normal * pushLen;//複数のポリゴンがあるはずなので、その分だけ加算する
	//	}
	//	if (push != nullptr)
	//	{
	//		*push = pushVec;
	//	}
	//	return true;
	//}
	return false;
}
