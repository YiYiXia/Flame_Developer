#include"../../include/common/File.h"
double DT;
int insert_time;
double  r;
int iteratime, endtime;
int SOLVER_STEPS;
Vector2d gravity = Vector2d(0, -0);
double VIEW_HEIGHT;
int WINDOW_WIDTH;//显示区域
int WINDOW_HEIGHT;
double VIEW_WIDTH;
File::File()
{
	
}

File::~File()
{
}

void File::Initial()
{
	//获取当前程序路径，建立与时间同名的文件夹
	string time = getTime();
	char pack[30];
	char op[300] = "md ";
	strcpy_s(pack, time.c_str());
	_getcwd(buffer, MAXPATH);
	cout << buffer << endl;
	strcat_s(buffer, "\\");
	strcat_s(buffer, pack);
	strcat_s(op, buffer);
	system(op);
	strcat_s(buffer, "\\");
}

string File::getTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H.%M.%S", localtime(&timep));
	return tmp;
}

void File::FileOut(int frame, ParticleCloud cloud)
{
	////在目录下新建的文件夹中输出文件
	//ofstream outfile, fout;
	//string over = ".txt";
	//stringstream ss;
	//string str;
	//ss << frame;
	//ss >> str;
	//str += over;
	//str = buffer + str;
	//outfile.open(str);
	////cout << frame << endl;
	//outfile << cloud.particles.size() << endl;
	//for (int i = 0; i < cloud.particles.size(); i++)
	//{
	//	outfile << cloud.particles[i]->position(0) << " " << cloud.particles[i]->position(1) << endl;
	//	outfile << cloud.particles[i]->color(0) << " " << cloud.particles[i]->color(1) << " " << cloud.particles[i]->color(2) << endl;
	//}
	//outfile.close();
}

 
void File::FileIn(int frame, ParticleCloud &cloud)
{
	//cloud.particles.clear();
	//ifstream infile;
	//string over = ".txt";
	//stringstream ss;
	//string str;
	//int num;
	//ss << frame;
	//ss >> str;
	//str += over;
	//infile.open(str);

	//infile >> num;
	//cloud.particles.reserve(num);

	//cloud.size = num;
	//cloud.p_num = 0;
	//int size = 0;
	//while (infile.good())
	//{
	//	cloud.particles[size] = new ParticleSand();
	//	infile >> cloud.particles[size]->position(0);
	//	infile >> cloud.particles[size]->position(1);
	//	cloud.particles[size]->index = size;
	//	size++;
	//}
	//cloud.p_num = size;
	//infile.close();
}

void File::FileIn(int frame, vector<Vector2d> &pointlist)
{
	/*double x, y;
	ifstream infile;
	string over = ".txt";
	stringstream ss;
	string str;
	int num;
	ss << frame;
	ss >> str;
	str += over;
	infile.open(str);
	if (!infile) return;
	infile >> num;
	while (infile.good())
	{
		infile >> x;
		infile >> y;
		pointlist.push_back(Vector2d(x, y));
	}
	infile.close();*/
}

void File::ParametersIn()
{
	ifstream infile;
	infile.open("config/config.txt");
	if (!infile.is_open())
	{
		cout << "Could Not Open File!!!";
		exit(EXIT_FAILURE);
	}
	double val;
	int temp;
	string st;
	int num;
	infile >> st;
	while (infile.good())
	{

		if (st.compare("g") == 0)
		{
			infile >> val;
			gravity(1) = val;
		}


		if (st.compare("insert_time") == 0)
		{
			infile >> temp;
			insert_time = temp;
		}
	
		if (st.compare("r") == 0)
		{
			infile >> val;
			r = val;
		}
		if (st.compare("iteratime") == 0)
		{
			infile >> temp;
			iteratime = temp;
		}
		if (st.compare("endtime") == 0)
		{
			infile >> temp;
			endtime = temp; 
		}
		if (st.compare("SOLVER_STEPS") == 0)
		{
			infile >> temp;
			SOLVER_STEPS = temp;
		}
		if (st.compare("WINDOW_WIDTH") == 0)
		{
			infile >> temp;
			WINDOW_WIDTH = temp;
		}
		if (st.compare("WINDOW_HEIGHT") == 0)
		{
			infile >> temp;
			WINDOW_HEIGHT = temp;
		}
		if (st.compare("VIEW_WIDTH") == 0)
		{
			infile >> val;
			VIEW_WIDTH = val;
		}
		infile >> st;
	}
	if (infile.eof())
		cout << "End Of File!!";
	infile.close();
	DT = ((1.0f / fps) / SOLVER_STEPS);
	VIEW_HEIGHT = WINDOW_HEIGHT*VIEW_WIDTH / WINDOW_WIDTH;
	
}