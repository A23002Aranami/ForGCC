#pragma once
#include "Object3D.h"
#include "MeshCollider.h"

class Door : public Object3D {
public:
	Door();
	~Door();
	/// <summary>
	/// 球体と自分との当たり判定をする
	/// 当たっている場合、押し返す向きと量をpushベクトルで返す
	/// </summary>
	/// <param name="sphere">球体コリジョン</param>
	/// <param name="push">押し返すベクトル</param>
	/// <returns>あたってたらtrue</returns>
	bool CollisionSphere(SphereCollider sphere,VECTOR3* push);
private:
	MeshCollider* collider;
};