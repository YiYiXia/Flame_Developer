#pragma once
#include"Scene.h"

//������������ͣ���ʽ����ʽ������Ҫ������ʽ�Ĳ����ѳ���
enum SolverType
{
	EXPLICIT,
	IMPLICIT
};
class Solver
{
public:
	Solver();
	~Solver();
	Scene* scene;
	int frames;
	SolverType Type;//��¼���������ʽ����
	void SceneControl();//������ݶ�����˶���Ϣ���������˶���
	virtual void Initial(SolverType SType) = 0;
	virtual void Update() = 0;
};