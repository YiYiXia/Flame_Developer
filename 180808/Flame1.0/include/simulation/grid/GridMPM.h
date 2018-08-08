#pragma once
#include <vector>
#include <math.h>
#include <Eigen/Dense>
#include<iostream>
#include <fstream>  
#include<sstream>
#include"../../geometry/SDF.h"
#include"../../physics/ParticleCloudMPM.h"
#include"../../math/Function.h"
using namespace std;
using namespace Eigen;
const double BSPLINE_EPSILON = 1e-64;
const int BSPLINE_RADIUS = 2;
enum MPMGridNodeType
{
	BOUNDARY,
	EMPTY,
	LIQUID
};

//���������࣬������취����
typedef struct MPMGridNode {
	double mass;
	double pressure;
	MPMGridNodeType type;
	bool active;
	bool collision;
	Vector2d velocity, velocity_new, velocity_out;
	Vector2d force;
	Vector2d x, r, r_new, p, Ls;//�����ݶȷ����õ��м����
	Vector2d Im_force;//��ʽ��
	Vector2d position;
	Vector2d normal;
	bool imp_active;
	int index;//���ı��
	int s_index;
	MPMGridNode()
	{
		collision = false;
		normal = Vector2d::Identity();
	}
} MPMGridNode;

class GridMPM
{
public:
	Vector2d start, end, size, cellsize;
	double Time;
	double Friction_c;//Ħ��ϵ������һ���ݷ�����
	vector<SDF*> polygon;//�����߽磬����
	int nodes_length;//�������
	int active_length;//��¼ϡ������С
	vector<MPMGridNode*> active_list;//��Ŵ����ĸ��
	double node_area;//�������
	MPMGridNode* nodes;
	ParticleCloudMPM* obj;
	GridMPM(Vector2d pos, Vector2d dims, Vector2d cells, ParticleCloudMPM* object);
	GridMPM(Vector2d pos, Vector2d dims,  ParticleCloudMPM* object);
	~GridMPM();

	void InitializeMass();
	void InitializeVelocities();
	void CalculateVolumes() const;
	void ExplicitForce();
	void GridVelocityUpdate();
	void UpdateVelocities();
	void CollisionGrid();//��������ײ
	void Friction();//������Ħ����
	void ImplicitVelocities();
	void RecomputeImplicitForces();
	void ApplyExternForce();
	void ParametersIn();//��������Ĳ���
	void ParticleCheck();//��Խ�����ӽ�������
	void GridCheck();//����ʽ�ⷨ�ĳ�ʼ��
	void SolvePressure();//���
	void SolveUpdate();
};