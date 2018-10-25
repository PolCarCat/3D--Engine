#pragma once
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include <list>

class GameObject;

class QuadtreeNode
{
public:
	QuadtreeNode();
	~QuadtreeNode();
	void AddObject(GameObject* o);
	void RemoveObject(GameObject* o);
	bool CheckBucketSize(uint s);
	void ResetBB();
	void Draw();

private:
	QuadtreeNode * childs[4] = { nullptr };
	QuadtreeNode* parent = nullptr;
	std::list<GameObject*> objects;
	AABB bBox;
};


class Quadtree
{
public:
	Quadtree();
	~Quadtree();

	void AddObject(GameObject* o);
	void RemoveObject(GameObject* o);
	void CreateRoot();
	void CheckStaticObj();
	void Draw();
	

private:
	uint bucketSize = 0;
	QuadtreeNode* root = nullptr;

};

