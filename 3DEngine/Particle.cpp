#include "Particle.h"


#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

Particle::Particle(ParticleInfo i)
{
	info = i;
	position = { 0,0,0 };

	lifeTime = info.lifetime;
	size = info.startSize;
	spin = info.startSpin;
	speed = info.speed;
	framesLeft = lifeTime;
	
}


Particle::~Particle()
{
}

void Particle::Start()
{
}

void Particle::Update(float dt)
{
	framesLeft -=  dt;

	if (framesLeft <= 0)
		toDelete = true;

	lifetimeRatio = float(framesLeft) / float(lifeTime);

	size = Ratio(info.startSize, info.endSize);
	spin = Ratio(info.startSpin, info.endSpin);
	color = Ratio(info.startColor, info.endColor);

	position += info.direction * speed * dt;

	glColor4f(0.2f, 0.2f, 1.0f, 1.0f);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex3f(position.x, position.y, position.z);
	glEnd();

}

void Particle::Draw()
{

}

void Particle::CleanUp()
{

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

void ParticleInfo::Set(float sSize, float eSize, float sSpin, float eSpin, float3 dir, Color sColor, Color eColor)
{
	startSize = sSize;
	endSize = eSize;
	startSpin = sSpin;
	endSpin = eSpin;
	direction = dir;
	startColor = sColor;
	endColor = eColor;
}
