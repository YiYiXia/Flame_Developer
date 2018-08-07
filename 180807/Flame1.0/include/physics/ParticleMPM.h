#pragma once
#include"Particle.h"

class ParticleMPM
	:public Particle
{
public:
	ParticleMPM();
	virtual~ParticleMPM();
	virtual Matrix2d EnergyDerivative() = 0;
	virtual void MPMUpdate() = 0;
	virtual void DeltaForce() = 0;//��ʽ�����ã��Ժ����Ҫ�ع�
	void Update() { MPMUpdate(); }
	bool volume_check;
	Matrix2d def_elastic;//�����α��ݶ�
	Matrix2d def_plastic;//�����α��ݶ�
	Matrix2d svd_u, svd_v, svd_s;
	Matrix2d polar_r, polar_s;//���ֽ���
	Matrix2d velocity_gradient;//�ٶ��ݶ�
	Matrix2d B;//APIC�еķ������
	Vector2d grid_position;
	Vector2d weight_gradient[16];
	double coh;//���ڿ�������ģ������ع�
	double weights[16];//����õ㵽��Χ16������B����Ȩ��ֵ
	Matrix2d delta_v;
	Matrix2d delta_f;
};