#include"SPHSolver.h"
SPHSolver::SPHSolver()
{
}
SPHSolver::~SPHSolver()
{
}
void SPHSolver::Initial(SolverType SType)
{
	Type = SType;
	ParametersIn();
	bar = new SPHParticleCloud();

	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].stype == Boundary || scene->SDFlist[i].stype == Object || scene->SDFlist[i].stype == Rivet) {
			bar->polygon.push_back(scene->SDFlist[i].sdf);
		}
		else if (scene->SDFlist[i].stype == ParticleBoundary)
		{
			SPHParticleCloud* temp;
			PointList list;
			if (scene->SDFlist[i].sdf->Sample_Type == BOUNDSDF_O)
			{
				list = OrderSample(scene->SDFlist[i].sdf);
			}
			else
			{
				do {
					if (scene->SDFlist[i].sdf->Sample_Type == BOUNDSDF_P)
					{
						list = PossionDisc(r, scene->SDFlist[i].sdf, iteratime);
					}

				} while (list.size() < 20);
			}
			

			temp = new SPHParticleCloud(list, scene->SDFlist[i].ptype, scene->SDFlist[i].velocity);

			bar->Merge(*temp);
		}
	}
	SPHInitial();
	PointListCheck();
	
	bar->GridInsert();
}
void SPHSolver::Update()
{
	SPHUpdate();
}
void SPHSolver::PointListCheck()
{
	int psize = scene->pointlist.size();
	int csize = bar->SPHCloud.size();
	if (psize<csize)
	{
		scene->pointlist.resize(csize);
		for (int i = 0; i < bar->SPHCloud.size(); i++)
			scene->pointlist[i] = bar->SPHCloud[i];
	}
}