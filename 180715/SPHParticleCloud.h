#pragma once
#include"ParticleCloud.h"
#include"SPHParticle.h"

class SPHParticleCloud
	:public ParticleCloud
{
public:
	SPHParticleCloud();
	SPHParticleCloud(PointList list, ParticleType type, Vector2d velocity);
	~SPHParticleCloud();
	vector<SPHParticle*> SPHCloud;
	vector<SDF*> polygon;//�����߽磬����
	void GridInsert();
	void EnforceBoundary();
	void FindNeighbour();
	void FindNeighbour(double radius);
	void AddGhostParticles();
	void DeleteGhostParticles();
	void Merge(SPHParticleCloud &other);
	//void ParametersIn();
	int p_num;
private:
	vector<SPHParticle*> grid;
	double CELL_SIZE; // set to smoothing radius�����о��������������ı߳���СΪH
	int GRID_WIDTH;//�о�����ˮƽ����ֱ�ж�������
	int GRID_HEIGHT;
	int NUM_CELLS;//��������
};