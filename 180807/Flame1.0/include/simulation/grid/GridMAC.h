#pragma once
#include <Eigen/Dense>
#include<vector>
#include"../../common/Parameters.h"
#include"../../physics/ParticleCloudMPM.h"

using namespace std;
using namespace Eigen;

enum MACNodeType
{
	FLUID,
	SOLID,
	MACNONE
};
//
typedef struct MACGridNode
{
	double pressure;
	double mass;
	double density;
	double liquid_phi;//��¼���ӵ����ĵ���̾���
	Vector2d velocity;
	Vector2d position;
	MACNodeType type;
	vector<ParticleMPM*> particles;//��Ÿ���е�����
	int index;//���ı��
	int index_x, index_y;
	int s_index;
	MACGridNode()
	{
		mass = 0;
	}
}MACGridNode;

class GridMAC
{
public:
	GridMAC(Vector2d pos, Vector2d dims, Vector2d cells);
	GridMAC(Vector2d pos, Vector2d dims, Vector2d cells, ParticleCloudMPM* object);
	~GridMAC();
	MACGridNode* nodes;
	vector<SDF*> polygon;//�����߽磬����
	Vector2d start, end, size, cellsize;//start�Ǹ�����ĵ���ʼλ�á�
	int nodes_length;//�������
	double *velocity_u;//��������������
	double *velocity_v;
	double *u_weights;
	double *v_weights;
	double *nodal_solid_phi;//��¼�Ӹ�㵽�߽�ľ���
	ParticleCloudMPM* obj;//���Ӵ��
	vector<MACGridNode*> active_list;//��Ŵ����ĸ��
	int active_length;
	double delta_t;
	void AddObject();
	void AddForce();
	//void CalculateCenter();
	void CalculateNodePhi();
	void ComputerPhi();
	void ComputerWeight();
	void ComputerNeighbour();
	void Correct();
	void GridCheck();
	void GetNeighbour(int i, int j, int wl, int wh, int hl, int hh, vector<ParticleMPM*> &neighbour);
	void Initial();
	void SolvePressure();
	//void Transport();
	//void Update();
	void UpdateParticle();
	void ParticleToGrid(); 
	void GridToParticle();
	void CollsionHandle();
	Matrix2d ComputerAffineMatrix(Vector2d p);
	//void SolvePressure();
};