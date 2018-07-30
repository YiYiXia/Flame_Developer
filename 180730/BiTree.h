#pragma once
#include<iostream>
#include<iomanip>
using namespace std;

struct TreePoint
{
	float key;                                                 //��ֵ
	TreePoint* right;                                          //�ҽڵ�
	TreePoint* left;                                           //��ڵ�
	TreePoint* p;                                              //ǰ��
	int num;
};

class BiTree 
{
public:
	BiTree();
	~BiTree();
	void Insert(TreePoint &z);
	TreePoint Min();
	TreePoint Max();
	void PreorderTreeWalk(TreePoint &x);
	TreePoint* Root_;
private:
	TreePoint* Nil_;
};