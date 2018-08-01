/* Copyright(c) 2018 YimingXia(491691865@qq.com)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#pragma once
#define EPSILON 0.01f
#include<Eigen/Dense>
#include<iostream>
#include<vector>

using namespace Eigen;
using namespace std;
//���塢�߽硢Դ����
enum SDFType
{
	Object,
	ParticleBoundary,
	Boundary,
	Source,
	Sink,
	Rivet
};

//������ʽ
enum SampleType
{
	BOUNDSDF_P,
	PICTURE_P,
	BOUNDSDF_O
};

//��ײ����
enum SDFCollisionType
{
	Sticky,
	Separating,
	Slipping,
	None
};

//�˶�������Ϣ������˶�������Ϣ����˶����ƽű�
typedef struct SDFMoveInfo
{
	SDFMoveInfo(int st, int et, Vector2d v, double o) :StartTime(st), EndTime(et), veloctiy(v),omega(o){}
	int StartTime;//�˶���ʼʱ��
	int EndTime;//�˶���ֹʱ��
	Vector2d veloctiy;//�˶��ٶ�
	double omega;//���ٶ�
}SDFMoveInfo;

//�����Ϣ��ʲôʱ����֡���ʧ
typedef struct SDFLiveInfo 
{
	int AppearTime;
}SDFLiveInfo;

//�㵽����ε���Ϣ
typedef struct SDFinfo
{
	double distance;
	int num_s;//��֮������߶α��
	int num_e;
} SDFinfo;

double CircleSDF(double x, double y, double c_x, double c_y, double r);
double PlaneSDF(double x, double y, double px, double py, double nx, double ny);//ƽ��
double SegmentSDF(double x, double y, double sx, double sy, double ex, double ey);
double CapsuleSDF(double x, double y, double sx, double sy, double ex, double ey, double r);
double BoxSDF(double x, double y, double cx, double cy, double theta, double sx, double sy);

class SDF
{
public:
	std::vector<SDFMoveInfo> move_info;
	std::vector<Vector2d> vertices;//��Ŷ���ζ���
	std::vector<Vector2d> velocity;//��Ŷ�����ٶ�
	Vector2d v_center, v_center_new;
	Vector2d v_acceleration, v_acceleration_new;
	SDF(); 
	SDF(int t ,SampleType stype=BOUNDSDF_P, string name = " ");//���ó���ʱ��
	SDF(int x, int y);//Ϊ�˸����׷���õģ�������Ҫ����
	~SDF();

	Vector2d center;//������ͼ�ε�����λ�� 
	double mass;
	double omega;
	bool move;
	SDFType type;//SDF������
	SDFLiveInfo Live_Info;
	SampleType Sample_Type;
	string Mat_name;


	//��������SDF
	int grid_sizex,grid_sizey;
	double cell_sizex, cell_sizey;
	double* record;
	double x_min, x_max, y_min, y_max;//����ε���ӱ߿�
	Vector3d Color;

	int sample_time;
	bool sample_check;
	//SDF������
	SDFinfo Distance(Vector2d point);//����㵽����εľ��룬�ڲ�����Ϊ�����ⲿ����Ϊ��
	Vector2d Gradient(double x, double y);//����õ㴦���ݶȣ�һ��ָ��������
	int Contains(double x, double y);//�鿴ĳ���Ƿ��ڶ�����ڲ�,�ڲ�Ϊ-1���ⲿΪ1
	Vector2d SDFveloctiy(double x, double y);//��ȡ�ٽ���

	//SDF��������
	void Insert(double x, double y, Vector2d v = Vector2d::Zero());//����һ������
	void AddControl(SDFMoveInfo info);//���SDF���˶���Ϣ
	void Control(int time);//���ݿ�����Ϣ���ı��˶���Ϣ
	void Update(double DT);//���¶���ζ���λ�á��ٶ� 
	void Initialize();
	void GirdInitial();
	double GridDistance(double x, double y);
	Vector2d Rotate(Vector2d p, double theata);
private:
	
	

};

