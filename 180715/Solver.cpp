#include"Solver.h"

Solver::Solver()
{
	frames = 0;
}

Solver::~Solver()
{
}
void Solver::SceneControl()
{
	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].sdf->move == true)
		{		
			//cout << frames << endl;
			scene->SDFlist[i].sdf->Control(frames);
			scene->SDFlist[i].sdf->Update(DT);
		}
	}
}