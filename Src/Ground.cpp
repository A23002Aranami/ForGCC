#include "Ground.h"

Ground::Ground()
{
	//タグをセットする
	SetTag("STAGEOBJ");
	mesh = new CFbxMesh();
	mesh->Load("Data/Map/map50Field1.mesh");
	
	meshCol = new MeshCollider;
	meshCol->MakeFromMesh(mesh);
}

Ground::~Ground()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}
