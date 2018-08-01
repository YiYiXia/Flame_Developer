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
#include<time.h>
#include<iostream>
#include<Eigen/Dense>
#include<algorithm>
using namespace Eigen;
double RandomNumber(double lo, double hi);
double BSpline(double x);
double BSplineSlope(double x);
double LinearKernel(Vector2d d,double cell);
double FractionInside(double phi_left, double phi_right);
double Clamp(double a, double s, double e);
double Lerp(Vector2d p, double *a,int width);
double LerpLine(double s,double e,double w);
Vector2d GradBiLerp(double v00, double v10, double v01, double v11, double fx, double fy);
Vector2d AffineLerp(Vector2d p, double *a, int width);
double SmoothKernel(double r, double h);
