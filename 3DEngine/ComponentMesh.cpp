#include "ComponentMesh.h"



ComponentMesh::ComponentMesh()
{
	type = MESH;
}


ComponentMesh::~ComponentMesh()
{
}


bool ComponentMesh::Start()
{
	return true;
}

bool ComponentMesh::Update()
{
	mesh.Draw();
	return true;
}
