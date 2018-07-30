#pragma once
#include"SPHSolver.h"
extern double SURFACE_TENSION;
extern double LINEAR_VISC;//线性阻力
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
	void PressureStep();//预计计算密度，密度的变化，压强，压强的变化
	void Correct();
	void Project();
	void ParametersIn();
};