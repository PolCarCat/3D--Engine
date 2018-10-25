#include "QuadTree.h"
#include "Application.h"




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
		bBox = o->GetBB();
	else
		bBox.Enclose(o->GetBB());
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
		bBox.minPoint = (*objects.begin())->GetBB().minPoint;
		bBox.maxPoint = (*objects.begin())->GetBB().maxPoint;

		for (std::list<GameObject*>::iterator item = objects.begin(); item != objects.end(); item++)
		{
			bBox.Enclose((*item)->GetBB());
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

void QuadtreeNode::DistributeNode(uint buckedSize)
{
	for (std::list<GameObject*>::iterator item = objects.begin(); item != objects.end();)
	{
		if (objects.size() > buckedSize)
		{
			Insert(*item);
			item = objects.erase(item);
		}
		else
			item++;
	}
}

void QuadtreeNode::Insert(GameObject* obj)
{


	if (!CheckIfChildNeeded(obj))
	{
		objects.push_back(obj);
	}
	else
	{
		if (childs[0] == nullptr)
		{
			GenerateChildren();
		}

		for (int i = 0; i < 4; i++)
		{
			if (childs[i]->bBox.Intersects(obj->GetBB()))
				childs[i]->Insert(obj);
		}
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



	return !(checkers[0].Intersects(obj->GetBB()) && checkers[1].Intersects(obj->GetBB()) && checkers[2].Intersects(obj->GetBB()) && checkers[3].Intersects(obj->GetBB()));
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

void Quadtree::DistributeTree()
{
	root->DistributeNode(bucketSize);

}