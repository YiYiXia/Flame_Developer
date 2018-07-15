#include"SPHParticleCloud.h"
double PARTICLE_RADIUS;//���Ӱ뾶

double H;// smoothing radius �������Ӱ뾶�ľ���
double EPS;


SPHParticleCloud::SPHParticleCloud()
{
	CELL_SIZE = H; // set to smoothing radius�����о��������������ı߳���СΪH
	GRID_WIDTH = (int)(VIEW_WIDTH / CELL_SIZE);//�о�����ˮƽ����ֱ�ж�������
	GRID_HEIGHT = (int)(VIEW_HEIGHT / CELL_SIZE);
	NUM_CELLS = GRID_WIDTH*GRID_HEIGHT;//��������
	vector<SPHParticle*> temp(NUM_CELLS);//�������ǰ���������ӣ���reshape��һά�ṹ
	grid = temp;
	p_num = 0;
	size = 0;
}

SPHParticleCloud::SPHParticleCloud(PointList list, ParticleType type, Vector2d velocity)
{
	CELL_SIZE = H; // set to smoothing radius�����о��������������ı߳���СΪH
	GRID_WIDTH = (int)(VIEW_WIDTH / CELL_SIZE);//�о�����ˮƽ����ֱ�ж�������
	GRID_HEIGHT = (int)(VIEW_HEIGHT / CELL_SIZE);
	NUM_CELLS = GRID_WIDTH*GRID_HEIGHT;//��������
	vector<SPHParticle*> temp(NUM_CELLS);//�������ǰ���������ӣ���reshape��һά�ṹ
	grid = temp;
	p_num = 0;
	size = list.size();
	for (int i = 0; i < list.size(); i++)
	{
		SPHParticle* p;
		p = new SPHParticle(list[i],p_num, velocity);
		SPHCloud.push_back(p);
		p_num++;
	}
}

SPHParticleCloud::~SPHParticleCloud()
{
}

void SPHParticleCloud::Merge(SPHParticleCloud &other)
{
	size += other.size;
	for (int i = 0; i < other.SPHCloud.size(); i++) {
		other.SPHCloud[i]->index += p_num;
	}
	p_num += other.size;
	SPHCloud.reserve(size);
	SPHCloud.insert(SPHCloud.end(), other.SPHCloud.begin(), other.SPHCloud.end());
}

void SPHParticleCloud::GridInsert()
{
	for (auto &elem : grid)//���grid�д�ŵ�����
	{
		elem = NULL;
	}

	//cout << GRID_WIDTH << endl;
	for (int i = 0; i < SPHCloud.size(); i++)
	{
		SPHParticle* p = SPHCloud[i];
		int xind = floor(p->position[0] / CELL_SIZE);
		int yind = floor(p->position[1] / CELL_SIZE);
		xind = max(1, min(GRID_WIDTH - 2, xind));//xind�����������ĩ�ˣ��������Ϊĩ��ֵ��������������ʼ�ˣ������Ϊ1.������-2����Ϊ���߽���һ������
		yind = max(1, min(GRID_HEIGHT - 2, yind));
		p->n = grid[xind + yind*GRID_WIDTH];
		grid[xind + yind*GRID_WIDTH] = p;
		p->gridIndices= Vector2i(xind, yind);
	}
}



void SPHParticleCloud::FindNeighbour()
{
	for (int i = 0; i < SPHCloud.size(); i++)
	{
		SPHParticle* p = SPHCloud[i];
		Vector2i ind = Vector2i(p->gridIndices[0], p->gridIndices[1] * GRID_WIDTH);//��ȡ��i�������ڵĸ������
		p->Nh->numNeighbors = 0;
		//p->Nh->particles.clear();
		//p->Nh->particles.shrink_to_fit();
		for (int ii = ind(0) - 1; ii <= ind(0) + 1; ii++)//��������Ѱ���ھӡ���Ϊ��reshape�ˣ�������������ѭ��
			for (int jj = ind(1) - GRID_WIDTH; jj <= ind(1) + GRID_WIDTH; jj += GRID_WIDTH)
				for (SPHParticle* pgrid = grid[ii + jj]; pgrid != NULL; pgrid = pgrid->n)//prid���������ʽ������ĳ���������е�����
				{
					SPHParticle& pj = *pgrid;
					Vector2d dx = pj.position - p->position;//�������i����ľ���
					double r2 = dx.
					squaredNorm();//dot(dx, dx)
					if (r2 < EPS*EPS || r2 > H*H)//��������⻬�뾶�������������غϣ�������������ζ�ţ��ھ��в�����i����
						continue;
					double r = sqrt(r2);
					if (p->Nh->numNeighbors < Neighborhood::MAX_NEIGHBORS)//����ھ���С������ھ��������в���
					{
						p->Nh->particles[p->Nh->numNeighbors] = &pj;//���ھӵ���Ϣ����
						p->Nh->r[p->Nh->numNeighbors] = r;//������ߵľ���
						//p->Nh->particles.push_back(&pj);//���ھӵ���Ϣ����
						//p->Nh->r.push_back(r);//������ߵľ���
						p->Nh->numNeighbors++;
					}
				}
	}
}

void SPHParticleCloud::FindNeighbour(double radius)
{
	int pat = radius / H + 2;
	//cout << pat << endl;
	for (int i = 0; i < SPHCloud.size(); i++)
	{
		SPHParticle* p = SPHCloud[i];
		Vector2i ind = Vector2i(p->gridIndices[0], p->gridIndices[1] * GRID_WIDTH);//��ȡ��i�������ڵĸ������
		p->Nh->numNeighbors = 0;
		//p->Nh->particles.clear();
		for (int ii = ind(0) - pat; ii <= ind(0) + pat; ii++)//��������Ѱ���ھӡ���Ϊ��reshape�ˣ�������������ѭ��
			for (int jj = ind(1) - pat*GRID_WIDTH; jj <= ind(1) + pat*GRID_WIDTH; jj += GRID_WIDTH) 
				for (SPHParticle* pgrid = grid[ii + jj]; pgrid != NULL; pgrid = pgrid->n)//prid���������ʽ������ĳ���������е�����
				{
					SPHParticle& pj = *pgrid;
					//if (pj.AType == GHOST)  cout << 1 << endl;
					Vector2d dx = pj.position - p->position;//�������i����ľ���
					double r = dx.norm();//dot(dx, dx)
					if (r <= 0 || r > radius)//��������⻬�뾶�������������غϣ�������������ζ�ţ��ھ��в�����i����
						continue;
					if (p->Nh->numNeighbors < Neighborhood::MAX_NEIGHBORS)//����ھ���С������ھ��������в���
					{
						//cout << pj.position[0] << " " << pj.position[1] << endl;
						p->Nh->particles[p->Nh->numNeighbors] = &pj;//���ھӵ���Ϣ����
						p->Nh->r[p->Nh->numNeighbors] = r;//������ߵľ���
						//p->Nh->particles.push_back(&pj);//���ھӵ���Ϣ����
						//p->Nh->r.push_back(r);//������ߵľ���
						p->Nh->numNeighbors++;
					}
				}
		//cout << endl;
		//cout << p->Nh->numNeighbors << endl;
	}
}

void SPHParticleCloud::EnforceBoundary()
{
	for (int i = 0; i < SPHCloud.size(); i++)
	{
		for (int j = 0; j < polygon.size(); j++) 
		{
			int co;//�����޸�����
			if (polygon[j]->type == Boundary) co = 1;
			else if (polygon[j]->type == Object) co = -1;
			else continue;
			double d = co*polygon[j]->Distance(SPHCloud[i]->position).distance;
			if (min(0., d) > -PARTICLE_RADIUS)
			{
				Vector2d v_boundary = polygon[j]->SDFveloctiy(SPHCloud[i]->position[0], SPHCloud[i]->position[1]);
				Vector2d normal = -co*polygon[j]->Gradient(SPHCloud[i]->position[0], SPHCloud[i]->position[1]);
				//SPHCloud[i]->velocity = v_boundary;
				Vector2d v_relate = SPHCloud[i]->velocity - v_boundary;
				double s = v_relate.dot(normal);
				if (s < 0)
				{
					v_relate = v_relate - s*normal;
					SPHCloud[i]->velocity = v_relate + v_boundary;
				}
				if (d > 0)	SPHCloud[i]->position += normal*(d + 0.5*PARTICLE_RADIUS);
		
			}
		}
	}
}


void SPHParticleCloud::AddGhostParticles()
{
	//cout << SPHCloud.size() << endl;
	vector<SPHParticle*> ghostlist;
	for (int i = 0; i < SPHCloud.size(); i++)
	{
		SPHParticle* p = SPHCloud[i];
		for (int j = 0; j < polygon.size(); j++)
		{
			int co;//�����޸�����
			if (polygon[j]->type == Boundary) co = 1;
			double d = co*polygon[j]->Distance(SPHCloud[i]->position).distance;

			if (d>= -0.08&&d<=0)//4444444444444444
			{
				//cout << 1 << endl;
				Vector2d normal = -co*polygon[j]->Gradient(SPHCloud[i]->position[0], SPHCloud[i]->position[1]);
				Vector2d ghost = p->position + 2 * d*normal;
				double s = p->velocity.dot(normal);
				Vector2d tangent = p->velocity - s*normal;
				Vector2d v_ghost = -s*normal + tangent;//�ܶ�ѹǿ��ʼ��
				SPHParticle* q;
				q = new SPHParticle(ghost, p_num, v_ghost);
				q->density = p->density;
				q->p = p->p;
				q->AType = GHOST;
				q->mass = p->mass;
				//�����Ӳ��������
				//int xind = floor(q->position[0] / CELL_SIZE);
				//int yind = floor(q->position[1] / CELL_SIZE);
				//xind = max(1, min(GRID_WIDTH, xind));//xind�����������ĩ�ˣ��������Ϊĩ��ֵ��������������ʼ�ˣ������Ϊ1.������-2����Ϊ���߽���һ������
				//yind = max(1, min(GRID_HEIGHT, yind));
				//q->n = grid[xind + yind*GRID_WIDTH];
				//grid[xind + yind*GRID_WIDTH] = q;
				//q->gridIndices = Vector2i(xind, yind);
				ghostlist.push_back(q);
				//SPHCloud.push_back(q);
				p_num++;
			}
		}
	}
	
	//cout << endl;
	for (int i = 0; i < ghostlist.size(); i++)
	{
		SPHCloud.push_back(ghostlist[i]);
		//cout <<i<<" "<< ghostlist[i]->position[0] << " " << ghostlist[i]->position[1] << endl;
	}
	ghostlist.clear();
	//cout << ghostlist.size() << endl;
	//cout << endl;
	//cout << ghostlist.size() << endl << endl;
}

void SPHParticleCloud::DeleteGhostParticles()
{
	
	while (SPHCloud.back()->AType == GHOST)
	{
		SPHParticle* p = SPHCloud.back();
		SPHCloud.pop_back();
		free(p);
		p_num--;
	}
	
}
