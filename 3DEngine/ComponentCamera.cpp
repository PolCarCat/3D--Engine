#include "ComponentCamera.h"
#include "VectorialFunctions.h"
#include "Globals.h"
#include "Application.h"


namespace VectF = VectorialFunctions;

ComponentCamera::ComponentCamera(float _near, float _far, float _fov)
{
	nearDistance = _near;
	farDistance = _far;
	fovy = _fov;

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);



	Position = float3(0.0f, 1.0f, -10.0f);
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
	////Debug camera


	float3 newPos(0, 0, 0);
	float speed = 1.0f;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 50.0f;

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) FocusMeshes();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) LookAt({ 0,0,0 });

	if (App->input->GetMouseZ() != 0 && !ImGui::IsMouseHoveringAnyWindow())
	{
		float wheelspd = Position.Length() / 2;
		if (App->input->GetMouseZ() > 0)
			newPos -= Z * speed * wheelspd;

		else
			newPos += Z * speed * wheelspd;
	}


	frustum.pos -= newPos;
	Reference -= newPos;


	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && !ImGui::IsMouseHoveringAnyWindow() && !ImGui::IsMouseDragging())
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();
		float3 realRef = Reference;
		float3 realPos = Position;
		float Sensitivity = 0.25f;
		float3 pos = App->renderer3D->GetMeshesCenter();

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
			LookAt({ pos.x, pos.y, pos.z });
		else
			Reference = Position;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = VectF::rotatef3(X, DeltaX, float3(0.0f, 1.0f, 0.0f));
			Y = VectF::rotatef3(Y, DeltaX, float3(0.0f, 1.0f, 0.0f));
			Z = VectF::rotatef3(Z, DeltaX, float3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = VectF::rotatef3(Y, DeltaY, X);
			Z = VectF::rotatef3(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}
		}


		Position = Reference + Z * Position.Length();
	}




	// Recalculate matrix -------------
	CalculateViewMatrix();



	return UPDATE_CONTINUE;
}



// -----------------------------------------------------------------
void ComponentCamera::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;


	Z = float3(Position - Reference).Normalized();

	X = float3(float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

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
void ComponentCamera::Move(const float3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

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


	Position.z = centerv.z;
	Position.x = gAABB.MaxX() - (gAABB.MinX() * 2);
	Position.y = gAABB.MaxY() - (gAABB.MinY() * 2);

	LookAt(centerv);

}

float4x4 ComponentCamera::ResizePerspMatrix(int width, int height)
{
	RecalculateFrustrum(width, height);
	return	frustum.ProjectionMatrix();
}

void ComponentCamera::RecalculateFrustrum(int width, int height)
{
	aspectRatio = (float)width / (float)height;
	frustum.verticalFov = math::DegToRad(fovy);

	//float ratio = tanf(frustum.verticalFov / 2) * aspectRatio;
	//frustum.horizontalFov = atanf(ratio);
	frustum.horizontalFov = frustum.verticalFov * aspectRatio;

	frustum.AspectRatio();
}