#pragma once
#include "Object3D.h"
#include "MeshCollider.h"

class MochiTest : public Object3D {
public:
	MochiTest();
	~MochiTest();
	private:
	MeshCollider* collider;
};
