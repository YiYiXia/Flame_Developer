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
	void SourceCheck();//��һ���ֻ�Ҫ�ع������Դ�������

};