#pragma once
#include <Eigen/Dense>
#include<tbb/task_scheduler_init.h>
#include<tbb/blocked_range.h>
#include<tbb/parallel_for.h>
extern int SOLVER_STEPS;
const static int fps = 40;
extern double DT;//每一步的时长CFL条件
extern int WINDOW_WIDTH;//显示区域
extern int WINDOW_HEIGHT;
extern double VIEW_WIDTH;//研究区域，二者要成比例
extern double VIEW_HEIGHT;
extern int insert_time;
extern double  r;
extern int iteratime, endtime;
extern Eigen::Vector2d gravity;

#define M_PI 3.14159265358979