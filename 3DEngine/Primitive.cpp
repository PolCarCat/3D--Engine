
#include "Globals.h"
#include "Primitive.h"

#include "Glew/include/glew.h" 
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
// ------------------------------------------------------------
Primitive::Primitive() : transform(transform.identity), color(White), wire(false), type(PrimitiveTypes::Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	//if(wire)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void Primitive::Create()
{

}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.Translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const float3 &u)
{
	transform.RotateAxisAngle(u, angle);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.Scale(x, y, z);
}

void Primitive::CreateBuffer()
{
	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

PCube::PCube() : Primitive()
{
	type = PrimitiveTypes::Cube;
}

void PCube::Create(float sx, float sy, float sz, float ox, float oy, float oz)
{
	type = PrimitiveTypes::Cube;

	float hx = sx / 2.0f;
	float hy = sy / 2.0f;
	float hz = sz / 2.0f;

	vertices.push_back(ox - hx);	//A
	vertices.push_back(oy - hy);
	vertices.push_back(oz + hz);

	vertices.push_back(ox + hx);	//B
	vertices.push_back(oy - hy);
	vertices.push_back(oz + hz);

	vertices.push_back(ox - hx);	//C
	vertices.push_back(oy + hy);
	vertices.push_back(oz + hz);

	vertices.push_back(ox + hx);	//D
	vertices.push_back(oy + hy);
	vertices.push_back(oz + hz);

	vertices.push_back(ox - hx);	//E
	vertices.push_back(oy - hy);
	vertices.push_back(oz - hz);

	vertices.push_back(ox + hx);	//F
	vertices.push_back(oy - hy);
	vertices.push_back(oz - hz);

	vertices.push_back(ox - hx);	//G
	vertices.push_back(oy + hy);
	vertices.push_back(oz - hz);

	vertices.push_back(ox + hx);	//H
	vertices.push_back(oy + hy);
	vertices.push_back(oz - hz);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(7); 
	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(5);
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(7); 
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(6); 
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(6);
	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(0);
	indices.push_back(5);
	indices.push_back(1);

	CreateBuffer();
}

PPlane::PPlane() : Primitive()
{
	type = PrimitiveTypes::Plane;
}

void PPlane::Create(float x, float y, float z, float d)
{
	type = PrimitiveTypes::Plane;
	depth = d;
}

void PPlane::Render() const
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	for (float i = -depth; i <= depth; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -depth);
		glVertex3f(i, 0.0f, depth);
		glVertex3f(-depth, 0.0f, i);
		glVertex3f(depth, 0.0f, i);
	}

	glEnd();
}

PAxis::PAxis() : Primitive()
{
	type = PrimitiveTypes::Axis;
}

void PAxis::Create(float thickness)
{
	type = PrimitiveTypes::Axis;
	this->thickness = thickness;
}

void PAxis::Render() const
{
	glLineWidth(thickness);

	glBegin(GL_LINES);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

	glEnd();

	glLineWidth(1.0f);
	glColor4f(color.r, color.g, color.b, color.a);
}

PLine::PLine() : Primitive()
{
	type = PrimitiveTypes::Line;
}

void PLine::Create(float thickness, float ox, float oy, float oz, float ex, float ey, float ez)
{
	type = PrimitiveTypes::Line;
	this->thickness = thickness;
	this->ox = ox;
	this->oy = oy;
	this->oz = oz;
	this->ex = ex;
	this->ey = ey;
	this->ez = ez;
}

void PLine::Render() const
{
	glLineWidth(thickness);
	glBegin(GL_LINES);
	glVertex3f(ox, oy, oz);
	glVertex3f(ex, ey, ez);
	glEnd();
	glLineWidth(1);
}

PArrow::PArrow() : Primitive()
{
	type = PrimitiveTypes::Arrow;
}

void PArrow::Create(float thickness, float ox, float oy, float oz, float ex, float ey, float ez)
{
	type = PrimitiveTypes::Arrow;
	this->thickness = thickness;
	this->ox = ox;
	this->oy = oy;
	this->oz = oz;
	this->ex = ex;
	this->ey = ey;
	this->ez = ez;
}

void PArrow::Render() const
{
	glLineWidth(thickness);
	glBegin(GL_LINES);
	glVertex3f(ox, oy, oz);
	glVertex3f(ex, ey, ez);
	glEnd();
	glLineWidth(1);
}

PSphere::PSphere() : Primitive()
{
	type = PrimitiveTypes::Sphere;
}

void PSphere::Create(float ox, float oy, float oz, float radius, uint stacks, uint sectors)
{
	type = PrimitiveTypes::Sphere;

	float x;
	float y;
	float z;
	float xy;
	float an1; //for stacks
	float an2; //for sectors
	float currstack = pi / stacks;
	float currsector = 2 * pi / sectors;

	for (int i = 0; i <= stacks; ++i)
	{
		an1 = pi / 2 - i * currstack;
		xy = radius * cosf(an1);
		z = radius * sinf(an1);

		for (int c = 0; c <= sectors; ++c)
		{
			an2 = c * currsector;

			x = xy * cosf(an2);
			y = xy * sinf(an2);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	int k1, k2;
	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (sectors + 1);
		k2 = k1 + sectors + 1;

		for (int c = 0; c < sectors; ++c, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	int c = 0;

	for (vector<float>::iterator i = vertices.begin(); i != vertices.end(); ++i)
	{
		if (c == 0)
			(*i) += ox;
		else if (c == 1)
			(*i) += oy;
		else if (c == 2)
			(*i) += oy;
		
		c++;

		if (c >= 3)
			c = 0;
	}

	CreateBuffer();
}

PCylinder::PCylinder() : Primitive()
{
	type = PrimitiveTypes::Cylinder;
}

void PCylinder::Create(float ox, float oy, float oz, float radius, float lenght, uint sectors)
{
	type = PrimitiveTypes::Cylinder;

	float x;
	float y;
	float z;
	float xy;

	float sector = 2 * pi / sectors;
	float an;

	y = oy - lenght / 2;
												
	for (int j = 0; j <= sectors; ++j)
	{
		an = j * sector;
		z = oz + radius * cosf(an);
		x = ox + radius * sinf(an);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}

	y = oy + lenght / 2;

	for (int j = 0; j <= sectors; ++j)
	{
		an = j * sector;
		z = oz + radius * cosf(an);
		x = ox + radius * sinf(an);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}

	vertices.push_back(ox);
	vertices.push_back(oy - lenght / 2);
	vertices.push_back(oz);

	vertices.push_back(ox);
	vertices.push_back(oy + lenght / 2);
	vertices.push_back(oz);


	int k1, k2;

	for (int i = 0; i < sectors; i++)
	{
		indices.push_back(i);
		indices.push_back(((int)vertices.size() / 3) - 2);
		indices.push_back(i + 1);
	}

	for (int i = 0; i < 1; ++i)
	{
		k1 = i * (sectors + 1);
		k2 = k1 + sectors + 1; 

		for (int j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			if (i >= 0)
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k1);

			}

			if (i != 1)
			{
				indices.push_back(k2 + 1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}
		}
	}

	int last_vertex = (vertices.size() / 3) - sectors - 3;
	for (int i = 0; i < sectors; i++)
	{
		indices.push_back(((int)vertices.size() / 3) - 1);
		indices.push_back(i + last_vertex);
		indices.push_back(i + last_vertex + 1);
	}


	CreateBuffer();
}