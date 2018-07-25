#pragma once
#pragma once
#include"Solver.h"


class WaterSolver
	:public Solver
{
public:
	WaterSolver();
	~WaterSolver();
	MPMParticleCloud* bar;
	MACGrid* grid;
	int Resolution;
	void Initial(SolverType SType);
	void Update();
	void PointListCheck();
	void SourceCheck();//这一部分还要重构（多个源的情况）

};