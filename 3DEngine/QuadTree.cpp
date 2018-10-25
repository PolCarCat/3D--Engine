#include "QuadTree.h"
#include "Application.h"




QuadtreeNode::QuadtreeNode()
{
	bBox.minPoint.Set(0, 0, 0);
	bBox.maxPoint.Set(0, 0, 0);


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

void Quadtree::CheckStaticObj()
{

}