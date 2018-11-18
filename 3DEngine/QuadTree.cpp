#include "QuadTree.h"
#include "Application.h"

#include "mmgr/mmgr.h"


QuadtreeNode::QuadtreeNode(QuadtreeNode* _parent)
{

	bBox.minPoint.Set(0, 0, 0);
	bBox.maxPoint.Set(0, 0, 0);
	
	parent = _parent;


}

QuadtreeNode::~QuadtreeNode()
{

}


void QuadtreeNode::AddObject(GameObject* o)
{


	if (bBox.minPoint.Equals(0, 0, 0) && bBox.maxPoint.Equals(0, 0, 0))
		bBox = o->GetGlobalABB();
	else
		bBox.Enclose(o->GetGlobalABB());
	objects.push_back(o);

}

void QuadtreeNode::RemoveObject(GameObject* o)
{
	Utils::RemoveFromList(o, objects);
	ResetBB();
}

bool QuadtreeNode::CheckBucketSize(uint s)
{
	return objects.size() <= s;
}

void QuadtreeNode::Draw()
{
	App->renderer3D->DrawAABB(bBox, { 1.0f, 0.0f, 0.0f , 1.0f });
	for (int i = 0; i < 4; i++)
	{
		if (childs[i] != nullptr)
			childs[i]->Draw();
	}
}

void QuadtreeNode::ResetBB()
{
	if (objects.size() > 0)
	{
		bBox.minPoint = (*objects.begin())->GetLocalABB().minPoint;
		bBox.maxPoint = (*objects.begin())->GetLocalABB().maxPoint;

		for (std::list<GameObject*>::iterator item = objects.begin(); item != objects.end(); item++)
		{
			bBox.Enclose((*item)->GetLocalABB());
		}
	}
	else
	{
		bBox.minPoint.Set(0, 0, 0);
		bBox.maxPoint.Set(0, 0, 0);
	}

}



void QuadtreeNode::GenerateChildren()
{
	float3 center = bBox.CenterPoint();
	float3 min = bBox.minPoint;
	float3 max = bBox.maxPoint;

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			childs[i] = new QuadtreeNode(this);
		}
	}
	
	

	childs[0]->bBox.minPoint.Set(min.x, min.y, center.z);
	childs[0]->bBox.maxPoint.Set(center.x, max.y, max.z);

	childs[1]->bBox.minPoint.Set(center.x, min.y, center.z);
	childs[1]->bBox.maxPoint.Set(max.x, max.y, max.z);

	childs[2]->bBox.minPoint.Set(min.x, min.y, min.z);
	childs[2]->bBox.maxPoint.Set(center.x, max.y, center.z);

	childs[3]->bBox.minPoint.Set(center.x, min.y, min.z);
	childs[3]->bBox.maxPoint.Set(max.x, max.y, center.z);

}


void QuadtreeNode::CollectIntersections(std::vector<GameObject*> &objects, math::Frustum frustum)
{
	if (frustum.Intersects(bBox))
	{
		for (std::list<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (frustum.Intersects((*it)->GetGlobalABB()))
				objects.push_back(*it);
		}
		for (int i = 0; i < 4; ++i)
			if (childs[i] != nullptr) childs[i]->CollectIntersections(objects, frustum);
	}
}

void QuadtreeNode::DistributeNode(uint buckedSize, uint& cap)
{
	for (std::list<GameObject*>::iterator item = objects.begin(); item != objects.end();)
	{

		if (objects.size() <= buckedSize)
			++item;
		else
		{
			GenerateChildren();
			cap++;
			for (int i = 0; i < 4; ++i)
			{
				if (childs[i]->bBox.Intersects((*item)->GetGlobalABB()))
					childs[i]->Insert((*item), buckedSize, cap);
			}
			item = objects.erase(item);
		}
	}
}

void QuadtreeNode::Insert(GameObject* obj, uint buckedSize, uint& cap)
{
	if (cap < MAXIUMDIVISIONS)
	{
		objects.push_back(obj);
		DistributeNode(buckedSize, cap);
	}
		
}

void QuadtreeNode::GenerateTestChildren()
{
	if (childs[0] != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			childs[i]->GenerateTestChildren();
		}
	}
	else
	{
		GenerateChildren();
	}

}

bool QuadtreeNode::CheckIfChildNeeded(GameObject* obj)
{
	float3 center = bBox.CenterPoint();
	float3 min = bBox.minPoint;
	float3 max = bBox.maxPoint;

	AABB checkers[4];

	checkers[0].minPoint.Set(min.x, min.y, center.z);
	checkers[0].maxPoint.Set(center.x, max.y, max.z);

	checkers[1].minPoint.Set(center.x, min.y, center.z);
	checkers[1].maxPoint.Set(max.x, max.y, max.z);

	checkers[2].minPoint.Set(min.x, min.y, min.z);
	checkers[2].maxPoint.Set(center.x, max.y, center.z);

	checkers[3].minPoint.Set(center.x, min.y, min.z);
	checkers[3].maxPoint.Set(max.x, max.y, center.z);



	return !(checkers[0].Intersects(obj->GetLocalABB()) && checkers[1].Intersects(obj->GetLocalABB()) && checkers[2].Intersects(obj->GetLocalABB()) && checkers[3].Intersects(obj->GetLocalABB()));
}

void QuadtreeNode::CleanUp()
{
	objects.clear();
	parent = nullptr;

	if (childs[0] != nullptr)
	{
		for (uint i = 0; i < 4; i++)
		{
			childs[i]->CleanUp();
			delete childs[i];
			childs[i] = nullptr;
		}
	}
}

void QuadtreeNode::SetBB(AABB b)
{
	bBox = b;
}

Quadtree::Quadtree()
{
	bucketSize = 1;
	root = new QuadtreeNode();
	
}

Quadtree::~Quadtree()
{
	delete root;

}

void Quadtree::AddObject(GameObject* o)
{
	root->AddObject(o);
	objects.push_back(o);
}

void Quadtree::RemoveObject(GameObject* o)
{
	root->RemoveObject(o);
}

void Quadtree::Draw()
{
	root->Draw();
}

void Quadtree::GenerateTestChildren()
{
	root->GenerateTestChildren();
}


void Quadtree::CleanUp()
{
	cap = 0;
	root->CleanUp();
	objects.clear();
}

void Quadtree::Reset()
{
	root->CleanUp();

	for (std::list<GameObject*>::iterator item = objects.begin(); item != objects.end(); item++)
	{
		root->AddObject(*item);
	}
	root->DistributeNode(bucketSize, cap);
}

void Quadtree::DistributeTree()
{
	root->DistributeNode(bucketSize, cap);

}