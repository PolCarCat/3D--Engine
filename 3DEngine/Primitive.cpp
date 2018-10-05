
#include "Globals.h"
#include "Primitive.h"

#include "Glew/include/glew.h" 
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
// ------------------------------------------------------------
Primitive::Primitive() : transform(transform.identity), color(White), wire(false), axis(false), type(PrimitiveTypes::Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	if(axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

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
	}

	//glColor3f(color.r, color.g, color.b);
	glColor4f(color.r, color.g, color.b, color.a);

	//if(wire)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (type != PrimitiveTypes::Plane)
	{
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id);
		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
	}
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

PPlane::PPlane(float x, float y, float z, float d) : Primitive()
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