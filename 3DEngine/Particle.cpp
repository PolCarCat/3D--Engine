#include "Particle.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

Particle::Particle(ParticleInfo i)
{
	info = i;

	position = info.startPosition;
	lifeTime = info.lifetime;
	size = info.startSize;
	spin = info.startSpin;
	speed = info.speed;
	lifeLeft = lifeTime;
	color = info.startColor;
	direction = info.direction;
	info.billboard = new ComponentBillboard();
	info.billboard->Start();
}


Particle::~Particle()
{
}

void Particle::Start()
{
}

void Particle::Update(float dt)
{
	lifeLeft -=  dt;

	if (lifeLeft <= 0)
		toDelete = true;

	lifetimeRatio = lifeLeft / lifeTime;

	size = Ratio(info.endSize, info.startSize);
	spin = Ratio(info.endSpin, info.startSpin);
	color = Ratio(info.endColor, info.startColor);

	float3 displacement = (direction + (info.gravity * lifetimeRatio)) * speed;

	position += displacement * dt;

	if (info.billboard != nullptr)
	{
		info.billboard->Update();
		direction.x *= info.billboard->transform->rotation.x;
		direction.y *= info.billboard->transform->rotation.y;
		direction.z *= info.billboard->transform->rotation.z;

		UpdateBillboardPos();
	}

	glColor4f(color.r, color.g, color.b, color.a);
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex3f(position.x, position.y, position.z);
	glEnd();

}

void Particle::Draw()
{

}

void Particle::CleanUp()
{
	delete info.billboard;
}

bool Particle::Delete()
{
	return toDelete;
}


float Particle::Ratio(float max, float min)
{
	return (max - min) * (1 - lifetimeRatio) + min;
}

Color Particle::Ratio(Color max, Color min)
{
	Color c;
	c.r = Ratio(info.endColor.r, info.startColor.r);
	c.g = Ratio(info.endColor.g, info.startColor.g);
	c.b = Ratio(info.endColor.b, info.startColor.b);
	c.a = Ratio(info.endColor.a, info.startColor.a);

	return c;
}

void Particle::UpdateBillboardPos()
{
	if (info.billboard != nullptr)
	{
		info.billboard->transform->position.x = position.x;
		info.billboard->transform->position.y = position.y;
		info.billboard->transform->position.z = position.z;
	}
}

void ParticleInfo::Set(float sSize, float eSize, float sSpin, float eSpin, float spd, uint life, float3 pos, float3 dir, float3 grav, Color sColor, Color eColor)
{
	startSize = sSize;
	endSize = eSize;

	startSpin = sSpin;
	endSpin = eSpin;

	speed = spd;
	lifetime = life;

	startPosition = pos;
	direction = dir;
	gravity = grav;

	startColor = sColor;
	endColor = eColor;
}
