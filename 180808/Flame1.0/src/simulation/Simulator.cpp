#include"../../include/simulation/Simulator.h"

Simulator::Simulator()
{
	insertTime = 0;
}

Simulator::~Simulator()
{

}

void Simulator::SimulatorInitial(Scene* scene, SolverType solverType, IntegralType integralType)
{
	sc = scene;
	//the type of child class is judged by the input information
	if (solverType == MPM) sol = new SolverMPM();
	
	//���������ĳ�ʼ����ͬʱ���������͸�ֵ����������Ϣ�͸������
	sol->SolverInitial(integralType, &scene->SDFlist);
	//���һ��ʼ�����ӳ�ʼ���������ʼ������Ҳ���Էֳ�����������Դ��ʱ����õ�
	for (int i = 0; i < sc->SDFlist.size(); i++)
	{
		if (sc->SDFlist[i].sdf->Live_Info.AppearTime == 0)
		{
			if (sc->SDFlist[i].stype == ParticleBoundary)
			{
				PointList list;
				do {
					if (sc->SDFlist[i].sdf->Sample_Type == PICTURE_P)
					{
						cv::Mat record = cv::imread(sc->SDFlist[i].sdf->Mat_name);
						list = PossionDisc(r, record, sc->SDFlist[i].sdf, iteratime);
					}
					if (sc->SDFlist[i].sdf->Sample_Type == BOUNDSDF_P)
					{
						list = PossionDisc(r, sc->SDFlist[i].sdf, iteratime);
					}
					if (sc->SDFlist[i].sdf->Sample_Type == BOUNDSDF_O)
					{
						list = OrderSample(sc->SDFlist[i].sdf);
					}
				} while (list.size() < 20);
				sol->AddParticle(&list, sc->SDFlist[i].ptype, sc->SDFlist[i].velocity, sc->SDFlist[i].color);
			}
		}
	}


}

void Simulator::SimulatorUpdate()
{
	for (int i = 0; i < SOLVER_STEPS; i++) 
	{
		InformationCheck();
		sol->SolverUpdate();
		sc->SceneUpdate();
		InformationUpdate();
		Flame_Frames++;
	}
	
}

void Simulator::InformationUpdate()
{
	int csize = sol->particleList.size();
	sc->pointlist.resize(csize);
	for (int i = 0; i < sol->particleList.size(); i++)
		sc->pointlist[i] = sol->particleList[i];
}

void Simulator::InformationCheck()
{
	if (Flame_Frames < endtime)
	{
		for (int i = 0; i < sc->SDFlist.size(); i++)
		{
			if (sc->SDFlist[i].stype == Source && (Flame_Frames - insertTime) == sc->SDFlist[i].sdf->sample_time) {
				PointList list;
				do {
					list = PossionDisc(r, sc->SDFlist[i].sdf, iteratime);
				} while (list.size() < 30);
				sol->AddParticle(&list, sc->SDFlist[i].ptype, sc->SDFlist[i].velocity, sc->SDFlist[i].color);
				insertTime = Flame_Frames;
				sc->SDFlist[i].sdf->sample_check = true;
			}
		}
	}
	for (int i = 0; i < sc->SDFlist.size(); i++)
	{
		if (sc->SDFlist[i].stype == ParticleBoundary&&Flame_Frames == sc->SDFlist[i].sdf->Live_Info.AppearTime&&Flame_Frames > 0)//֮��������壬����������
		{
			PointList list;
			do {
				if (sc->SDFlist[i].sdf->Sample_Type == PICTURE_P)
				{
					cv::Mat record = cv::imread(sc->SDFlist[i].sdf->Mat_name);
					list = PossionDisc(r, record, sc->SDFlist[i].sdf, iteratime);
				}
				if (sc->SDFlist[i].sdf->Sample_Type == BOUNDSDF_P)
				{
					list = PossionDisc(r, sc->SDFlist[i].sdf, iteratime);
				}
			} while (list.size() < 20);
			sol->AddParticle(&list, sc->SDFlist[i].ptype, sc->SDFlist[i].velocity, sc->SDFlist[i].color);
		}
	}
}