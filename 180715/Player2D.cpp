#include"Player2D.h"

Player2D::Player2D()
{
}
Player2D::~Player2D()
{

}
void Player2D::Initial(int argc, char** argv)
{
	srand((int)time(0));
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutInit(&argc, argv);  //��GLUT���г�ʼ�����������������������GLUTʹ��֮ǰ����һ��
	glutCreateWindow("MPMElastic");
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//������ʾ��ʽ������GLUT_RGB��ʾʹ��RGB��ɫ����֮��Ӧ�Ļ���GLUT_INDEX����ʾʹ��������ɫ����GLUT_SINGLE��ʾʹ�õ����壬��֮��Ӧ�Ļ���GLUT_DOUBLE��ʹ��˫���壩
	glClearColor(0.9f, 0.9f, 0.9f, 1);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(3);
	glMatrixMode(GL_PROJECTION);
}

void Player2D::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glOrtho(0, VIEW_WIDTH, 0, VIEW_HEIGHT, 0, 1);

	glFlush();
}

void Player2D::Update()
{
	glutPostRedisplay();
}

void Player2D::DrawSDF(SDF sdf)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glOrtho(0, VIEW_WIDTH, 0, VIEW_HEIGHT, 0, 1);
	glColor3f(1, 0.1, 0.2);
	sdf.Draw();
	glFlush();
}
void Player2D::DrawSurface()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glOrtho(0, VIEW_WIDTH, 0, VIEW_HEIGHT, 0, 1);
	glColor3f(1, 0.1, 0.2);
	double cell_x = VIEW_WIDTH / render->width;
	double cell_y = VIEW_HEIGHT / render->height;
	Vector2d begin, end;
	for (int i = 0; i < render->width - 1; i++)
	{
		for (int j = 0; j < render->height - 1; j++)
		{
			Vector2d p1(j, i);
			Vector2d p2(j, i + 1);
			Vector2d p3(j + 1, i + 1);
			Vector2d p4(j + 1, i);
			int t1 = 0, t2 = 0, t3 = 0, t4 = 0;
			
			if (render->canvas.at<cv::Vec3b>(j, i)(0) == 0) t1 = 0;
			else t1 = 1;
			if (render->canvas.at<cv::Vec3b>(j, i + 1)(0) == 0) t2 = 0;
			else t2 = 1;
			if (render->canvas.at<cv::Vec3b>(j + 1, i + 1)(0) == 0) t3 = 0;
			else t3 = 1;
			if (render->canvas.at<cv::Vec3b>(j + 1, i)(0) == 0) t4 = 0;
			else t4 = 1;
			bool key = true;
			if (t1 + t2 == 1)
			{
				if (key == true)
				{
					begin = (p1 + p2) / 2;
					key = false;
				}
			}
			if (t2 + t3 == 1)
			{
				if (key == true)
				{
					begin = (p2 + p3) / 2; 
					key = false;
				}
				else
				{
					end = (p2 + p3) / 2;
					glLineWidth(2);
					glBegin(GL_LINES);
					glVertex2f(cell_x*begin[1], VIEW_HEIGHT - cell_y*begin[0]);
					glVertex2f(cell_x*end[1], VIEW_HEIGHT - cell_y*end[0]);
					glEnd();
					key = true;
				}
			}
			if (t3 + t4 == 1)
			{
				if (key == true)
				{
					begin = (p3 + p4) / 2;
					key = false;
				}
				else
				{
					end = (p3 + p4) / 2;
					glLineWidth(2);
					glBegin(GL_LINES);
					glVertex2f(cell_x*begin[1], VIEW_HEIGHT - cell_y*begin[0]);
					glVertex2f(cell_x*end[1], VIEW_HEIGHT - cell_y*end[0]);
					glEnd();
					key = true;
				}
			}

			if (t4 + t1 == 1)
			{
				if (key == true)
				{
					begin = (p4 + p1) / 2;
					key = false;
				}
				else
				{
					end = (p4 + p1) / 2;
					glLineWidth(2);
					glBegin(GL_LINES);
					glVertex2f(cell_x*begin[1], VIEW_HEIGHT - cell_y*begin[0]);
					glVertex2f(cell_x*end[1], VIEW_HEIGHT - cell_y*end[0]);
					glEnd();
					key = true;
				}
			}
		}
	}

	glFlush();
}
