#include"BoxSolver.h"

BoxSolver::BoxSolver()
{

}

BoxSolver::~BoxSolver()
{

}

void BoxSolver::Initial(SolverType SType)
{
	Type = SType;
}

void BoxSolver::Update()
{
	for (int i = 0; i < SOLVER_STEPS; i++)
	{
		ApplyForce();
		for (int i = 0; i < scene->SDFlist.size(); i++)
		{
			if (scene->SDFlist[i].stype != Object) continue;
			scene->SDFlist[i].sdf->Update(DT);
		}
		CollisionCheck();
		for (int i = 0; i < scene->SDFlist.size(); i++)
		{
			if (scene->SDFlist[i].stype != Object) continue;
			scene->SDFlist[i].sdf->v_center = scene->SDFlist[i].sdf->v_center_new;
		}
	}
}
void BoxSolver::ApplyForce()
{
	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].stype != Object) continue;
		scene->SDFlist[i].sdf->v_center += DT*Vector2d(0, -2.0);
	}

	//弹簧受力
	for (int i = 0; i < scene->linelist.size(); i++)
	{
		scene->linelist[i]->CalculateForce();
		Vector2d l_d = scene->linelist[i]->start_di;//以绳子起始处方向标记为绳子方向
		double v_s = l_d.dot(scene->linelist[i]->start->v_center);//
		double v_e = l_d.dot(scene->linelist[i]->end->v_center);
		double co = 0;
		if (scene->linelist[i]->start->type == Object)
		{
			//cout << "force" << scene->linelist[i]->force / scene->linelist[i]->end->mass << endl;
			scene->linelist[i]->start->v_center += DT*scene->linelist[i]->start_di*scene->linelist[i]->force / scene->linelist[i]->start->mass;
			scene->linelist[i]->start->v_center -= DT*(v_s - v_e)*l_d*scene->linelist[i]->damping;
			//cout << "v" << scene->linelist[i]->end->v_center << endl;
		}
		if (scene->linelist[i]->end->type == Object)
		{
		   
			scene->linelist[i]->end->v_center += DT*scene->linelist[i]->end_di*scene->linelist[i]->force / scene->linelist[i]->end->mass;
			scene->linelist[i]->end->v_center -= DT*(v_e - v_s)*l_d*scene->linelist[i]->damping;
		}
		
	}

}
void BoxSolver::CollisionCheck()
{
	
	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		
		if (scene->SDFlist[i].stype != Object) continue;
		scene->SDFlist[i].sdf->v_center_new = scene->SDFlist[i].sdf->v_center;
		for (int j = 0; j < scene->SDFlist.size(); j++)
		{
			int co = 0;
			if (j == i)continue;
			if (scene->SDFlist[j].stype == Boundary)co = 1;
			if (scene->SDFlist[j].stype == Object)co = -1;
			
			//多边形分点接触与面接触
			int index_st = -1, index_nd = -1;
			double max_dist_st = -999999, max_dist_nd = -999999;
			//找A到B最近的顶点
			for (int k = 0; k < scene->SDFlist[i].sdf->vertices.size(); k++)
			{
				double d = co*scene->SDFlist[j].sdf->Distance(scene->SDFlist[i].sdf->vertices[k]).distance;
				if (d > max_dist_st)
				{
					//第一次信息转给下一个
					index_nd = index_st;
					max_dist_nd = max_dist_st;
					max_dist_st = d;
					index_st = k;
				}
				else
				{
					if (d >= max_dist_nd)
					{
						max_dist_nd = d;
						index_nd = k;
					}
				}

			}
			if (max_dist_st < -0.05) continue;
			Vector2d v_boundary;
			Vector2d normal;
			if (max_dist_st == max_dist_nd)
			{
				Vector2d m_point = (scene->SDFlist[i].sdf->vertices[index_st] + scene->SDFlist[i].sdf->vertices[index_nd]) / 2.0;
				v_boundary = scene->SDFlist[j].sdf->SDFveloctiy(m_point[0], m_point[1]);
	/*			if (scene->SDFlist[j].stype == Boundary)co = -1;
				if (scene->SDFlist[j].stype == Object)co = 1;*/
				normal = -scene->SDFlist[i].sdf->Gradient(m_point[0], m_point[1]);
				//cout << normal << endl;
			}
			else
			{
				v_boundary = scene->SDFlist[j].sdf->SDFveloctiy(scene->SDFlist[i].sdf->vertices[index_st][0], scene->SDFlist[i].sdf->vertices[index_st][1]);
				normal = -co*scene->SDFlist[j].sdf->Gradient(scene->SDFlist[i].sdf->vertices[index_st][0], scene->SDFlist[i].sdf->vertices[index_st][1]);
			}
			
			Vector2d v_relate = scene->SDFlist[i].sdf->v_center - v_boundary;
			double s = v_relate.dot(normal);
			if (s < 0.0)
			{
				Vector2d v_normal = s*normal;
				v_relate = v_relate -  1.1*v_normal;
				//cout << normal << endl << endl;
				scene->SDFlist[i].sdf->v_center_new = v_relate + v_boundary;
			}
			
			//cout << v_boundary << endl;

			//包围盒相交检测
			//int keyx = 0, keyy = 0;
			//if ((scene->SDFlist[i].sdf->x_min > scene->SDFlist[j].sdf->x_min&&scene->SDFlist[i].sdf->x_min < scene->SDFlist[j].sdf->x_max) ||
			//	(scene->SDFlist[i].sdf->x_max > scene->SDFlist[j].sdf->x_min&&scene->SDFlist[i].sdf->x_max < scene->SDFlist[j].sdf->x_max))
			//	keyx = 1;
			//if ((scene->SDFlist[i].sdf->y_min > scene->SDFlist[j].sdf->y_min&&scene->SDFlist[i].sdf->y_min < scene->SDFlist[j].sdf->y_max) ||
			//	(scene->SDFlist[i].sdf->y_max > scene->SDFlist[j].sdf->y_min&&scene->SDFlist[i].sdf->y_max < scene->SDFlist[j].sdf->y_max))
			//	keyy = 1;

			//cout << i<<" "<<keyx << " " << keyy << " " << co << endl;

			//if (((scene->SDFlist[i].sdf->x_min > scene->SDFlist[j].sdf->x_min&&scene->SDFlist[i].sdf->x_min<scene->SDFlist[j].sdf->x_max) ||
			//	(scene->SDFlist[i].sdf->x_max>scene->SDFlist[j].sdf->x_min&&scene->SDFlist[i].sdf->x_max < scene->SDFlist[j].sdf->x_max))&&
			//	((scene->SDFlist[i].sdf->y_min > scene->SDFlist[j].sdf->y_min&&scene->SDFlist[i].sdf->y_min<scene->SDFlist[j].sdf->y_max) ||
			//		(scene->SDFlist[i].sdf->y_max>scene->SDFlist[j].sdf->y_min&&scene->SDFlist[i].sdf->y_max < scene->SDFlist[j].sdf->y_max)))
			//{
			//	//if (co == -1) cout << 11111 << endl;
			//	int index = -1;
			//	double max_dist = -999999;
			//	//找A到B最近的顶点
			//	for (int k = 0; k < scene->SDFlist[i].sdf->vertices.size(); k++)
			//	{
			//		double d = co*scene->SDFlist[j].sdf->Distance(scene->SDFlist[i].sdf->vertices[k]).distance;
			//		if (d > max_dist)
			//		{
			//			max_dist = d;
			//			index = k;
			//		}
			//	}
			//	if (max_dist < -0.05) continue;
			//	Vector2d v_boundary = scene->SDFlist[j].sdf->SDFveloctiy(scene->SDFlist[i].sdf->vertices[index][0], scene->SDFlist[i].sdf->vertices[index][1]);
			//	Vector2d v_relate = scene->SDFlist[i].sdf->v_center - v_boundary;
			//	Vector2d normal = -co*scene->SDFlist[j].sdf->Gradient(scene->SDFlist[i].sdf->vertices[index][0], scene->SDFlist[i].sdf->vertices[index][1]);
			//	double s = v_relate.dot(normal);
			//	if (s <= 0.0)
			//	{ 
			//		Vector2d v_normal = s*normal;
			//		v_relate = v_relate - 2*v_normal;
			//	}
			//	scene->SDFlist[i].sdf->v_center_new = v_relate + v_boundary;
			//}
		}
	}
}