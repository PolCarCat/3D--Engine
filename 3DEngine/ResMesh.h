#ifndef __RESMESH_H__
#define __RESMESH_H__
#include "Resource.h"
#include "MathGeoLib/MathGeoLib.h"

class ResMesh : public Resource
{
public:
	ResMesh(uint32_t UUID = 0);
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

	void GenerateBuffer();
	void Draw();
	void DrawNormals();
	void CleanUp();
	void Init();
	AABB boundingBox;
};

#endif //!__RESMES_H__