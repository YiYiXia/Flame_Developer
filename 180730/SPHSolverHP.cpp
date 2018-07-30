#include"SPHSolverHP.h"

int res;					// grid res
double dx;					// grid dx width
double h;					// particle search radius
double dt;					// time step length
double rho0;				// initial rho0
double gamma;				// gamma
double g;				// gravity
double c;					// speed of sound
double supp_size;			// support size
double water_width;			// width of water
double water_height;		// height of water
double dom_size;			// size of simulation domain
double end_t;				// end time
int frame_num;				// number of frame
int temp_num;				// temporary number counter (for density reinit)
double frame_dt;			// frame dt


SPHSolverHP::SPHSolverHP()
{
	n_real = 0;
	pre_n_real = 0;
}

SPHSolverHP::~SPHSolverHP()
{
}

void SPHSolverHP::ParametersIn()
{
	ifstream myfile;
	string s, str;
	int i, j;
	myfile.open("SPHHPconfig.txt");

	// get rho0 from file
	str.clear();
	getline(myfile, s);
	for (i = 8; i < s.length(); i++)
		str.push_back(s[i]);
	rho0 = stod(str);

	// get gamma from file
	str.clear();
	getline(myfile, s);
	for (i = 6; i < s.length(); i++)
		str.push_back(s[i]);
	gamma = stod(str);

	// get gravity from file
	str.clear();
	getline(myfile, s);
	for (i = 8; i < s.length(); i++)
		str.push_back(s[i]);
	g = stod(str);

	// get support size from file
	str.clear();
	getline(myfile, s);
	for (i = 10; i < s.length(); i++)
		str.push_back(s[i]);
	supp_size = stod(str);

	// get dx from file
	str.clear();
	getline(myfile, s);
	for (i = 8; i < s.length(); i++)
		str.push_back(s[i]);
	dx = stod(str);

	h = supp_size * dx;

	// get domain size from file
	str.clear();
	getline(myfile, s);
	for (i = 9; i < s.length(); i++)
		str.push_back(s[i]);
	dom_size = stod(str);
	res = ceil(dom_size / dx);

	// get water size from file
	str.clear();
	getline(myfile, s);
	for (i = 12; i < s.length(); i++)
		str.push_back(s[i]);
	water_width = stod(str);
	str.clear();
	getline(myfile, s);
	for (i = 13; i < s.length(); i++)
		str.push_back(s[i]);
	water_height = stod(str);

	c = 10 * sqrt(2 * g * water_height);
	//c = 10 * sqrt(2 * g * 1.0);
	// get frame dt from file
	str.clear();
	getline(myfile, s);
	for (i = 11; i < s.length(); i++)
		str.push_back(s[i]);
	int frame_rate = stoi(str);
	frame_num = 0;
	frame_dt = 1.0 / double(frame_rate);

	// get dt from file
	str.clear();
	getline(myfile, s);
	for (i = 8; i < s.length(); i++)
		str.push_back(s[i]);
	dt = stod(str);
	temp_num = 0;

	// get end_t from file
	str.clear();
	getline(myfile, s);
	for (i = 6; i < s.length(); i++)
		str.push_back(s[i]);
	end_t = stod(str);

	myfile.close();
	H = dx;
	EPS = 0.0000001;
	PARTICLE_RADIUS = h;
	t = 0;
	
}

void SPHSolverHP::SPHInitial()
{
	bar_pre = new SPHParticleCloud();
	bar_pre->size = bar->SPHCloud.size();
	for (int i = 0; i < bar->SPHCloud.size(); i++)
	{
		SPHParticle* p = bar->SPHCloud[i];
		p->density = rho0;
		p->mass = rho0*dx*dx;
		p->p = 0;
	}
	for (int i = 0; i < bar->SPHCloud.size(); i++)
	{
		SPHParticle* p = bar->SPHCloud[i];
		SPHParticle* q;
		q = new SPHParticle(p->position, p->index, p->velocity);
		q->density = rho0;
		q->mass = rho0*dx*dx;
		q->p = 0;
		bar_pre->SPHCloud.push_back(q);
	}
	bar_pre->p_num = bar->p_num;
	/*for (int i = 0; i < bar_pre->SPHCloud.size(); i++)
	{
		SPHParticle* p = bar_pre->SPHCloud[i];
		p->density = rho0;
		p->mass = rho0*dx*dx;
		p->p = 0;
	}*/
	for (int i = 0; i < bar->polygon.size(); i++)
	{
		bar_pre->polygon.push_back(bar->polygon[i]);
	}
	pre_n_real = n_real = bar->SPHCloud.size();
}

void SPHSolverHP::Predictor()
{
	double drho_dt;
	Vector2d dv_dt;
	//º∆À„—π«ø
	for (int i = 0; i < n_real; i++)
		bar->SPHCloud[i]->p = equation_of_state(bar->SPHCloud[i]->density);
	
	bar->AddGhostParticles();
	bar->GridInsert();
	
	/* evolve particle rho0 */
	bar->FindNeighbour(3 * h);

	for (int i = 0; i < n_real; i++)
	{
		SPHParticle* pi = bar->SPHCloud[i];
		drho_dt = 0;
		//if (pi->density < 1e-12) cout << "error" << endl;
		//cout << pi->position[0] << " " << pi->position[1] << " " << pi->Nh->numNeighbors << endl;
		for (int j = 0; j < pi->Nh->numNeighbors; j++)
		{
			const SPHParticle& pj = *pi->Nh->particles[j];
			drho_dt += drhodt_increment(*pi, pj);
			//cout << pj.position[0] << " " << pj.position[1] << endl;
			//cout << drho_dt << endl;
		}
		//cout << i << "  " << pi->density << "  " << drho_dt << endl;
		bar_pre->SPHCloud[i]->density = pi->density + drho_dt * dt / 2;
	}
	//cout << endl;
	/* evolve particle velocity */
	for (int i = 0; i < n_real; i++)
	{
		SPHParticle* pi = bar->SPHCloud[i];
		dv_dt = Vector2d(0.0, -g);
		for (int j = 0; j < pi->Nh->numNeighbors; j++)
		{
			const SPHParticle& pj = *pi->Nh->particles[j];
			dv_dt += dvdt_increment(*pi, pj);

		}
	//	cout << i << "  " << dv_dt[0] << "  " << dv_dt[1] << endl;
		bar_pre->SPHCloud[i]->velocity = pi->velocity + dv_dt * dt / 2;
		bar_pre->SPHCloud[i]->position = pi->position + pi->velocity * dt / 2;
	
	}
//	cout <<111<< endl;
	//PointListCheck();
	bar->DeleteGhostParticles();

}
void SPHSolverHP::Corrector() 
{
	double drho_dt;
	Vector2d dv_dt, dw;

	for (int i = 0; i < n_real; i++)
		bar_pre->SPHCloud[i]->p = equation_of_state(bar_pre->SPHCloud[i]->density);
	
	bar_pre->AddGhostParticles();
	bar_pre->GridInsert();
	
	bar_pre->FindNeighbour(3 * h); 
	/* evolve particle rho0 */
	for (int i = 0; i < n_real; i++)
	{
		SPHParticle* qi = bar_pre->SPHCloud[i];
		drho_dt = 0;
		//if (qi->density < 1e-12) cout << "error" << endl;
		for (int j = 0; j < qi->Nh->numNeighbors; j++)
		{
			const SPHParticle& qj = *qi->Nh->particles[j];
			drho_dt += drhodt_increment(*qi, qj);
		}
		//cout << i << "  " << drho_dt << endl;
		//cout << drho_dt << endl;
		bar->SPHCloud[i]->density += drho_dt * dt;
		// density >= 1
		//if (pts_sys.particles[i].rho < 1)
		//	pts_sys.particles[i].rho = 1;
	}

	/* evolve particle velocity */
	for (int i = 0; i < n_real; i++)
	{
		SPHParticle* qi = bar_pre->SPHCloud[i];
		dv_dt = Vector2d(0.0, -g);
		for (int j = 0; j < qi->Nh->numNeighbors; j++)
		{
			const SPHParticle& qj = *qi->Nh->particles[j];
			dv_dt += dvdt_increment(*qi, qj);
		}
		bar->SPHCloud[i]->velocity += dv_dt * dt;
		bar->SPHCloud[i]->position += bar_pre->SPHCloud[i]->velocity * dt ;
		
	} 
	// pts_sys.handleCollisions();
	bar_pre->DeleteGhostParticles();
//	cout << n_real << endl;
	for (int i = 0; i < n_real; i++)
		bar->SPHCloud[i]->p = equation_of_state(bar->SPHCloud[i]->density);
}

void SPHSolverHP::DensityReinit()
{

	double m_sum, v_sum, w;
	bar->GridInsert();
	bar->FindNeighbour(3 * h);
	for (int i = 0; i < n_real; i++)
	{
		w = weightFunction(Vector2d(0, 0));
		m_sum = bar->SPHCloud[i]->mass * w;
		v_sum = bar->SPHCloud[i]->mass / bar->SPHCloud[i]->density * w;

		SPHParticle* pi = bar->SPHCloud[i];
		for (int j = 0; j < pi->Nh->numNeighbors; j++)
		{
			const SPHParticle& pj = *pi->Nh->particles[j];
			w = weightFunction(pi->position - pj.position);
			m_sum += pj.mass * w;
			v_sum += pj.mass / pj.density * w;
		}

		pi->density = m_sum / v_sum;
	}
}



void SPHSolverHP::SPHUpdate()
{
	double tmp_dt = dt;
	if (t + dt > frame_num * frame_dt)
	{
		//writeData(frame_num);
		//writeDataa(frame_num);
		if (t < frame_num * frame_dt)
			dt = frame_num * frame_dt - t;
		frame_num++;
	}
	Predictor();
	Corrector();
	t += dt;
	temp_num++;
	if (temp_num == 20)
	{
		//densityReinit();
		temp_num = 0;
	}
	dt = tmp_dt;
	cout << t << endl;
}




double weightFunction(Vector2d x)					// weight function
{
	double s = x.norm() / h;
	double c = 1.0 / ((Pi * h * h) * (1.0 - 10.0 * exp(-9.0)));
	if (s < 3)
		return c * (exp(-s * s) - exp(-9.0));
	else
		return 0;
}

Vector2d weightGradient(Vector2d x)				// weight gradient
{
	double r = x.norm();
	double s = r / h;
	double e = 1.0 / ((Pi * h * h) * (1.0 - 10.0 * exp(-9.0)));
	if (s > 0 && s < 3)
		return e * (-2 * s * exp(-s * s)) * x / (r*h);
	else
		return Vector2d(0, 0);
}

double equation_of_state(double rho)
{
	return rho0 * c * c / gamma * (pow(rho / rho0, gamma) - 1.0);
}

double drhodt_increment(SPHParticle p1, SPHParticle p2)
{
	Vector2d dw = weightGradient(p1.position - p2.position);

	return p2.mass * p1.density / p2.density * (p1.velocity - p2.velocity).dot(dw);
}

double artificial_viscosity(SPHParticle p1, SPHParticle p2)
{
	double phi = 0.1 * h;
	double mu = h * (p1.velocity - p2.velocity).dot(p1.position - p2.position) /
		((p1.position - p2.position).dot(p1.position- p2.position) + phi * phi);
	double alpha = 0.01;
	double beta = 1.0;
	double rho_ij = (p1.density + p2.density) / 2;
	return (-alpha * c * mu + beta * mu * mu) / rho_ij;
}

Vector2d boundary_force(Vector2d x1, Vector2d x2, double r0)
{
	double r = (x1 - x2).norm();
	double q = r0 / r;
	double D = 0.01;
	if (q >= 1)
		return D * (pow(q, 4) - pow(q, 2)) * (x1 - x2) / (r * r);
	else
		return Vector2d(0, 0);
}

Vector2d dvdt_increment(SPHParticle p1, SPHParticle p2)
{
	Vector2d dw = weightGradient(p1.position - p2.position);
	Vector2d incre = -p2.mass * (p1.p + p2.p) / (p1.density * p2.density) * dw;
	// Vector2d incre = -p2.m * (p1.p / (p1.rho * p1.rho) + p2.p / (p2.rho * p2.rho) ) * dw;
	if (p2.AType == REAL)
		incre -= p2.mass * artificial_viscosity(p1, p2) * dw;
	if (p2.AType == GHOST)
		incre += boundary_force(p1.position, p2.position, dx);
	return incre;
}