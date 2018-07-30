#pragma once
#include"Solver.h"
#include"SDF.h"
class BoxSolver
	:public Solver
{
public:
	BoxSolver();
	~BoxSolver();
	void Initial(SolverType SType);
	void Update();
	void CollisionCheck();
	void ApplyForce();
};
