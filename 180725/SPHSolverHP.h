#pragma once
#include"SPHSolver.h"
const double Pi = 4.0 * atan(1);

extern int res;						// grid res
extern double dx;					// grid dx width
extern double h;					// particle search radius
extern double dt;					// time step length
extern double rho0;					// initial rho0
extern double gamma;				// gamma
extern double g;				// gravity
extern double c;					// speed of sound
extern double supp_size;			// support size
extern double water_width;			// width of water
extern double water_height;			// height of water
extern double dom_size;				// size of domain
extern double end_t;				// end time
extern int frame_num;				// number of frame
extern int temp_num;				// temporary number counter (for density reinit)
extern double frame_dt;				// frame dt

class SPHSolverHP
	:public SPHSolver
{
public:
	SPHSolverHP();
	~SPHSolverHP();
	SPHParticleCloud* bar_pre;
	int n_real, pre_n_real;
	double t;
	void Predictor();
	void Corrector();
	void DensityReinit();
	void ParametersIn();
	void SPHUpdate();
	void SPHInitial();
};

double weightFunction(Vector2d x);
Vector2d weightGradient(Vector2d x);
double equation_of_state(double rho);
double drhodt_increment(SPHParticle p1, SPHParticle p2);
double artificial_viscosity(SPHParticle p1, SPHParticle p2);
Vector2d boundary_force(Vector2d x1, Vector2d x2, double r0);
Vector2d dvdt_increment(SPHParticle p1, SPHParticle p2);