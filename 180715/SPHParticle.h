#pragma once
#include"Particle.h"
extern double PARTICLE_RADIUS;//粒子半径
extern double H;// smoothing radius 六个粒子半径的距离
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
	constexpr const static double PARTICLE_MASS = 1.0f;//后面要调整
};

class Neighborhood
{
public:
	Neighborhood() : particles(MAX_NEIGHBORS), r(MAX_NEIGHBORS), numNeighbors(0) {}
	~Neighborhood() {};
	vector<SPHParticle*> particles;//此处修饰后续需要考虑
	vector<double> r;
	int numNeighbors;
	const static int MAX_NEIGHBORS = 100; // by grid definition
};