#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Color.h"
#include <vector>
#include "MathGeoLib/MathGeoLib.h"

using namespace std;

enum PrimitiveTypes
{
	Point,
	Line,
	Plane,
	Cube,
	Sphere,
	Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const float3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	float4x4 transform;
	bool axis;
	bool wire;
	uint my_id;
	vector<float> vertices;
	vector<uint> indices;

protected:
	PrimitiveTypes type;

protected:
	void CreateBuffer();
};

// ============================================
class PCube : public Primitive
{
public :
	PCube();
void Create(float sx = 1, float sy = 1, float sz = 1, float ox = 0, float oy = 0, float oz = 0); // s = size, o = origin
};

class PPlane : public Primitive
{
public:
	PPlane();
	PPlane(float x = 0, float y = 0, float z = 0, float d = 200);
	void Render() const;
public:
	float depth;
};

#endif