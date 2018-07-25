#pragma once
#include"MPMParticle.h"

typedef struct SandParameters 
{
	double Lambda;
	double Mu;
	double H0;
	double H1;
	double H2;
	double H3;
	double mass;
}SandParameters;

enum SandType 
{
	DRY,
	WET
};

class MPMParticleSand
	:public MPMParticle
{
public:
	MPMParticleSand();
	MPMParticleSand(Vector2d pos, Vector2d vel, double mass, int num = 0, double lambda = 0, double mu = 0);
	MPMParticleSand(Vector2d pos, Vector2d vel, int num, SandParameters p, SandType Type, Vector3d PColor);
	~MPMParticleSand();
	double alpha;
	double delta_q;
	double q;
	double V_c;
	double C_cp0,C_cp;
	double Lambda;
	double Mu;
	double H0;
	double H1;
	double H2;
	double H3;
	
	void updatePos();
	void updateGradient();
	void applyPlasticity();
	Matrix2d energyDerivative();
	Matrix2d Derivative(Matrix2d s);
	Matrix2d project();
	void DeltaForce();
	void MPMUpdate();
};