#pragma once
#include"SPHSolver.h"
extern double SURFACE_TENSION;
extern double LINEAR_VISC;//��������
extern double QUAD_VISC;
extern double KERN_NORM;
extern double KERN;

extern double STIFFNESS;
extern double STIFF_APPROX;
extern double REST_DENSITY;
class SPHSolverPCI
	:public SPHSolver
{
public:
	SPHSolverPCI();
	~SPHSolverPCI();
	void SPHUpdate();
	void SPHInitial();
	void Integrate();
	void ApplyExternalForce(Vector2d f);
	void PressureStep();//Ԥ�Ƽ����ܶȣ��ܶȵı仯��ѹǿ��ѹǿ�ı仯
	void Correct();
	void Project();
	void ParametersIn();
};