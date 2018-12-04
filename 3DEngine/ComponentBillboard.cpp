#include "ComponentBillboard.h"
#include "Application.h"

#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ResMesh.h"
#include "ResTexture.h"


ComponentBillboard::ComponentBillboard()
{
	transform = new ComponentTransform();
	mesh = App->resourceManager->GetBillboard();
}


ComponentBillboard::~ComponentBillboard()
{
}

bool ComponentBillboard::Start()
{

	return true;
}

bool ComponentBillboard::Update()
{
	return true;
}

void ComponentBillboard::UpdateUI()
{
}

bool ComponentBillboard::CleanUp()
{
	return true;
}

void ComponentBillboard::FaceCamera()
{
	ComponentCamera* cam;

	if (App->renderer3D->IsUsingGhostCam())
		cam = App->scene->GetGhostCam();
	else
		cam = App->scene->GetCurCam();


	switch (alignment)
	{
	case SCREEN_ALIGN:

		break;
	case WORLD_ALIGN:

		break;
	case AXIAL_ALIGN:

		break;
	default:
		break;
	}
}

void ComponentBillboard::SetTexture(ResTexture * t)
{
	tex = t;
}
