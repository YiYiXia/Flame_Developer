#pragma once
#pragma once
#include"Solver.h"


class SolverMAC
	:public Solver
{
public:
	SolverMAC();
	~SolverMAC();
	ParticleCloudMPM* bar;
	GridMAC* grid;
	int Resolution;
	void Initial(SolverType SType);
	void Update();
	void PointListCheck();
	void SourceCheck();//��һ���ֻ�Ҫ�ع������Դ�������

};