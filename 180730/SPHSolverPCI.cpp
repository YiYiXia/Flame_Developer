#include"SPHSolverPCI.h"

double SURFACE_TENSION;
double LINEAR_VISC;//线性阻力
double QUAD_VISC;
double KERN_NORM;
double KERN;

double STIFFNESS;
double STIFF_APPROX;
double REST_DENSITY;

SPHSolverPCI::SPHSolverPCI()
{
	
}

SPHSolverPCI::~SPHSolverPCI()
{

}

void SPHSolverPCI::SPHInitial()
{

}

void SPHSolverPCI::SPHUpdate()
{
	for (int i = 0; i < SOLVER_STEPS; i++)
	{
		//for (int j = 0; j < bar->SPHCloud.size(); j++)
		//{
		//	bar->SPHCloud[j]->ApplyExternalForce(SPHg);
		//	bar->SPHCloud[j]->Integrate();
		//}
		ApplyExternalForce(SPHg);
		Integrate();
		bar->GridInsert();
		bar->FindNeighbour();
		PressureStep();
		Project();
		Correct();
		/*for (int j = 0; j < bar->SPHCloud.size(); j++)
		{
			bar->SPHCloud[j]->PressureStep();
		}
		for (int j = 0; j < bar->SPHCloud.size(); j++)
		{
			bar->SPHCloud[j]->Project();
		}
		for (int j = 0; j < bar->SPHCloud.size(); j++)
		{
			bar->SPHCloud[j]->Correct();
		}*/
		SceneControl();
		bar->EnforceBoundary();
		PointListCheck();
		frames++;
	}
}

void SPHSolverPCI::Integrate()
{

	for (int i = 0; i < bar->SPHCloud.size(); i++)
	{
		SPHParticle* pi = bar->SPHCloud[i];
		pi->position_last = pi->position;
		pi->position = pi->position + DT*pi->velocity;
	}
}
void SPHSolverPCI::ApplyExternalForce(Vector2d f)
{
	for (int j = 0; j < bar->SPHCloud.size(); j++)
		bar->SPHCloud[j]->velocity += f*DT;

}
void SPHSolverPCI::PressureStep()
{
	for (int i = 0; i < bar->SPHCloud.size(); i++)
	{
		SPHParticle* pi = bar->SPHCloud[i];
		double dens = 0.0f;//密度清零
		double dens_proj = 0.0f;
		for (int j = 0; j < pi->Nh->numNeighbors; j++)
		{
			const SPHParticle& pj = *pi->Nh->particles[j];
			double r = pi->Nh->r[j];
			double a = 1. - r / H;//公式中的1-q
			dens += pj.mass * a*a*a * KERN;//邻居的贡献
			dens_proj += pj.mass * a*a*a*a * KERN_NORM;
		}
		pi->density = dens;
		pi->densityv = dens_proj;
		pi->p = STIFFNESS * (dens - pi->mass*REST_DENSITY);//p=k*(rho-m*rho0)
		pi->pv = STIFF_APPROX * dens_proj;
	}

	

	
	
}
void SPHSolverPCI::Correct()
{
	for (int i = 0; i < bar->SPHCloud.size(); i++)
	{
		SPHParticle* pi = bar->SPHCloud[i];
		pi->position = pi->position_proj;
		pi->velocity = (pi->position - pi->position_last) / DT;
	}
	
}
void SPHSolverPCI::Project()
{
	for (int i = 0; i < bar->SPHCloud.size(); i++)
	{
		SPHParticle* pi = bar->SPHCloud[i];
		Vector2d position_temp = pi->position;
		for (int j = 0; j < pi->Nh->numNeighbors; j++)
		{
			const SPHParticle& pj = *pi->Nh->particles[j];
			double r = pi->Nh->r[j];
			Vector2d dx = pj.position - pi->position;
			double a = 1.0 - r / H;
			double d = DT*DT * ((pi->pv + pj.pv)*a*a*a*KERN_NORM + (pi->p + pj.p)*a*a*KERN) / 2.;//第一个是压强项，第二个是密度项
			position_temp -= d*dx / (r*pi->mass);
			if (pi->mass == pj.mass)
				position_temp += (SURFACE_TENSION / pi->mass) * pj.mass*a*a*KERN * dx;
			Vector2d dv = pi->velocity - pj.velocity;
			double u = dv.dot(dx);
			if (u > 0)
			{
				u /= r;
				double a = 1 - r / H;
				double I = 0.5f * DT * a * (LINEAR_VISC*u + QUAD_VISC*u*u);//粘滞阻力给速度带来的impluse，这里直接换算成位置的impluse
				position_temp -= I*dx*DT;
			}
		}
		pi->position_proj = position_temp;
	}
}

void SPHSolverPCI::ParametersIn()
{
	ifstream infile;
	infile.open("SPHPCIConfig.txt");
	if (!infile.is_open())
	{
		cout << "Could Not Open File!!!";
		exit(EXIT_FAILURE);
	}
	double val;
	string st;
	infile >> st;
	while (infile.good())
	{
		if (st.compare("PARTICLE_RADIUS") == 0) {
			infile >> val;
			PARTICLE_RADIUS = val;
		}
		if (st.compare("EPS") == 0) {
			infile >> val;
			EPS = val;
		}
		if (st.compare("SURFACE_TENSION") == 0) {
			infile >> val;
			SURFACE_TENSION = val;
		}
		if (st.compare("LINEAR_VISC") == 0) {
			infile >> val;
			LINEAR_VISC = val;
		}
		if (st.compare("QUAD_VISC") == 0) {
			infile >> val;
			QUAD_VISC = val;
		}
		if (st.compare("STIFFNESS") == 0) {
			infile >> val;
			STIFFNESS = val;
		}
		if (st.compare("STIFF_APPROX") == 0) {
			infile >> val;
			STIFF_APPROX = val;
		}
		if (st.compare("REST_DENSITY") == 0) {
			infile >> val;
			REST_DENSITY = val;
		}
		infile >> st;
	}
	H = 6.0f*PARTICLE_RADIUS;
	KERN_NORM = 30. / (2.*M_PI*H*H);
	KERN = 20. / (2.*M_PI*H*H);
}