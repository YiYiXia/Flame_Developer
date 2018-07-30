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
	int InsertTime;
	void Initial(SolverType SType);
	void Update();
	void PointListCheck();
	void SourceCheck();//��һ���ֻ�Ҫ�ع������Դ�������

};