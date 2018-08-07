#pragma once
#include"Solver.h"


class SolverMPM
	:public Solver
{
public:
	SolverMPM();
	~SolverMPM();
	ParticleCloudMPM* bar;
	GridMPM* grid;
	int Resolution;
	int InsertTime;
	void Initial(SolverType SType);
	void Update();
	void PointListCheck();
	void SourceCheck();//��һ���ֻ�Ҫ�ع������Դ�������

};