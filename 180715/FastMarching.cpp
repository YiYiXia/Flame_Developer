#include"FastMarching.h"

FastMarching::FastMarching(int _x, int _y, float _h)
	:x(_x+2),y(_y+2),h(_h),boundary(10000), num_open(0)
{
	Initial();
	record = cv::Mat::zeros(x, y, CV_8UC3);
}

FastMarching::~FastMarching()
{
}

float max(float a, float b)
{
	return (a > b) ? a : b;
}

float min(float a, float b)
{
	return (a > b) ? b : a;
}
/*��ʼ��ʮ�������ϵ*/
void FastMarching::Initial()
{
	points = (Point*)malloc(x*y * sizeof(Point));
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			points[j*x + i].i = i;
			points[j*x + i].j = j;
			points[j*x + i].n = j*x + i;
			if (i == 0 || i == x - 1 || j == 0 || j == y - 1)//�߽����������
				points[j*x + i].s = BOUNDARY;
			else
				points[j*x + i].s = FARER;
			points[j*x + i].f = 1000000;
			points[j*x + i].u = NULL;
			points[j*x + i].d = NULL;
			points[j*x + i].l = NULL;
			points[j*x + i].r = NULL;
			if (i != 0)	points[j*x + i].l = &points[j*x + i - 1];
			if (i != x - 1)	points[j*x + i].r = &points[j*x + i + 1];
			if (j != 0) points[j*x + i].d = &points[(j - 1)*x + i];
			if (j != y - 1) points[j*x + i].u = &points[(j + 1)*x + i];
		}
	}
}
/*��ĳ��Ԫ�ش�open���Ƴ�*/
void FastMarching::Delet(int n)
{
	vector<Point*>::iterator p;
	for (p = open.begin(); p != open.end();)
	{
		if ((*p)->n == n)
		{
			open.erase(p);
			break;
		}
		else p++;
	}
}

/*�ö���������open�е�����Ԫ�ؽ�������*/
void FastMarching::Sort()
{
	BiTree t;
	vector<Point*>::iterator p;
	for (p = open.begin(); p != open.end(); p++)
	{
		TreePoint *tp;
		tp = new TreePoint();
		tp->key = (*p)->f;
		tp->num = (*p)->n;
		t.Insert(*tp);
	}
	TreePoint out = t.Min();
	nmin = out.num;
	out = t.Max();
	dmax = out.key;
}


/*f(x)�ļ��㣬������1�Ǽ�����볡�������滻�������ٶȳ����������Լ��������ʱ��ֲ�*/
float FastMarching::Functionx(int i, int j)
{
	return 1;
}

/*����Eikonal Equation����p������ֵ��һ�׾���*/
void FastMarching::Distance(Point &p)
{
	float ua;
	float uc;
	/*���ұȽ�ȡ��С��*/
	if (p.l->s == BOUNDARY)
	{
		ua = min(boundary, p.r->f);
	}
	else
	{
		if (p.r->s == BOUNDARY)
			ua = min(p.l->f, boundary);
		else
			ua = min(p.l->f, p.r->f);
	}
	/*���±Ƚ�ȡ��С��*/
	if (p.u->s == BOUNDARY)
	{
		uc = min(boundary, p.d->f);
	}
	else
	{
		if (p.d->s == BOUNDARY)
			uc = min(p.u->f, boundary);
		else
			uc = min(p.u->f, p.d->f);
	}
	int i = p.i,j = p.j;
	float record = p.f;
	float fx = Functionx(i, j);
	float di = 2 * h*h / (fx*fx) - (ua - uc)*(ua - uc);
	if (di < 0)
		p.f = min(ua, uc) + h / fx;
	else
		p.f = (ua + uc + sqrt(di)) / 2;
	p.f = min(p.f, record);//������������ֵ��֮ǰ��С��ȡ֮
}
/*������ֵ*/
void FastMarching::Caculatedistance()
{

	while (num_open != 0)//���open���ǿգ�open����narrow band
	{
		int recent = nmin;
		points[recent].s = DEAD;
		num_open--;
		Delet(recent);
		if (points[recent].l->s != BOUNDARY&&points[recent].l->s == FARER)
		{
			points[recent].l->s = OPEN;
			open.push_back(points[recent].l);
			num_open++;
		}
		if (points[recent].r->s != BOUNDARY&&points[recent].r->s == FARER)
		{
			points[recent].r->s = OPEN;
			open.push_back(points[recent].r);
			num_open++;
		}
		if (points[recent].u->s != BOUNDARY&&points[recent].u->s == FARER)
		{
			points[recent].u->s = OPEN;
			open.push_back(points[recent].u);
			num_open++;
		}
		if (points[recent].d->s != BOUNDARY&&points[recent].d->s == FARER)
		{
			points[recent].d->s = OPEN;
			open.push_back(points[recent].d);
			num_open++;
		}
		if (points[recent].l->s != BOUNDARY&&points[recent].l->s == OPEN) Distance(*points[recent].l);
		if (points[recent].r->s != BOUNDARY&&points[recent].r->s == OPEN) Distance(*points[recent].r);
		if (points[recent].u->s != BOUNDARY&&points[recent].u->s == OPEN) Distance(*points[recent].u);
		if (points[recent].d->s != BOUNDARY&&points[recent].d->s == OPEN) Distance(*points[recent].d);
		if (num_open >= 1) Sort();//ÿ����һ�Σ��������򣬸�����Сֵ�����
	}
	Image();
}

/*��ʼ�����������Ϣ*/
void FastMarching::AddStart(int start_x, int start_y, int end_x, int end_y)
{
	for (int i = start_x; i < end_x + 1; i++)
		for (int j = start_y; j < end_y + 1; j++)
		{
			points[j*x + i].s = OPEN;
			open.push_back(&points[j*x + i]);
			num_open++;
			points[j*x + i].f = 0;
		}
	Sort();
}

/*��ӱ߽��ϰ�����Ϣ*/
void FastMarching::AddBoundary(int start_x, int start_y, int end_x, int end_y)
{
	for (int i = start_x; i < end_x + 1; i++)
		for (int j = start_y; j < end_y + 1; j++)
			points[j*x + i].s = BOUNDARY;
}

void FastMarching::SearchPath(int ix, int jx)
{
	int nx = ix;
	int ny = jx;
	int tx, ty;
	while (points[ny*x + nx].f != 0)
	{
		record.at<cv::Vec3b>(nx, ny) = cv::Vec3b(0, 0, 255);
		float d = 1000000,ted;
		for (int  i= -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (points[(ny + j)*x + nx + i].s == BOUNDARY)
				{
					continue;
				}

				if (points[(ny + j)*x + nx + i].f < d)
				{
					d = points[(ny + j)*x + nx + i].f;
					tx = nx + i;
					ty = ny + j;
				}
				//if ((i != 0 && j == 0) || (i == 0 && j != 0))
				//{ 
				//	ted = points[ny*x + nx].f - points[(ny + j)*x + nx + i].f;
				//	if (0< ted < d)
				//	{
				//		d = ted;
				//		tx = nx + i;
				//		ty = ny + j;
				//	}
				//}
				//if (i != 0 && j != 0)
				//{
				//	ted = points[ny*x + nx].f - points[(ny + j)*x + nx + i].f;
				//	//ted /= sqrt(2);
				//	if (0< ted < d)
				//	{
				//		d = ted;
				//		tx = nx + i;
				//		ty = ny + j;
				//	}
				//}
				
			}
		}
		nx = tx;
		ny = ty;
	}
}


/*�������ֵ���Թ�����ʹ��*/
float FastMarching::Value(int i, int j)
{
	if (points[j*x + i].s == BOUNDARY)
		return -1;
	else
		return points[j*x + i].f;
}

/*��ӡ���и���ֵ������Debug*/
void FastMarching::Display()
{
	for (int j = 0; j < y; j++)
	{
		for (int i = 0; i < x; i++)
		{
			cout << setw(8) << points[j*x + i].f << "  ";
		}
		cout << endl;
	}
}


/*�����ת��Ϊͼ����ʾ����*/
void FastMarching::Image()
{
	float v;
	int o;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			v = points[j*x + i].f;
			if (points[j*x + i].s == BOUNDARY)
			{
				record.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
			}
			else
			{
				v /= dmax;
				int num = int(v / 0.25);
				switch (num)
				{
				case 0:
					o = int(v * 255 / 0.25);
					record.at<cv::Vec3b>(i, j) = cv::Vec3b(255, o, 0);
					break;
				case 1:
					o = int((0.5 - v) * 255 / 0.25);
					record.at<cv::Vec3b>(i, j) = cv::Vec3b(o, 255, 0);
					break;
				case 2:
					o = int((v - 0.5) * 255 / 0.25);
					record.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 255, o);
					break;
				case 3:
					o = int((1 - v) * 255 / 0.25);
					record.at<cv::Vec3b>(i, j) = cv::Vec3b(0, o, 255);
				case 4:
					record.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 255, 255);
					break;
				default:
					break;
				}
				if (v == 0.0)
				{
					record.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255);
				}
			}
		}
	}
}

void FastMarching::Show()
{
	cv::namedWindow("1",CV_WINDOW_NORMAL);
	cv::imshow("1", record);
	cv::waitKey(0);
}