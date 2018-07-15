#pragma once
#include"Particle.h"
extern double PARTICLE_RADIUS;//���Ӱ뾶
extern double H;// smoothing radius �������Ӱ뾶�ľ���
extern double EPS;
const static Vector2d SPHg(0.0f, -9.8f);
class Neighborhood;

enum SPHParticleAdressType
{
	REAL,
	GHOST
};

class SPHParticle
	:public Particle
{
public:
	SPHParticle();
	SPHParticle(Vector2d p, int num, Vector2d v=Vector2d::Zero(),Vector3d PColor= Vector3d(0, 0.3, 0.7));
	~SPHParticle();
	Vector2d position_proj, position_last;
	double p;
	double pv;
	double densityv;
	SPHParticle* n;
	Neighborhood* Nh;
	Vector2i gridIndices;
	SPHParticleAdressType AType;
	void Update();
	constexpr const static double PARTICLE_MASS = 1.0f;//����Ҫ����
};

class Neighborhood
{
public:
	Neighborhood() : particles(MAX_NEIGHBORS), r(MAX_NEIGHBORS), numNeighbors(0) {}
	~Neighborhood() {};
	vector<SPHParticle*> particles;//�˴����κ�����Ҫ����
	vector<double> r;
	int numNeighbors;
	const static int MAX_NEIGHBORS = 100; // by grid definition
};