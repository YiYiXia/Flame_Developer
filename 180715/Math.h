#pragma once
#include<time.h>
#include<iostream>
#include<Eigen/Dense>
#include<algorithm>
using namespace Eigen;
double random_number(double lo, double hi);
double bspline(double x);
double bsplineSlope(double x);
double linearkernel(Vector2d d,double cell);
double fraction_inside(double phi_left, double phi_right);
double clamp(double a, double s, double e);
double lerp(Vector2d p, double *a,int width);
double lerp_line(double s,double e,double w);
Vector2d grad_bilerp(double v00, double v10, double v01, double v11, double fx, double fy);
Vector2d affine_lerp(Vector2d p, double *a, int width);
double smooth_kernel(double r, double h);
