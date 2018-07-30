#include"MPMParticleWater.h"
#include"Math.h"

MPMParticleWater::MPMParticleWater()
{
}
MPMParticleWater::~MPMParticleWater()
{
}

MPMParticleWater::MPMParticleWater(Vector2d pos, Vector2d vel, double mass, int num, double gama, double hardness)
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	index = num;
	Gama = gama;
	Hardness = hardness;
	J = 1;
	color = Vector3d(0, 0.8, 1.0);
	p_type = WATER;
}

MPMParticleWater::MPMParticleWater(Vector2d pos, Vector2d vel, int num, WaterParameters p, Vector3d PColor)
{
	position = pos;
	velocity = vel;
	index = num;
	this->mass = p.mass;
	Gama = p.Gama;
	Hardness = p.Hardness;
	J = 1;
	float dc = random_number(0, 0.2);
	if (PColor[0] < 0) color = Vector3d(dc, -dc, 0.0) + Vector3d(0.3, 0.8, 1.0);
	else color = Vector3d(dc, -dc, 0.0) + PColor;
	p_type = WATER;
}

void MPMParticleWater::updatePos()
{
	position += DT*velocity;
}
void MPMParticleWater::updateGradient()
{
	double trace;
	trace = velocity_gradient.trace();
	trace *= DT;
	trace = trace + 1;
	J = trace * J;
}

Matrix2d MPMParticleWater::energyDerivative()
{
	double Je = def_elastic.determinant();
	Matrix2d out;

	double P = Hardness*(1 / pow(J, Gama) - 1);
	Matrix2d temp = Matrix2d::Identity();
	out = -P*temp;
	return volume * out;
}

void MPMParticleWater::MPMUpdate()
{
	updatePos();
	updateGradient();
}

void MPMParticleWater::DeltaForce()
{

}