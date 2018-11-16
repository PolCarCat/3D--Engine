#ifndef __QUADTREE_H__
#define __QUADTREE_H__


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
	void GenerateChildren();
	void CollectIntersections(std::vector<GameObject*> &objects, const AABB b);
	void DistributeNode(uint buckedSize);
	void Insert(GameObject* obj, uint buckedSize);
	void GenerateTestChildren();
	bool CheckIfChildNeeded(GameObject* obj);

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
	void Draw();
	void DistributeTree();
	void GenerateTestChildren();
	void TEST() 
	{
		AABB b;
		b.minPoint = { -5, 0, -5 };
		b.maxPoint = { 5, 5, 5 };
		root->SetBB(b);
	}
	

private:
	uint bucketSize = 0;
	QuadtreeNode* root = nullptr;
	uint cap = 0;

};

#endif // !__QUADTREE_H__