#pragma once
#include"Particle.h"

#include"Parameters.h"
#include <GL/freeglut.h>
#include<opencv2/opencv.hpp>
#include<time.h>
#include"SDF.h"
#include <fstream>  
#include<sstream>
#include"LinkList.h"
#include <Eigen/Dense>
#include <list>
#include"Math.h"
using namespace std;
using namespace Eigen;


typedef vector<Vector2d> PointList;
PointList OrderSample(SDF* polygon);
PointList PossionDisc(double r, SDF* polygon, int k);
PointList PossionDisc(double r, cv::Mat picture, SDF* polygon, int k);
bool MatContain(cv::Mat picture, double scalex, double scaley, Vector2d p);//判定点是否在图像内
class ParticleCloud 
{
public:
	int size;
	int p_num;
	ParticleCloud();
	ParticleCloud(int cloud_size);
	virtual~ParticleCloud();
};

