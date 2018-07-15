#include<iostream>
#include<math.h>
#include<time.h>
#include<conio.h>
#include"Parameters.h"
#include"SDF.h"
#include"Player2D.h"
#include"ParallelRender.h"
#include"File.h"
#include"Scene.h"
#include"Render2D.h"
#include"MarchingCube.h"
#include"LinkList.h"
#include"MPMSolver.h"
#include"SPHSolver.h"
#include"MPMMultiSolver.h"
#include"WaterSolver.h"
using namespace std;
int main()
{
	File file;
	file.ParametersIn();
	Scene sc(150);
	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH, SHOW);
	Solver* solve;
	solve = new WaterSolver();
	/*SDF a(0);
	a.Insert(4.7, 1.85);
	a.Insert(4.8, 1.85);
	a.Insert(4.8, 1.95);
	a.Insert(4.7, 1.95);
	sc.AddObject(&a, Source, Vector2d(-2.0, 0), WATER);*/

	SDF a(0);
	a.Insert(0.7, 0.8);
	a.Insert(1.7, 0.8);
	a.Insert(1.7, 1.8);
	a.Insert(0.7, 1.8);
	sc.AddObject(&a, ParticleBoundary, Vector2d(-0.0, 0), WATER);

	SDF c(0);
	c.Insert(0.2, 0.2);
	c.Insert(0.2, 4.8);
	c.Insert(4.8, 4.8);
	c.Insert(4.8, 0.2);
	sc.AddObject(&c, Boundary);
	
	solve->scene = &sc;
	solve->Initial(EXPLICIT);
	
	int i = 0;
	while (1)
	{
		solve->Update();
		render.CanvasDraw(&sc);
		//render.CanvasOutput(i);
		render.CanvasShow();
		render.CanvasClear();
		i++;
	}
}
//int main(int argc, char** argv)
//{
//	File file;
//	file.ParametersIn();
//	Scene sc(150);
//	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH, SHOW);
//	Solver* solve;
//	solve = new MPMMultiSolver();
//	SDF a(0);
//	a.Insert(4.7, 1.85);
//	a.Insert(4.8, 1.85);
//	a.Insert(4.8, 1.95);
//	a.Insert(4.7, 1.95);
//	sc.AddObject(&a, Source, Vector2d(-2.0, 0), WATER);
//	SDF b(0);
//	b.Insert(2.25, 0.70);
//	b.Insert(3.05, 0.70);
//	b.Insert(3.05, 1.70);
//	b.Insert(2.25, 1.70);
//	sc.AddObject(&b, ParticleBoundary, Vector2d(0, 0), WETSAND);
//	SDF c(0);
//	c.Insert(0.2, 0.2);
//	c.Insert(0.2, 2.4);
//	c.Insert(4.8, 2.4);
//	c.Insert(4.8, 0.2);
//	sc.AddObject(&c, Boundary);
//	SDF d(0);
//	d.Insert(4.3, 0.25);
//	d.Insert(4.3, 1.5);
//	d.Insert(4.1, 1.5);
//	d.Insert(4.1, 0.65);
//	d.Insert(2.2, 0.65);
//	d.Insert(1.8, 0.25);
//	sc.AddObject(&d, Object);
//	solve->scene = &sc;
//	solve->Initial(EXPLICIT);
//
//	int i = 0;
//	while (1)
//	{
//		solve->Update();
//		render.CanvasDraw(&sc);
//		//render.CanvasOutput(i);
//		render.CanvasShow();
//		render.CanvasClear();
//		i++;
//	}
//
//}



//int main(int argc, char** argv)
//{
//
//
//	File file;
//	file.ParametersIn();
//	Scene sc(150);
//	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH, SHOW);
//	Solver* solve;
//	solve = new MPMSolver();
//
//
//	SDF b(0, BOUNDSDF_P);
//	b.Insert(1.20, 2.50);
//	b.Insert(3.20, 2.50);   
//	b.Insert(3.20, 3.50);
//	b.Insert(1.20, 3.50);
//	sc.AddObject(&b, ParticleBoundary, Vector2d(0, 0.0), ELASTIC);
//
//	SDF a(0);
//	a.Insert(1.00, 2.30);
//	a.Insert(1.40, 2.30);
//	a.Insert(1.40, 3.70);
//	a.Insert(1.00, 3.70);
//	//a.Initialize();
//	sc.AddObject(&a, Rivet);
//	a.AddControl(SDFMoveInfo(0, 800, Vector2d(-0.5, 0), 0));
//	a.AddControl(SDFMoveInfo(800, 1600, Vector2d(0, 0), 0));
//
//	SDF d(0);
//	d.Insert(3.00, 2.30);
//	d.Insert(3.40, 2.30);
//	d.Insert(3.40, 3.70);
//	d.Insert(3.00, 3.70);
//	//a.Initialize();
//	sc.AddObject(&d, Rivet);
//	d.AddControl(SDFMoveInfo(0, 800, Vector2d(0.5, 0), 0));
//	d.AddControl(SDFMoveInfo(800, 1600, Vector2d(0, 0), 0));
//
//	SDF c(0);
//	c.Insert(0.3, 0.3);
//	c.Insert(0.3, 4.7);
//	c.Insert(4.7, 4.7);
//	c.Insert(4.7, 0.3);
//	sc.AddObject(&c, Boundary);
//
//	solve->scene = &sc;
//	solve->Initial(EXPLICIT);
//
//	cv::waitKey(0);
//	int i = 0;
//	while (1)
//	{
//		solve->Update();
//		render.CanvasDraw(&sc);
//		//render.CanvasOutput(i);
//		render.CanvasShow();
//		render.CanvasClear();
//		//sc.camera.look_at += Vector2d(0,-0.08);
//		i++;
//		cout << i << endl;
//	}
//
//}


//int main(int argc, char** argv)
//{
//	File file;
//	file.ParametersIn();
//	Scene sc(150);
//	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH, SHOW);
//	Solver* solve;
//	solve = new MPMMultiSolver();
//	SDF a(0);
//	a.Insert(4.7, 1.75);
//	a.Insert(4.8, 1.75);
//	a.Insert(4.8, 1.90);
//	a.Insert(4.7, 1.90);
//	sc.AddObject(&a, Source, Vector2d(-2.0, 0), WATER);
//	SDF b(0);
//	b.Insert(2.25, 0.70);
//	b.Insert(3.05, 0.70);
//	b.Insert(3.05, 1.70);
//	b.Insert(2.25, 1.70);
//	sc.AddObject(&b, ParticleBoundary, Vector2d(0, 0), WETSAND);
//	SDF c(0);
//	c.Insert(0.2, 0.2);
//	c.Insert(0.2, 2.4);
//	c.Insert(4.8, 2.4);
//	c.Insert(4.8, 0.2);
//	sc.AddObject(&c, Boundary);
//	SDF d(0);
//	d.Insert(4.3, 0.25);
//	d.Insert(4.3, 1.5);
//	d.Insert(4.1, 1.5);
//	d.Insert(4.1, 0.65);
//	d.Insert(2.2, 0.65);
//	d.Insert(1.8, 0.25);
//	sc.AddObject(&d, Object);
//	solve->scene = &sc;
//	solve->Initial(EXPLICIT);
//
//	int i = 0;
//	while (1)
//	{
//		solve->Update();
//		render.CanvasDraw(&sc);
//		//render.CanvasOutput(i);
//		render.CanvasShow();
//		render.CanvasClear();
//		//sc.camera.look_at += Vector2d(0,-0.08);
//		i++;
//	}
//
//}


//#include<iostream>
//#include<math.h>
//#include<time.h>
//#include"Parameters.h"
//#include"SDF.h"
//#include"Player2D.h"
//#include"ParallelRender.h"
//#include"File.h"
//#include"Scene.h"
//#include"Render2D.h"
//#include"MarchingCube.h"
//#include"LinkList.h"
//#include"MPMSolver.h"
//#include"SPHSolverPCI.h"
//#include"BoxSolver.h"  
//#include"MPMMultiSolver.h"
//using namespace std;
//int main(int argc, char** argv)
//{
//
//
//	File file;
//	file.ParametersIn();
//	Scene sc(150);
//	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH, SHOW);
//	Solver* solve;
//	solve = new SPHSolverPCI();
//	//solve = new BoxSolver();
//	SDF a(0);
//	a.Insert(6.0, 1.5);
//	a.Insert(1.5, 1.5);
//	a.Insert(1.5, 5.5);
//	a.Insert(6.0, 5.5);
//	sc.AddObject(&a, ParticleBoundary);
//	SDF c(0);
//	c.Insert(0.8, 0.8);
//	c.Insert(0.8, 9.2);
//	c.Insert(9.2, 9.2);
//	c.Insert(9.2, 0.8);
//	sc.AddObject(&c, Boundary);
//	solve->scene = &sc;
//	solve->Initial(EXPLICIT);
//
//	int i = 0;
//	while (1)
//	{
//		solve->Update();
//		render.CanvasDraw(&sc);
//		//render.CanvasOutput(i);
//		render.CanvasShow();
//		render.CanvasClear();
//		i++;
//	}
//
//}









