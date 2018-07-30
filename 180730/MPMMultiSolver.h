#pragma once
#pragma once
#include"Solver.h"

class MPMMultiSolver
	:public Solver
{
public:
	MPMMultiSolver();
	~MPMMultiSolver();
	MPMParticleCloud* sand;
	MPMParticleCloud* water;
	MPMGrid* grid_s;
	MPMGrid* grid_w;
	double *cohesion_w; 
	int Resolution;
	void Initial(SolverType SType);
	void Update();
	void PointListCheck();
	void SourceCheck();
	void MomentaExchange();
	void Saturation();
};