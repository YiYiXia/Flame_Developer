#pragma once
#include"MPMGrid.h"
#include"MACGrid.h"
#include"ParticleCloud.h"
#include"Parameters.h"
#include<map>
#include"Line.h"
#include"Camera2D.h"
//��һ��֮�����Ҫ�ģ���ΪҪ����SPH�����������кܶණ���Ͳ�Ӧ�ó�����
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


	void AddObject(SDF* polygon, SDFType Type, Vector2d velocity = Vector2d::Zero(), ParticleType PType=NONE,Vector3d ColorFill=Vector3d(-1,-1,-1));//�����������ʱ��Ĭ�����ʼ�ٶ�Ϊ��

};