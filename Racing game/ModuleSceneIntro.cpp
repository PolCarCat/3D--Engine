#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->audio->PlayMusic("Audio/Song.ogg");
	LOG("Loading Intro assets");
	bool ret = true;

	startcube.size = vec3(20, 10, 2);
	startcube.SetPos(0, 10, 8);
	finishcube.size = vec3(20, 10, 2);
	finishcube.SetPos(-25, 0, 1380);

	Sstart = App->physics->AddBody(startcube, 0.0f);
	Sstart->SetAsSensor(true);
	Sstart->collision_listeners.add(this);
	Sfinish = App->physics->AddBody(finishcube, 0.0f);
	Sfinish->SetAsSensor(true);
	Sfinish->collision_listeners.add(this);

	p2List<SpeedwayPieceDef> speedway_pieces_def;
	p2List<SpeedwaySensorDef> speedway_sensors_def;

	SpeedwayPieceDef piece1(vec3(10, 5, 100), vec3(0, 5, 40)); //Road
	speedway_pieces_def.add(piece1);

	SpeedwayPieceDef piece2(vec3(10, 5, 100), vec3(0, 5, 80), -20, vec3(1, 0, 0)); //Road
	speedway_pieces_def.add(piece2);

	SpeedwayPieceDef piece3(vec3(10, 5, 50), vec3(0, 10, 195)); //Road
	speedway_pieces_def.add(piece3);

	SpeedwayPieceDef piece4(vec3(10, 5, 50), vec3(-20, 5, 230), -50, vec3 (0,1,0)); //Road
	speedway_pieces_def.add(piece4);

	SpeedwayPieceDef piece5(vec3(10, 5, 100), vec3(-37, -10, 300), 10, vec3(1,0,0)); //Road
	speedway_pieces_def.add(piece5);

	SpeedwayPieceDef piece6(vec3(20, 5, 300), vec3(-37, -40, 530)); //Road
	speedway_pieces_def.add(piece6);

	SpeedwayPieceDef piece7(vec3(10, 20, 20), vec3(-30, -40, 430)); //Obstacle
	speedway_pieces_def.add(piece7);
	SpeedwaySensorDef Spiece7(vec3(8, 20, 20), vec3(-30, -40, 430));
	speedway_sensors_def.add(Spiece7);

	SpeedwayPieceDef piece8(vec3(10, 20, 20), vec3(-45, -40, 430)); //Obstacle
	speedway_pieces_def.add(piece8);
	SpeedwaySensorDef Spiece8(vec3(8, 20, 20), vec3(-45, -40, 430));
	speedway_sensors_def.add(Spiece8);

	SpeedwayPieceDef piece9(vec3(10, 20, 20), vec3(-35, -40, 500)); //Obstacle
	speedway_pieces_def.add(piece9);
	SpeedwaySensorDef Spiece9(vec3(8, 20, 20), vec3(-35, -40, 500));
	speedway_sensors_def.add(Spiece9);

	SpeedwayPieceDef piece10(vec3(10, 20, 20), vec3(-40, -40, 540)); //Obstacle
	speedway_pieces_def.add(piece10);
	SpeedwaySensorDef Spiece10(vec3(8, 20, 20), vec3(-40, -40, 540));
	speedway_sensors_def.add(Spiece10);

	SpeedwayPieceDef piece11(vec3(10, 20, 20), vec3(-35, -40, 580)); //Obstacle
	speedway_pieces_def.add(piece11);
	SpeedwaySensorDef Spiece11(vec3(8, 20, 20), vec3(-35, -40, 580));
	speedway_sensors_def.add(Spiece11);

	SpeedwayPieceDef piece12(vec3(20, 5, 300), vec3(-37, -40, 830)); //Road
	speedway_pieces_def.add(piece12);

	SpeedwayPieceDef piece13(vec3(20, 5, 100), vec3(-60, -40, 1030), -20, vec3(1,1,0)); //Road
	speedway_pieces_def.add(piece13);

	SpeedwayPieceDef piece14(vec3(20, 5, 100), vec3(-50, -35, 1140), 20, vec3(0, 1, 0)); //Road
	speedway_pieces_def.add(piece14);

	SpeedwayPieceDef piece15(vec3(10, 5, 100), vec3(-25, -35, 1230)); //Road
	speedway_pieces_def.add(piece15);

	SpeedwayPieceDef piece16(vec3(5, 5, 100), vec3(-25, -20, 1330), -20, vec3(1,0,0)); //Road
	speedway_pieces_def.add(piece16);

	for (int i = 0; i < speedway_pieces_def.count(); i++)
	{
		Cube c;
		SpeedwayPieceDef piece_def;
		speedway_pieces_def.at(i, piece_def);

		c.size = piece_def.size;
		c.SetPos(piece_def.position.x, piece_def.position.y, piece_def.position.z);
		if (piece_def.angle != 0)
		{ 
			c.SetRotation(piece_def.angle, piece_def.rotation_axis);
		}
		speedway.add(App->physics->AddBody(c, 0.0f));
	}
	for (int i = 0; i < speedway_sensors_def.count(); i++)
	{
		Cube c;
		SpeedwaySensorDef sensor_def;
		speedway_sensors_def.at(i, sensor_def);

		c.size = sensor_def.size;
		c.SetPos(sensor_def.position.x, sensor_def.position.y, sensor_def.position.z);
		speedway_sensors.add(App->physics->AddBody(c, 0.0f, true, this));
	}

	p2List<ObstacleDef> obstacles_def;

	ObstacleDef obstacle1(vec3(0, 30, 165));
	obstacles_def.add(obstacle1);

	int h = -26;

	ObstacleDef obstacle2(vec3(-40, h, 650));
	obstacles_def.add(obstacle2);

	ObstacleDef obstacle3(vec3(-31, h, 700));
	obstacles_def.add(obstacle3);

	ObstacleDef obstacle4(vec3(-38, h, 750));
	obstacles_def.add(obstacle4);

	ObstacleDef obstacle5(vec3(-48, h, 800));
	obstacles_def.add(obstacle5);

	ObstacleDef obstacle6(vec3(-43, h, 850));
	obstacles_def.add(obstacle6);

	ObstacleDef obstacle7(vec3(-52, h, 700));
	obstacles_def.add(obstacle7);

	ObstacleDef obstacle8(vec3(-34, h, 825));
	obstacles_def.add(obstacle8);

	for (int i = 0; i < obstacles_def.count(); i++)
	{
		Cube c;
		ObstacleDef obstacle_def;
		obstacles_def.at(i, obstacle_def);
		c.size = vec3(5, 5, 5);
		c.SetPos(obstacle_def.position.x, obstacle_def.position.y, obstacle_def.position.z);
		hammers_axis.add(App->physics->AddBody(c, 0.0f));

		c.size = vec3(20, 5, 5);
		hammers.add(App->physics->AddBody(c, 0.0f));
	}

	for (int i = 0; i < hammers.count(); i++)
	{
		PhysBody3D* bodyA;
		PhysBody3D* bodyB;
		hammers_axis.at(i, bodyA);
		hammers.at(i, bodyB);
		App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3(0, 0, 0), (0, 0, 0), (1, 1, 1), (1, 80, 1), true);
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	current_lap_time = (float)lap_timer.Read() / 1000;
	remaining_time = 300 - (float)lose_timer.Read() / 1000;

	if (remaining_time < 0)
		remaining_time = 0;

	if (Sstart->GetPos().z >= App->player->z)
	{
		lap_timer.Reset();
		started = false;
	}

	//Blit everything
	for (int i = 0; i < speedway.count(); i++)
	{
		Cube runder;
		PhysBody3D* body;
		speedway.at(i, body);
		body->GetTransform(&runder.transform);
		runder.size = body->size;
		runder.Render();
	}

	for (int i = 0; i < hammers_axis.count(); i++)
	{
		Cube runder;
		PhysBody3D* body;
		hammers_axis.at(i, body);
		body->GetTransform(&runder.transform);
		runder.size = body->size;
		runder.Render();
	}

	for (int i = 0; i < hammers.count(); i++)
	{
		Cube runder;
		PhysBody3D* body;
		hammers.at(i, body);
		body->GetTransform(&runder.transform);
		runder.size = body->size;
		runder.Render();
	}

	if (remaining_time <= 0)
	{
		if (laps >= 3)
			App->player->win = true;
		else
			App->player->lose = true;

		wonlost_timer.Start();
		lose_timer.Reset();
		laps = 0;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Hit!");
	if (body1 == Sstart)
	{
		lap_timer.Start();
		started = true;
		if (lose_timer.Read() == 0)
			lose_timer.Start();
	}
	else if (body1 == Sfinish)
	{
		App->player->Player_reset();
		last_lap_time = (float)lap_timer.Read() / 1000;
		if (best_lap_time == 0 || last_lap_time < best_lap_time)
			best_lap_time = last_lap_time;
		laps++;
		started = false;
		lap_timer.Reset();
	}
	else if (App->player->speed >= 70)
	{
		for (int i = 0; i < speedway_sensors.count(); i++)
		{
			PhysBody3D* sensor;
			speedway_sensors.at(i, sensor);
			if (body1 == sensor)
			{
				App->audio->PlayFx(4);
				App->player->Player_reset();
			}
		}
	}
}

