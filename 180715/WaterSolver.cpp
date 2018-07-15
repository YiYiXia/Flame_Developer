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
	//根据场景输入信息分配信息
	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].sdf->Live_Info.AppearTime == 0)//出现时间对上
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
			bar->merge(*grid_temp->obj);
		}
	}

}