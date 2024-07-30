#include "Object3D.h"
#include "MeshCollider.h"

Object3D::Object3D()
{
	mesh = nullptr;
	meshCol = nullptr;
}

Object3D::~Object3D()
{
	if (mesh != nullptr) {
		delete mesh;
	}
	if (meshCol != nullptr) {
		delete meshCol;
	}
}

void Object3D::Update()
{
}

void Object3D::Draw()
{
	mesh->Render(transform.matrix());
}

void Object3D::SetPosition(const VECTOR3& pos)
{
	transform.position = pos;
}

void Object3D::SetPosition(float x, float y, float z)
{
	SetPosition(VECTOR3(x, y, z));
}

void Object3D::SetRotation(const VECTOR3& rot)
{
	transform.rotation = rot;
}

void Object3D::SetRotation(float x, float y, float z)
{
	SetRotation(VECTOR3(x, y, z));
}

void Object3D::SetScale(const VECTOR3& sca)
{
	transform.scale = sca;
}

void Object3D::SetScale(float x, float y, float z)
{
	SetScale(VECTOR3(x, y, z));
}

SphereCollider Object3D::Collider()
{
	SphereCollider sphere;
	sphere.center = transform.position + VECTOR3(0, 1, 0);
	sphere.radius = 0.5f;
	return sphere;
}

bool Object3D::HitSphereToMesh(const SphereCollider& sphere, VECTOR3* push)
{
	if (meshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	std::list<MeshCollider::CollInfo> meshes = meshCol->CheckCollisionSphereList(mat, sphere.center, sphere.radius);
	if (meshes.size() == 0)
		return false;
	if (push != nullptr) { // 座標が必要なのであれば
		VECTOR3 pushVec = VECTOR3(0, 0, 0); // 最終的に押すベクトル
		for (const MeshCollider::CollInfo& m : meshes) { // 当たっているすべてのポリゴンで
			VECTOR3 move = sphere.center - m.hitPosition;
			float len = move.Length(); // 当たった点から中心への距離
			move = move * ((sphere.radius - len) / len);
			VECTOR3 push = m.normal * Dot(move, m.normal); // 押し返したいベクトル
			// 今のpushVecと合成する
			VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // 合成済みベクトルの向き
			float dot = Dot(push, pushVecNorm);	// その成分の長さ
			if (dot < pushVec.Length()) {
				pushVec += push - pushVecNorm * dot; // その成分を減らしていい
			}
			else {
				pushVec = push;
			}
		}
		*push = pushVec;
	}
	return true;
}

bool Object3D::HitLineToMesh(const VECTOR3& from, const VECTOR3& to, VECTOR3* hit)
{
	if (meshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	MeshCollider::CollInfo coll;
	bool ret = meshCol->CheckCollisionLine(mat, from, to, &coll);
	if (ret) {
		if (hit != nullptr) {
			*hit = coll.hitPosition;
		}
		return true;
	}
	return false;
}

float Object3D::HitSphereToSphere(const SphereCollider& target, bool withY)
{
	SphereCollider my = Collider();
	VECTOR3 diff = target.center - my.center;
	if (withY == false)
		diff.y = 0.0f;
	float rsum = target.radius + my.radius;
	if (diff.LengthSquare() < rsum * rsum) {
		return rsum - diff.Length();
	}
	return 0.0f;
}
