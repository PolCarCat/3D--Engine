#ifndef __QUADTREE_H__
#define __QUADTREE_H__


#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include <list>
#define MAXIUMDIVISIONS 20

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
	void CollectIntersections(std::vector<GameObject*> &objects, math::Frustum frustum);
	void DistributeNode(uint buckedSize, uint& cap);
	void Insert(GameObject* obj, uint buckedSize, uint& cap);
	void GenerateTestChildren();
	bool CheckIfChildNeeded(GameObject* obj);
	void CleanUp();

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
	void CleanUp();
	void Reset();
	

private:
	uint bucketSize = 1;
	QuadtreeNode* root = nullptr;
	uint cap = 0;
	std::list<GameObject*> objects;
};

#endif // !__QUADTREE_H__