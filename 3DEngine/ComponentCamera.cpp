#include "ComponentCamera.h"
#include "VectorialFunctions.h"
#include "Globals.h"
#include "Application.h"
#include <math.h>


namespace VectF = VectorialFunctions;

ComponentCamera::ComponentCamera(float _near, float _far, float _fov)
{
	nearDistance = _near;
	farDistance = _far;
	fovy = _fov;


	Reference = float3(0.0f, 0.0f, 0.0f);

	frustum.nearPlaneDistance = nearDistance;
	frustum.farPlaneDistance = farDistance;

	RecalculateFrustrum(App->window->w, App->window->h);
	CalculateViewMatrix();

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.up = float3(0, 1, 0);
	
	frustum.SetWorldMatrix(float3x4::identity);

	ViewMatrix = frustum.ViewMatrix();

}

ComponentCamera::~ComponentCamera()
{}

// -----------------------------------------------------------------
bool ComponentCamera::Start()
{
	VSLOG("Setting up the camera");
	bool ret = true;

	frustum.pos = float3(0.0f, 1.0f, 10.0f);

	return ret;
}

// -----------------------------------------------------------------
bool ComponentCamera::CleanUp()
{
	VSLOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
bool ComponentCamera::Update()
{


	// Recalculate matrix -------------
	if (drawFrustum)
	App->renderer3D->DrawFrustum(frustum);

	
	CalculateViewMatrix();
	return UPDATE_CONTINUE;
}

void ComponentCamera::UpdateUI()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		bool cur = isCurCam;
		ImGui::Checkbox("Is current camera", &cur);

		if (cur != isCurCam)
			App->scene->SetCurCam(this);

		ImGui::Checkbox("Draw Fustum", &drawFrustum);

		ImGui::SliderFloat("FarPlane", &farDistance, nearDistance + 10, 600);
		ImGui::SliderFloat("NearPlane", &nearDistance, 0, farDistance - 10);
		ImGui::SliderFloat("FOV", &fovy, 0, 100);

		frustum.nearPlaneDistance = nearDistance;
		frustum.farPlaneDistance = farDistance;
		RecalculateFrustrum();

	}
}


void ComponentCamera::CheckInput(float dt)
{

	float3 newPos(0, 0, 0);

	float speed = 8.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 50.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) FocusMeshes();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos.z -= speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos.z += speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos.x -= speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos.x += speed;


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) LookAt({ 0,0,0 });

	if (App->input->GetMouseZ() != 0 && !ImGui::IsMouseHoveringAnyWindow())
	{
		float wheelspd = frustum.pos.Length() / 2;
		if (App->input->GetMouseZ() > 0)
			newPos.z -= speed * wheelspd;

		else
			newPos.z += speed * wheelspd;
	}
	frustum.pos -= frustum.front * newPos.z;
	frustum.pos -= frustum.up.Cross(frustum.front) * newPos.x;
 

	Reference -= newPos;


	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && !ImGui::IsMouseHoveringAnyWindow() && !ImGui::IsMouseDragging())
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;
		float3 pos = App->renderer3D->GetMeshesCenter();

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			//Reference = center

			Reference = { 1,1,1 };
			if (!lookingAt)
			{
				LookAt(Reference);
				lookingAt = true;
			}


			float3 focus = frustum.pos - Reference;
			transform.rotation = transform.rotation.RotateAxisAngle(frustum.up, math::DegToRad(dx)) * transform.rotation.RotateAxisAngle(frustum.front.Cross(frustum.up), math::DegToRad(dy));

			focus = transform.rotation.Transform(focus);
			frustum.pos = focus + Reference;


		}
		else
			lookingAt = false;


		float3 left = frustum.front.Cross(frustum.up);
		transform.rotation = transform.rotation.RotateAxisAngle(frustum.up, math::DegToRad(dx)) * transform.rotation.RotateAxisAngle(left, math::DegToRad(dy));
		//transform.rotation =  transform.rotation.RotateX(math::DegToRad(dy)) * transform.rotation.RotateY(math::DegToRad(dx));
		frustum.front = transform.rotation.Mul(frustum.front);
		frustum.up = transform.rotation.Mul(frustum.up);

		//Position = Reference + Z * Position.Length();
	}





}


// -----------------------------------------------------------------
void ComponentCamera::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	//this->Position = Position;
	//this->Reference = Reference;


	//Z = float3(Position - Reference).Normalized();
	//X = float3(float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	//Y = Z.Cross(X);

	//if (!RotateAroundReference)
	//{
	//	this->Reference = this->Position;
	//	this->Position += Z * 0.05f;
	//}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const float3 &Spot)
{
	Reference = Spot;

	float3x3 rotmat = float3x3::LookAt(frustum.front, (Spot - frustum.pos).Normalized(), frustum.up, { 0,1,0 });


	frustum.front = rotmat.MulDir(frustum.front).Normalized();
	frustum.up = rotmat.MulDir(frustum.up).Normalized();

	CalculateViewMatrix();
}


// -----------------------------------------------------------------


// -----------------------------------------------------------------
float* ComponentCamera::GetViewMatrix()
{
	return &ViewMatrix[0][0];
}

// -----------------------------------------------------------------
void ComponentCamera::CalculateViewMatrix()
{
	ViewMatrix = frustum.ViewMatrix();
	ViewMatrix.Transpose();
}


void ComponentCamera::FocusMeshes()
{
	if (App->renderer3D->meshes.size() == 0)
		return;

	float3 centerf = App->renderer3D->GetMeshesCenter();
	AABB gAABB = App->renderer3D->GetMeshesAABB();
	float3 centerv = { centerf.x, centerf.y, centerf.z };


	transform.position.z = centerv.z;
	transform.position.x = gAABB.MaxX() - (gAABB.MinX() * 2);
	transform.position.y = gAABB.MaxY() - (gAABB.MinY() * 2);

	LookAt(centerv);

}

float4x4 ComponentCamera::ResizePerspMatrix(int width, int height)
{
	RecalculateFrustrum(width, height);
	return	frustum.ProjectionMatrix();
}

void ComponentCamera::RecalculateFrustrum(int width, int height)
{
	if (width != 0 && height != 0)
	aspectRatio = (float)width / (float)height;

	frustum.verticalFov = math::DegToRad(fovy);

	float ratio = tanf(frustum.verticalFov / 2) * aspectRatio;
	frustum.horizontalFov = 2*atanf(ratio);
	//frustum.horizontalFov = frustum.verticalFov * aspectRatio;

	frustum.AspectRatio();
}

bool ComponentCamera::CheckInside(const ResMesh m)
{
	bool ret = false;
	math::Sphere sphere;
	sphere.pos = frustum.CenterPoint();
	float3 point[8];
	frustum.GetCornerPoints(point);
	float3 radius = point[0] - sphere.pos;

	for (int i = 0; i < 8; i++)
	{
		if ((point[i] - sphere.pos).Length() > radius.Length())
			radius = point[i] - sphere.pos;
	}
	sphere.r = radius.Length();

	if (sphere.Contains(m.boundingBox))
	{
		float3 corners[8];
		m.boundingBox.GetCornerPoints(corners);
		math::Plane planes[6];
		frustum.GetPlanes(planes);
		uint in = 8;

		for (int p = 0; p < 6; ++p) {

			for (int i = 0; i < 8; ++i) {

				if (planes[p].IsOnPositiveSide(corners[i]))
				{
					in--;
				}
			}
			// were all the points outside of plane p?

			if (in == 0)
				return false;
		}

		return true;
	}
}