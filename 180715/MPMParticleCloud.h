#pragma once
#include"ParticleCloud.h"
#include"MPMParticle.h"
#include"MPMParticleSand.h"
#include"MPMParticleWater.h"
#include"MPMParticleElastic.h"
#include"MPMParticleSnow.h"
#include <fstream>  
#include <sstream>
class MPMParticleCloud
	:public ParticleCloud
{
public:
	MPMParticleCloud();
	MPMParticleCloud(PointList list, ParticleType type, Vector2d velocity, Vector3d PColor = Vector3d(-1, -1, -1));
	~MPMParticleCloud();
	vector<MPMParticle*> particles;
	void merge(MPMParticleCloud& other);
	void update();
private:
	void SandParametersIn(SandParameters &p);
	void SnowParametersIn(SnowParameters &p);
	void WaterParametersIn(WaterParameters &p);
	void ElasticParametersIn(ElasticParameters &p);

};