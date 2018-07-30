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
	vector<SDF*> polygon;//交互边界，物体
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
	double CELL_SIZE; // set to smoothing radius，对研究区域分网格，网格的边长大小为H
	int GRID_WIDTH;//研究区域水平、竖直有多少网格
	int GRID_HEIGHT;
	int NUM_CELLS;//网格数量
};