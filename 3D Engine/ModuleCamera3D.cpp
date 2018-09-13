#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "PhysVehicle3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	////Debug camera

	//if(App->physics->debug)
	//{ 
	//	vec3 newPos(0,0,0);
	//	float speed = 100.0f * dt;
	//	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//		speed = 8.0f * dt;

	//	if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	//	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	//	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	//	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	//	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	//	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	//	Position += newPos;
	//	Reference += newPos;

	//	// Mouse motion ----------------

	//	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	//	{
	//		int dx = -App->input->GetMouseXMotion();
	//		int dy = -App->input->GetMouseYMotion();

	//		float Sensitivity = 0.25f;

	//		Position -= Reference;

	//		if(dx != 0)
	//		{
	//			float DeltaX = (float)dx * Sensitivity;

	//			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		}

	//		if(dy != 0)
	//		{
	//			float DeltaY = (float)dy * Sensitivity;

	//			Y = rotate(Y, DeltaY, X);
	//			Z = rotate(Z, DeltaY, X);

	//			if(Y.y < 0.0f)
	//			{
	//				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//				Y = cross(Z, X);
	//			}
	//		}

	//		Position = Reference + Z * length(Position);
	//	}

	//	// Recalculate matrix -------------
	//	CalculateViewMatrix();
	//}

	//////Make camera follow player
	//else
	//{ 

	//	//Position of camera from player frame
	//	vec3 p_camera_pos(0, 10, -20);
	//	//Position of player
	//	vec3 player_pos = App->player->vehicle->GetPos();
	//	//Position of camera from world coordinates
	//	vec3 camera_pos = App->player->Player_to_World(p_camera_pos);
	//	//Adjustment so camera feels better
	//	camera_pos.z = player_pos.z - 20;
	//	//Make camera look at player from its position
	//	Look(camera_pos, player_pos);
	//}
	//
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}