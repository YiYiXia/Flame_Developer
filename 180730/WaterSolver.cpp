#include"WaterSolver.h"
WaterSolver::WaterSolver()
{
}
WaterSolver::~WaterSolver()
{
}
void WaterSolver::Initial(SolverType SType)
{
	Type = SType;
	bar = new MPMParticleCloud();
	grid = new MACGrid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), Vector2d(100, 100), bar);
	grid->Initial();
	//���ݳ���������Ϣ������Ϣ
	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].sdf->Live_Info.AppearTime == 0)//����ʱ�����
		{
			if (scene->SDFlist[i].stype == Boundary || scene->SDFlist[i].stype == Object || scene->SDFlist[i].stype == Rivet) {
				grid->polygon.push_back(scene->SDFlist[i].sdf);
			}
			else if (scene->SDFlist[i].stype == ParticleBoundary)
			{
				MPMParticleCloud* temp;
				PointList list;
				if (scene->SDFlist[i].sdf->Sample_Type == BOUNDSDF_O)
				{
					list = OrderSample(scene->SDFlist[i].sdf);
				}
				else
				{
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
				}
				

				
				temp = new MPMParticleCloud(list, scene->SDFlist[i].ptype, scene->SDFlist[i].velocity, scene->SDFlist[i].color);
				MPMGrid*grid_temp;
				grid_temp = new MPMGrid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), temp);
				grid_temp->initializeMass();
				grid_temp->calculateVolumes();
				bar->merge(*grid_temp->obj);
			}
		}

	}
	PointListCheck();
	grid->CalculateNodePhi();

	//test
	
}

void WaterSolver::Update()
{
	//memset(grid->nodes, 0, sizeof(MPMGridNode)*grid->nodes_length);
	//for (int i = 0; i < grid->size[0]; i++)
	//{
	//	for (int j = 0; j < grid->size[1]; j++)
	//	{
	//		grid->nodes[(int)(j*grid->size[0] + i)].position = grid->start + Vector2d(grid->cellsize[0] * i, grid->cellsize[1] * j);
	//		grid->nodes[(int)(j*grid->size[0] + i)].index = (int)(j*grid->size[0] + i);
	//	}
	//}
	//grid->nodes[(int)(60 * grid->size[0] + 50)].velocity_new = Vector2d::Zero(); grid->nodes[(int)(60 * grid->size[0] + 50)].active = true;
	//grid->nodes[(int)(61 * grid->size[0] + 50)].velocity_new = Vector2d(0, -1); grid->nodes[(int)(61 * grid->size[0] + 50)].active = true;
	//grid->GridCheck();
	//grid->SolvePressure();

	//for (int i = 0; i < grid->active_list.size(); i++)
	//{
	//	cout << grid->active_list[i]->pressure << endl;
	//}
	//cout << endl;




	//for (int i = 0; i < SOLVER_STEPS; i++)
	//{
	//	SourceCheck();
	//	grid->initializeMass();
	//	grid->initializeVelocities();
	//	grid->GridCheck();
	//	//grid->explicitForce();
	//	 
	//	//grid->gridVelocityUpdate();

	//	for (int i = 0; i < grid->nodes_length; i++)
	//	{
	//		MPMGridNode &node = grid->nodes[i];
	//		node.velocity_new = node.velocity;
	//	}
	//	grid->ApplyExternForce();
	//	grid->SolvePressure();
	//	grid->SolveUpdate();
	//	//
	//	//if (Type == IMPLICIT) grid->ImplicitVelocities();
	//	//grid->ApplyExternForce();
	//	//grid->collisionGrid();
	//	//grid->Friction();
	//	grid->updateVelocities();
	//	//bar->update();
	//	for (int i = 0; i<bar->size; i++) {
	//		MPMParticle* p;
	//		p = bar->particles[i];
	//		p->position = p->position + p->velocity*DT;
	//	}
	//	SceneControl();
	//	PointListCheck();
	//	frames++;
	//}
	for (int i = 0; i < SOLVER_STEPS; i++)
	{
		SourceCheck();

		grid->UpdateParticle();
		grid->ComputerNeighbour();
		grid->ParticleToGrid();
		grid->AddForce();
		grid->ComputerPhi();
		grid->ComputerWeight();
		grid->GridCheck();
		grid->SolvePressure();
		//grid->Correct();
		grid->GridToParticle();
		
		SceneControl();
		PointListCheck();
		frames++;
	}
	

}

void WaterSolver::PointListCheck()
{
	int psize = scene->pointlist.size();
	int csize = bar->particles.size();
	if (psize<csize)
	{
		scene->pointlist.resize(csize);
		for (int i = 0; i < bar->particles.size(); i++)
			scene->pointlist[i] = bar->particles[i];
	}

}

void WaterSolver::SourceCheck()
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
				temp = new MPMParticleCloud(list, scene->SDFlist[i].ptype, scene->SDFlist[i].velocity, scene->SDFlist[i].color);
				MPMGrid*grid_temp;
				grid_temp = new MPMGrid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), temp);
				grid_temp->initializeMass();
				grid_temp->calculateVolumes();
				bar->merge(*grid_temp->obj);
			}
		}
	}
	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].stype == ParticleBoundary&&frames == scene->SDFlist[i].sdf->Live_Info.AppearTime&&frames > 0)//֮��������壬����������
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
			bar->merge(*grid_temp->obj);
		}
	}

}