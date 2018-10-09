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
	Cylinder,
	Axis,
	Arrow
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	Create();
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const float3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	float4x4 transform;
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
	void Create(float x = 0, float y = 0, float z = 0, float d = 200);
	void Render() const;
public:
	float depth;
};

class PAxis : public Primitive
{
public:
	PAxis();
	void Create(float thickness = 2.0f);
	void Render() const;
public:
	float thickness;
};

class PLine : public Primitive
{
public:
	PLine();
	void Create(float thickness = 2.0f, float ox = 0.0f, float oy = 0.0f, float oz = 0.0f, float ex = 0.0f, float ey = 1.0f, float ez = 0.0f);
	void Render() const;
public:
	float thickness, ox, oy, oz, ex, ey, ez;
};

class PArrow : public Primitive
{
public:
	PArrow();
	void Create(float thickness = 2.0f, float ox = 0.0f, float oy = 0.0f, float oz = 0.0f, float ex = 0.0f, float ey = 1.0f, float ez = 0.0f);
	void Render() const;
public:
	float thickness, ox, oy, oz, ex, ey, ez;
};

class PSphere : public Primitive
{
public:
	PSphere();
	void Create(float ox = 0.0f, float oy = 0.0f, float oz = 0.0f, float radius = 1.0f, uint stacks = 30, uint sectors = 30);
};

class PCylinder : public Primitive
{
public:
	PCylinder();
	void Create(float ox = 0.0f, float oy = 0.0f, float oz = 0.0f, float radius = 1.0f, float lenght = 0.3f, uint sectors = 30);
};

#endif