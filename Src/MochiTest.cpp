#include "MochiTest.h"

MochiTest::MochiTest()
{
	collider = nullptr;
	mesh = new CFbxMesh();
	mesh->Load("Data/Mochi/mochi.mesh");


}

MochiTest::~MochiTest()
{
}
