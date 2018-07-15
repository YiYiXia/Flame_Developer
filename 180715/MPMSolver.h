#pragma once
#include"Solver.h"


class MPMSolver
	:public Solver
{
public:
	MPMSolver();
	~MPMSolver();
	MPMParticleCloud* bar;
	MPMGrid* grid;
	int Resolution;
	void Initial(SolverType SType);
	void Update();
	void PointListCheck();
	void SourceCheck();//这一部分还要重构（多个源的情况）

};