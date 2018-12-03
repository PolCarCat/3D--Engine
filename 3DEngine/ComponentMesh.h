#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

class ResMesh;

class ComponentMesh :
	public Component
{
public:
	ComponentMesh(ResMesh* _mesh);
	ComponentMesh();
	~ComponentMesh();
	bool Start();
	bool Update();
	void UpdateUI();
	bool CleanUp();
	void Draw();
	bool Save(JSON_Object* json, JsonDoc* doc);
	bool Load(JSON_Object* json, JsonDoc* doc);
	
	float DistanceToCamera() const;

	bool draw = true;
	bool drawNormals = false;
	ResMesh* mesh;
	ComponentMaterial* material = nullptr;

private:
	void UpdateMatWin();
};


#endif // !__COMPONENTMESH_H__
