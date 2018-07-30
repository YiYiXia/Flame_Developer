#include"MarchingCube.h"


MarchingCube::MarchingCube()
{

}
MarchingCube::~MarchingCube()
{

}

void MarchingCube::Polygon(cv::Mat p)
{
	
	int col = p.cols;
	int row = p.rows;
	double cell_x = VIEW_WIDTH / col;
	double cell_y = VIEW_HEIGHT / row;
	map<int, Point*>::iterator it;//边线对应点
	Point* begin = NULL;//存放点
	Point* end = NULL;
	int num = 0;
	for (int i = 0; i < col-1; i++)
	{
		for (int j = 0; j < row-1; j++)
		{
			Vector2d p1(j, i);
			Vector2d p2(j, i + 1);
			Vector2d p3(j + 1, i + 1);
			Vector2d p4(j + 1, i);
			int t1 = 0, t2 = 0, t3 = 0, t4 = 0;
			if (p.at<cv::Vec3b>(j, i)(0) == 0) t1 = 0;
			else t1 = 1;
			if (p.at<cv::Vec3b>(j, i + 1)(0) == 0) t2 = 0;
			else t2 = 1;
			if (p.at<cv::Vec3b>(j + 1, i + 1)(0) == 0) t3 = 0;
			else t3 = 1;
			if (p.at<cv::Vec3b>(j + 1, i)(0) == 0) t4 = 0;
			else t4 = 1;
			bool key = true;
			int index;
			if (t1 + t2 == 1)
			{
				Point* p;
				it = judge.find(j*(2 * col + 1) + i);
				if (it == judge.end())
				{
					Point* new_point;
					new_point = new Point((p1 + p2) / 2);
					new_point->index = num;
					judge.insert(make_pair(j*(2 * col + 1) + i, new_point));
					check.insert(make_pair(num, new_point));
					num++;
				}
				it = judge.find(j*(2 * col + 1) + i);
				p = it->second;
				if (key == true)
				{
					begin = p;
					key = false;
				}
			}
			if (t2 + t3 == 1)
			{
				Point* p;
				it = judge.find(j*(2 * col + 1) + col + 1 + i);
				if(it == judge.end())
				{
					Point* new_point;
					new_point = new Point((p2 + p3) / 2);
					new_point->index = num;		
					judge.insert(make_pair(j*(2 * col + 1) + col + 1 + i, new_point));
					check.insert(make_pair(num, new_point));
					num++;
				}
				it = judge.find(j*(2 * col + 1) + col + 1 + i);
				p = it->second;
				if (key == true)
				{
					begin = p;
					key = false;
				}
				else
				{
					end = p;
					if (begin->p1 == NULL)	begin->p1 = end;
					else begin->p2 = end;
					if (end->p1 == NULL) end->p1 = begin;
					else end->p2 = begin;
					key = true;
				}
			}
			if (t3 + t4 == 1)
			{
				Point* p;
				it = judge.find((j + 1)*(2 * col + 1) + i);
				if (it == judge.end())
				{
					Point* new_point;
					new_point = new Point((p3 + p4) / 2);
					new_point->index = num;
					judge.insert(make_pair((j + 1)*(2 * col + 1) + i, new_point));
					check.insert(make_pair(num, new_point));
					num++;
				}
				it = judge.find((j + 1)*(2 * col + 1) + i);
				p = it->second;
				if (key == true)
				{
					begin = p;
					key = false;
				}
				else
				{
					end = p;
					if (begin->p1 == NULL)	begin->p1 = end;
					else begin->p2 = end;
					if (end->p1 == NULL) end->p1 = begin;
					else end->p2 = begin;
					key = true;
				}
			}

			if (t4 + t1 == 1)
			{
				Point* p;
				it = judge.find(j*(2 * col + 1) + col + i);
				if (it == judge.end())
				{
					Point* new_point;
					new_point = new Point((p4 + p1) / 2);
					new_point->index = num;
					judge.insert(make_pair(j*(2 * col + 1) + col + i, new_point));
					check.insert(make_pair(num, new_point));
					num++;
				}
				it = judge.find(j*(2 * col + 1) + col + i);
				p = it->second;
				if (key == true){
					begin = p;
					key = false;
				}
				else{
					end = p;
					if (begin->p1 == NULL)	begin->p1 = end;
					else begin->p2 = end;
					if (end->p1 == NULL) end->p1 = begin;
					else end->p2 = begin;
					key = true;
				}
			}
		}
	}
	Point* start;
	Point* p_now;
	Point* p_next;
	while (!check.empty())
	{
		SDF* sdf;
		sdf = new SDF();
		start = check.begin()->second;
		p_now = start;
		p_next = p_now->p1;
		sdf->Insert(p_now->c[1], p_now->c[0]);
		check.erase(p_now->index);
		while (p_next != start)
		{
			if (p_next->p1 == p_now){
				p_now = p_next;
				p_next = p_next->p2;
			}
			else{
				p_now = p_next;
				p_next = p_next->p1;
			}
			sdf->Insert(p_now->c[1] ,  p_now->c[0] );
			check.erase(p_now->index);
		}
		polygons.push_back(*sdf);
		cout << sdf->vertices.size() << endl;
	}
}