#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "PhysBody3D.h"
#include "MathGeoLib/MathGeoLib.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	//VLOG("Loading scene assets");
	bool ret = true;
	
	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	//LOG("Unloading scene");


	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	Plane p;
	Sphere spehere1({ 0,0,0 }, 5);
	Triangle triangle1({ 3,4,4 }, { 1,1,1 }, { 5,2,3 });
	AABB aabb1({ -5,-5,-5 }, { 10,10,10 });

	if (spehere1.Intersects(triangle1))
	{
		VSLOG("Intersection triangle Sphere");
	}
	if (spehere1.Intersects(aabb1))
	{
		VSLOG("Intersection aabb Sphere");
	}

	
	return UPDATE_CONTINUE;
}

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	//LOG("Hit!");
}