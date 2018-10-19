
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "JsonDoc.h"
#include "MathGeoLib/MathGeoLib.h"
#include "WinConsole.h"
#include "ResMesh.h"
#include "ResTexture.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>



#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")  

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Renderer";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	VSLOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Load from config
	Load(App->config.GetObj(name.c_str()));

	//Setting attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


	//Create context
	context = SDL_GL_CreateContext(App->window->window);


	GLenum err = glewInit();
	// ... check for errors
	VSLOG("Using Glew %s", glewGetString(GLEW_VERSION));


	if(context == NULL)
	{
		VSLOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(vsync && SDL_GL_SetSwapInterval(1) < 0)
			VSLOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			VSLOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			VSLOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			VSLOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		if (depthTest) glEnable(GL_DEPTH_TEST);
		if (cullface) glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		if(lightning) glEnable(GL_LIGHTING);
		if (colorMaterial) glEnable(GL_COLOR_MATERIAL);
		if(texture2D) glEnable(GL_TEXTURE);
	}

	// Projection matrix for
	OnResize(App->window->w, App->window->h);

	// Create Primitives

	cube.Create();
	plane.Create();
	axis.Create();
	line.Create(3.0f);
	arrow.Create(3.0f);
	sphere.Create();
	cylinder.Create();
	capsule.Create();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
	
	//Color c = App->camera->background;
	//glClearColor(c.r, c.g, c.b, c.a);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(cam->GetOpenGLViewMatrix());
	if (drawCube)
		cube.Render();
	if (drawLine)
		line.Render();
	if (drawArrow)
		arrow.Render();
	if (drawAxis)
		axis.Render();
	if (drawPlane)
		plane.Render();
	if (drawSphere)
		sphere.Render();
	if (drawCylinder)
		cylinder.Render();
	if (drawCapsule)
		capsule.Render();

	DrawMeshes();

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	VSLOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool ModuleRenderer3D::Load(json_object_t* doc)
{

	vsync = App->config.GetObjValueInt(doc, "Vsync");
	return true;
}

bool ModuleRenderer3D::Save(json_object_t* doc)
{
	json_object_dotset_boolean(doc, "Renderer.Vsync", vsync);
	return true;
}


void ModuleRenderer3D::EnableVsync(bool enable)
{
	vsync = !vsync;
	vsync == true ? (SDL_GL_SetSwapInterval(1) < 0) : (SDL_GL_SetSwapInterval(0) < 0);
}

void ModuleRenderer3D::EnableDepthTest(bool enable)
{
	depthTest = enable;
	depthTest == true ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer3D::EnableCullFace(bool enable)
{
	cullface = enable;
	cullface == true ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::EnableLighting(bool enable)
{
	lightning = enable;
	lightning == true ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
}


void ModuleRenderer3D::EnableColorMaterial(bool enable)
{
	colorMaterial = enable;
	colorMaterial == true ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer3D::EnableTexture2D(bool enable)
{
	texture2D = enable;
	texture2D == true ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::EnableWireframe(bool enable)
{
	wireframe = enable;
	wireframe == true ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModuleRenderer3D::DrawMeshes()
{
	for (std::list<ResMesh*>::iterator item = meshes.begin(); item != meshes.end(); item++) 
	{
		(*item)->Draw();
		if (drawNormals)
			(*item)->DrawNormals();
		if (drawBBox)
			(*item)->DrawBoundingBox();
	}
}


void ModuleRenderer3D::CleanUpMeshes()
{
	for (std::list<ResMesh*>::iterator item = meshes.begin(); item != meshes.end(); item++)
	{
		(*item)->CleanUp();

	}

	meshes.clear();
}


void ModuleRenderer3D::SetMeshesTex(ResTexture i)
{

	for (std::list<ResMesh*>::iterator item = meshes.begin(); item != meshes.end(); item++)
	{
		(*item)->tex = i;
	}
}

float3 ModuleRenderer3D::GetMeshesCenter()
{
	float3 a = { 0,0,0 };
	int i = 0;
	for (std::list<ResMesh*>::iterator item = meshes.begin(); item != meshes.end(); item++)
	{
		a += (*item)->boundingBox.CenterPoint();
		i++;
	}

	return a/i;
}

AABB ModuleRenderer3D::GetMeshesAABB()
{
	AABB ret;
	float3 min = (*App->renderer3D->meshes.begin())->boundingBox.minPoint;
	float3 max = (*App->renderer3D->meshes.begin())->boundingBox.maxPoint;

	for (std::list<ResMesh*>::iterator item = meshes.begin(); item != meshes.end(); item++)
	{
		if ((*item)->boundingBox.minPoint.x < min.x) min.x = (*item)->boundingBox.minPoint.x;
		if ((*item)->boundingBox.minPoint.y < min.y) min.y = (*item)->boundingBox.minPoint.y;
		if ((*item)->boundingBox.minPoint.z < min.z) min.z = (*item)->boundingBox.minPoint.z;

		if ((*item)->boundingBox.maxPoint.x > max.x) max.x = (*item)->boundingBox.maxPoint.x;
		if ((*item)->boundingBox.maxPoint.y > max.y) max.y = (*item)->boundingBox.maxPoint.y;
		if ((*item)->boundingBox.maxPoint.z > max.z) max.z = (*item)->boundingBox.maxPoint.z;

	}
	ret.minPoint = min;
	ret.maxPoint = max;
	return ret;
}

