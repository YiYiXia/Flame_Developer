#include"../../../include/simulation/solver/SolverMPM.h"

SolverMPM::SolverMPM()
{

}

SolverMPM::~SolverMPM()
{

}

void SolverMPM::SolverInitial(IntegralType SType, vector<SDFInfo>* SDFlist)
{
	Type = SType;
	objList = SDFlist;
	bar = new ParticleCloudMPM();
	grid = new GridMPM(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), bar);
	grid->InitializeMass();
	grid->CalculateVolumes();
	InsertTime = 0;
	for (int i = 0; i < (*objList).size(); i++)
	{
		if ((*objList)[i].sdf->Live_Info.AppearTime == 0)
		{
			if ((*objList)[i].stype == Boundary || (*objList)[i].stype == Object || (*objList)[i].stype == Rivet || (*objList)[i].stype == Sink) {
				grid->polygon.push_back((*objList)[i].sdf);
			}
		}
	}
}

void SolverMPM::AddParticle(vector<Vector2d> *particleList, ParticleType particleType, Vector2d particleVelocity, Vector3d particleColor)
{
	ParticleCloudMPM* temp;
	temp = new ParticleCloudMPM(*particleList, particleType, particleVelocity, particleColor);
	GridMPM*grid_temp;
	grid_temp = new GridMPM(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), temp);
	grid_temp->InitializeMass();
	grid_temp->CalculateVolumes();
	bar->Merge(*grid_temp->obj);
	free(temp);
	free(grid_temp);
}

void SolverMPM::SolverUpdate()
{
	grid->InitializeMass();
	grid->InitializeVelocities();
	grid->ExplicitForce();
	grid->GridVelocityUpdate();
	if (Type == IMPLICIT) grid->ImplicitVelocities();
	grid->ApplyExternForce();
	grid->CollisionGrid();
	grid->Friction();
	grid->UpdateVelocities();
	grid->ParticleCheck();
	bar->Update();
	InformationReturn();
}

void SolverMPM::InformationReturn()
{
	int csize = bar->particles.size();
	particleList.resize(csize);
	for (int i = 0; i < bar->particles.size(); i++)
		particleList[i] = bar->particles[i];
}

