#include"MPMMultiSolver.h"
MPMMultiSolver::MPMMultiSolver()
{
}
MPMMultiSolver::~MPMMultiSolver()
{
}
void MPMMultiSolver::Initial(SolverType SType)
{
	Type = SType;
	water = new MPMParticleCloud();
	grid_w = new MPMGrid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), water);
	grid_w->initializeMass();
	grid_w->calculateVolumes();

	sand = new MPMParticleCloud();
	grid_s = new MPMGrid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), sand);
	grid_s->initializeMass();
	grid_s->calculateVolumes();
	//根据场景输入信息分配信息
	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].sdf->Live_Info.AppearTime == 0)//出现时间对上
		{
			if (scene->SDFlist[i].stype == Boundary || scene->SDFlist[i].stype == Object) {
				grid_w->polygon.push_back(scene->SDFlist[i].sdf);
				grid_s->polygon.push_back(scene->SDFlist[i].sdf);
			}
			else if (scene->SDFlist[i].stype == ParticleBoundary)
			{
				MPMParticleCloud* temp;
				PointList list;
				do {
					if (scene->SDFlist[i].sdf->Sample_Type == PICTURE_P)
					{
						cv::Mat record = cv::imread(scene->SDFlist[i].sdf->Mat_name);
						list = PossionDisc(r, record, scene->SDFlist[i].sdf, iteratime);
					}
					if (scene->SDFlist[i].sdf->Sample_Type == BOUNDSDF_P)
					{
						list = PossionDisc(r, scene->SDFlist[i].sdf, iteratime);
					}
				} while (list.size() < 20);

				temp = new MPMParticleCloud(list, scene->SDFlist[i].ptype, scene->SDFlist[i].velocity);
				MPMGrid*grid_temp;
				grid_temp = new MPMGrid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), temp);
				grid_temp->initializeMass();
				grid_temp->calculateVolumes();
				if (scene->SDFlist[i].ptype == WETSAND) sand->merge(*grid_temp->obj);
				if (scene->SDFlist[i].ptype == WATER) water->merge(*grid_temp->obj);
			}
		}
		
	}
	PointListCheck();
	cohesion_w = new double[grid_s->nodes_length];
}

void MPMMultiSolver::Update()
{
	for (int i = 0; i < SOLVER_STEPS; i++)
	{
		SourceCheck();
		grid_s->initializeMass();
		grid_s->initializeVelocities();
		grid_s->explicitForce();
		grid_w->initializeMass();
		grid_w->initializeVelocities();
		grid_w->explicitForce();

		MomentaExchange();
		Saturation();
		grid_s->collisionGrid();
		grid_s->Friction();
		grid_s->updateVelocities();
		sand->update();
		grid_w->collisionGrid();
		grid_w->Friction();
		grid_w->updateVelocities();
		water->update();
		SceneControl();
		PointListCheck();

		frames++;
	}
}

void MPMMultiSolver::PointListCheck()
{
	int psize = scene->pointlist.size();
	int csize = sand->particles.size() + water->particles.size();
	if (psize<csize)
	{
		scene->pointlist.resize(csize);
		for (int i = 0; i < sand->particles.size(); i++)
			scene->pointlist[i] = sand->particles[i];
		for (int i = sand->particles.size(); i < csize; i++)
			scene->pointlist[i] = water->particles[i- sand->particles.size()];
	}
}

void MPMMultiSolver::SourceCheck()
{
	if (frames % insert_time == 0 && frames < endtime)
	{
		for (int i = 0; i < scene->SDFlist.size(); i++)
		{
			if (scene->SDFlist[i].stype == Source) {

				MPMParticleCloud* temp;
				PointList list;
				do {
					list = PossionDisc(r, scene->SDFlist[i].sdf, iteratime);
				} while (list.size() < 30);
				cout << "Num of particle:" << list.size() << endl;
				temp = new MPMParticleCloud(list, scene->SDFlist[i].ptype, scene->SDFlist[i].velocity);
				MPMGrid*grid_temp;
				grid_temp = new MPMGrid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), temp);
				grid_temp->initializeMass();
				grid_temp->calculateVolumes();
				//cout << grid_temp->obj->particles[0]->velocity << endl;
				if (scene->SDFlist[i].ptype == WETSAND) sand->merge(*grid_temp->obj);
				if (scene->SDFlist[i].ptype == WATER) water->merge(*grid_temp->obj);
			}
		}
	}

	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].stype == ParticleBoundary&&frames == scene->SDFlist[i].sdf->Live_Info.AppearTime&&frames > 0)//之后插入物体，有三个条件
		{
			MPMParticleCloud* temp;
			PointList list;
			do {
				if (scene->SDFlist[i].sdf->Sample_Type == PICTURE_P)
				{
					cv::Mat record = cv::imread(scene->SDFlist[i].sdf->Mat_name);
					list = PossionDisc(r, record, scene->SDFlist[i].sdf, iteratime);
				}
				if (scene->SDFlist[i].sdf->Sample_Type == BOUNDSDF_P)
				{
					list = PossionDisc(r, scene->SDFlist[i].sdf, iteratime);
				}
			} while (list.size() < 20);

			temp = new MPMParticleCloud(list, scene->SDFlist[i].ptype, scene->SDFlist[i].velocity, scene->SDFlist[i].color);
			MPMGrid*grid_temp;
			grid_temp = new MPMGrid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), temp);
			grid_temp->initializeMass();
			grid_temp->calculateVolumes();
			if (scene->SDFlist[i].ptype == WETSAND) sand->merge(*grid_temp->obj);
			if (scene->SDFlist[i].ptype == WATER) water->merge(*grid_temp->obj);
		}
	}
}

void MPMMultiSolver::MomentaExchange()
{
	double C_e;
	
	memset(cohesion_w, 0, sizeof(double)*grid_s->nodes_length);

	for (int i = 0; i < grid_s->nodes_length; i++)
	{
		if (grid_s->nodes[i].active&&grid_w->nodes[i].active)
		{
			//cout << grid_w->nodes[i].mass << endl;
			C_e = -grid_w->nodes[i].mass * 0.3;
			cohesion_w[i] = 1;
			Matrix2d M = Matrix2d::Zero();
			M(0, 0) = grid_s->nodes[i].mass;
			M(1, 1) = grid_w->nodes[i].mass;
			Matrix2d D = Matrix2d::Zero();
			double Delement = C_e*grid_s->nodes[i].mass*grid_w->nodes[i].mass;
			D(0, 0) = -Delement;
			D(0, 1) = Delement;
			D(1, 0) = Delement;
			D(1, 1) = -Delement;

			Matrix2d V = Matrix2d::Zero();
			V(0, 0) = grid_s->nodes[i].velocity[0];
			V(0, 1) = grid_s->nodes[i].velocity[1];
			V(1, 0) = grid_w->nodes[i].velocity[0];
			V(1, 1) = grid_w->nodes[i].velocity[1];

			Matrix2d G = Matrix2d::Zero();
			G(0, 0) = 0;
			G(0, 1) =-3.0;
			G(1, 0) = 0;
			G(1, 1) = -3.0;

			Matrix2d F = Matrix2d::Zero();
			F(0, 0) = grid_s->nodes[i].force[0];
			F(0, 1) = grid_s->nodes[i].force[1];
			F(1, 0) = grid_w->nodes[i].force[0];
			F(1, 1) = grid_w->nodes[i].force[1];

			Matrix2d A = M + DT*D;
			Matrix2d B = M*V + DT*(M*G - F);
			Matrix2d X = A.inverse()*B;
			grid_s->nodes[i].velocity_new = Vector2d(X(0, 0), X(0, 1));
			grid_w->nodes[i].velocity_new = Vector2d(X(1, 0), X(1, 1));
			//cout << A.inverse() << endl;
		}
		else if(grid_s->nodes[i].active)
		{
			grid_s->nodes[i].velocity_new = grid_s->nodes[i].velocity + DT*(gravity - grid_s->nodes[i].force / grid_s->nodes[i].mass);
		}
		else if (grid_w->nodes[i].active)
		{
			grid_w->nodes[i].velocity_new = grid_w->nodes[i].velocity + DT*(gravity - grid_w->nodes[i].force / grid_w->nodes[i].mass);
		}
	}
	
}

void MPMMultiSolver::Saturation()
{
	//memset(cohesion_s, 0, sizeof(double)*sand->size);
	for (int i = 0; i < sand->size; i++)
	{
		MPMParticle* p = sand->particles[i];
		p->coh = 0;
		int ox = p->grid_position[0], oy = p->grid_position[1];
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				double w = p->weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					int index = (int)(y*grid_s->size[0] + x);
					p->coh += cohesion_w[index] * w;
				}
			}
		}

	}
}