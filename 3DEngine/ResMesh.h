#pragma once

#include "Globals.h"
#include "ResTexture.h"

class ResMesh
{
public:
	ResMesh();
	~ResMesh();


	uint id_indice = 0; // index in VRAM
	uint num_indice = 0;
	uint* indice = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	uint num_normals = 0;
	float* normals = nullptr;

	uint id_colors = 0;
	uint num_colors = 0;
	float* colors = nullptr;

	uint id_textC = 0;
	uint num_textC = 0;
	float* textC = nullptr;

	ResTexture tex;
	std::string name;

	AABB boundingBox;

	uint32_t uuid = 0;

	void GenerateBuffer();
	void Draw();
	void DrawNormals();
	void DrawBoundingBox();
	void CleanUp();

};

