#include"SPHParticle.h"

SPHParticle::SPHParticle()
{
	mass = PARTICLE_MASS;
}
SPHParticle::SPHParticle(Vector2d p, int num ,Vector2d v, Vector3d PColor)
{
	index = num;
	position = p;
	velocity = v;
	mass = PARTICLE_MASS;
	color = PColor;
	Nh = new Neighborhood();
	AType = REAL;
}
SPHParticle::~SPHParticle()
{

}

void SPHParticle::Update()
{
}
