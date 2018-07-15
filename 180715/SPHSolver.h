#pragma once
#include"Solver.h"
#include"SPHParticleCloud.h"
#include"SPHParticle.h"
class SPHSolver
	:public Solver
{
public:
	SPHSolver();
	~SPHSolver();
	SPHParticleCloud* bar;
	void Initial(SolverType SType);
	void Update();
	virtual void ParametersIn() = 0;
	virtual void SPHUpdate() = 0;
	virtual void SPHInitial() = 0;
	void PointListCheck();
};