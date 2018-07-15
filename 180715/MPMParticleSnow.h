#pragma once
#include"MPMParticle.h"

typedef struct SnowParameters
{
	double Lambda;
	double Mu;
	double CRIT_COMPRESS;
	double CRIT_STRETCH;
	double HARDENING;
	double mass;
}SnowParameters;

class MPMParticleSnow
	:public MPMParticle
{
public:
	MPMParticleSnow();
	MPMParticleSnow(Vector2d pos, Vector2d vel, double mass, int num, double lambda, double mu,double harden,double COMPRESS, double STRETCH);
	MPMParticleSnow(Vector2d pos, Vector2d vel, int num, SnowParameters p, Vector3d PColor);
	~MPMParticleSnow();
	
	double Lambda;
	double Mu;
	double CRIT_COMPRESS;
	double CRIT_STRETCH;
	double HARDENING;
	void updatePos();
	void updateGradient();
	void applyPlasticity();
	Matrix2d energyDerivative();
	void MPMUpdate();
	void DeltaForce();
};
