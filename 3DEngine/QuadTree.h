#pragma once
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include <list>

class GameObject;

class QuadtreeNode
{
public:
	QuadtreeNode(QuadtreeNode* parent = nullptr);
	~QuadtreeNode();
	void AddObject(GameObject* o);
	void RemoveObject(GameObject* o);
	bool CheckBucketSize(uint s);
	void ResetBB();
	void SetBB(AABB b);
	void Draw();
	void GenerateChilds();

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
	void CheckStaticObj();
	void Draw();
	

private:
	uint bucketSize = 0;
	QuadtreeNode* root = nullptr;

};

