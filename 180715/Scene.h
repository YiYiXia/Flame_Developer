#pragma once
#include"MPMGrid.h"
#include"MACGrid.h"
#include"ParticleCloud.h"
#include"Parameters.h"
#include<map>
#include"Line.h"
#include"Camera2D.h"
//这一层之后可能要改，因为要引入SPH，所以这里有很多东西就不应该出现了
using namespace std;
typedef struct SDFInfo 
{
	SDF* sdf;
	SDFType stype;
	ParticleType ptype;
	Vector2d velocity;
	Vector3d color;
};



class Scene
{
public:
	Scene(int resolution);
	~Scene();
	vector<SDFInfo> SDFlist;
	vector<Particle*> pointlist;
	vector<Line*> linelist;
	int Resolution;
	bool source;
	int frames;
	Camera camera;
	bool key;
	int startframes, endframes;


	void AddObject(SDF* polygon, SDFType Type, Vector2d velocity = Vector2d::Zero(), ParticleType PType=NONE,Vector3d ColorFill=Vector3d(-1,-1,-1));//加粒子物体的时候，默认其初始速度为零

};