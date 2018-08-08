#pragma once
#include"solver/Solver.h"
#include"solver/SolverMPM.h"
#include"../scene/Scene.h"
#include"../simulation/ParticleSample.h"

class Simulator
{
public:
	Simulator();
	~Simulator();
	Solver* sol;//solver
	Scene* sc;//the scene used to solve
	void SimulatorInitial(Scene* scene, SolverType solverType, IntegralType integralType);
	void SimulatorUpdate();
private:
	int insertTime;
	void InformationCheck();
	void InformationUpdate();
};