#include"MACGrid.h"
#include<iostream>
#include <Eigen/Sparse>
typedef Eigen::SparseMatrix<double> SparseMatrixType;
MACGrid::MACGrid(Vector2d pos, Vector2d dims, Vector2d cells)
{
	start = pos;
	end = dims;
	cellsize[0] = (dims[0] - pos[0]) / cells[0];//格点横向长度
	cellsize[1] = (dims[1] - pos[1]) / cells[1];//格点竖直方向长度
	nodes_length = (cells[0] + 1)*(cells[1] + 1);
	size = cells + Vector2d(1, 1);
	nodes = new MACGridNode[nodes_length];
	velocity_u = new double[(size[0] + 1) * size[1]];
	velocity_v = new double[size[0] * (size[1] + 1)];
}

MACGrid::MACGrid(Vector2d pos, Vector2d dims, Vector2d cells, MPMParticleCloud* object)
{
	start = pos;
	end = dims;
	cellsize[0] = (dims[0] - pos[0]) / cells[0];//格点横向长度
	cellsize[1] = (dims[1] - pos[1]) / cells[1];//格点竖直方向长度
	delta_t = DT;
	size = cells + Vector2d(1, 1);
	nodes_length = size[0] * size[1];
	nodes = new MACGridNode[nodes_length];
	velocity_u = new double[(size[0] + 1) * size[1]];
	velocity_v = new double[size[0] * (size[1] + 1)];
	u_weights = new double[(size[0] + 1) * size[1]];
	v_weights = new double[size[0] * (size[1] + 1)];
	nodal_solid_phi = new double[(size[0] + 1) *(size[1] + 1)];
	obj = object;
}

MACGrid::~MACGrid()
{

}

void MACGrid::Initial()
{
	
	//此处之后换接口
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			int idx = j*size[0] + i;
			nodes[idx].pressure = 0;
			nodes[idx].mass = 0;
			nodes[idx].type = MACNONE;
			nodes[idx].velocity = Vector2d::Zero();
			nodes[idx].density = 0;
			nodes[idx].position = start + Vector2d(cellsize[0] * i, cellsize[1] * j);
			nodes[idx].index = idx;
			nodes[idx].index_x = i; 
			nodes[idx].index_y = j;
			//cout <<idx<<"  "<< nodes[idx].position[0] << "  " << nodes[idx].position[1] << std::endl;
		}
	}
	for (int i = 0; i < (size[0] + 1) * size[1]; i++)	velocity_u[i] = 0;
	for (int i = 0; i < (size[0] + 1) * size[1]; i++)	u_weights[i] = 0;
	for (int i = 0; i < size[0] * (size[1] + 1); i++)	velocity_v[i] = 0;
	for (int i = 0; i < size[0] * (size[1] + 1); i++)	v_weights[i] = 0;

	for (int i = 0; i < (size[0] + 1) * (size[1] + 1); i++)	nodal_solid_phi[i] = 0;
}



void MACGrid::AddForce()
{
	double g = -9.8;
	double dt = delta_t;
	for (int i = 0; i < size[0] * (size[1] + 1); i++)
	{
		velocity_v[i] = velocity_v[i] + g*dt;
		//cout << velocity_v[i] << endl;
	}
}



void MACGrid::CalculateNodePhi()
{
	for (int i = 0; i < size[0] + 1; i++)
	{
		for (int j = 0; j < size[1] + 1; j++)
		{
			Vector2d new_pos = Vector2d((i - 0.5)*cellsize[0] + start[0], (j - 0.5)*cellsize[1] + start[1]);
			int idx = j*(size[0] + 1) + i;
			for (int k = 0; k < polygon.size(); k++)
			{
				if (polygon[k]->type == Boundary)
				{
					double d = -polygon[k]->Distance(new_pos).distance;
					nodal_solid_phi[idx] = d;
					//if (d < 0) nodes[idx].type = SOLID;
					//cout << d << endl;
				}
			}
		}
	}

	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			Vector2d new_pos = Vector2d(i*cellsize[0] + start[0], j *cellsize[1] + start[1]);
			int idx = j*size[0] + i;
			for (int k = 0; k < polygon.size(); k++)
			{
				if (polygon[k]->type == Boundary)
				{
					double d = -polygon[k]->Distance(new_pos).distance;
					if (d < -0.000002) nodes[idx].type = SOLID;
					//cout << d << endl;
				}
			}
		}
	}

}



void MACGrid::GridCheck()
{
	//active_list.clear();
	active_list.swap(vector<MACGridNode*>());
	active_length = 0;
	for (int i = 0; i < nodes_length; i++)
	{
		int idx_x = nodes[i].index_x;
		int idx_y = nodes[i].index_y;
		int i_off, j_off;
		bool key = false;
		
		i_off = idx_x;
		j_off = idx_y;
		int idx_t = i_off + j_off*size[0];
		if (nodes[idx_t].liquid_phi < 0) key = true;
		

		if (key == true)
		{
			active_list.push_back(&nodes[i]);
			nodes[i].s_index = active_length;
			active_length++;
		}
	}
}


void MACGrid::ComputerNeighbour()
{
	for (int i = 0; i < nodes_length; i++)
	{
		//nodes[i].particles.clear();
		nodes[i].particles.swap(vector<MPMParticle*>());
	}
	//判断粒子在哪个格子中
	for (int i = 0; i < obj->particles.size(); i++)
	{
		int idx_x = (int)((obj->particles[i]->position[0] - start[0]) / cellsize[0] + 0.5);
		int idx_y = (int)((obj->particles[i]->position[1] - start[1]) / cellsize[1] + 0.5);
		int idx = idx_y*size[0] + idx_x;
		//cout << idx_x << "  " << idx_y << endl;
		nodes[idx].particles.push_back(obj->particles[i]);
	}
}

void MACGrid::UpdateParticle()
{
	for (int i = 0; i < obj->particles.size(); i++)
	{
		obj->particles[i]->position += obj->particles[i]->velocity*delta_t; 
	}
	for (int i = 0; i < obj->particles.size(); i++)
	{
		for (int k = 0; k < polygon.size(); k++)
		{
			if (polygon[k]->type == Boundary)
			{
				double d = polygon[k]->Distance(obj->particles[i]->position).distance;
				if (d > 0.05)
				{
					Vector2d normal = -polygon[i]->Gradient(obj->particles[i]->position[0], obj->particles[i]->position[1]);
					obj->particles[i]->position = obj->particles[i]->position + 1.02*normal*d;
				}
			}
		}
	}
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			int idx = j*size[0] + i;
			nodes[idx].pressure = 0;
		}
	}
	for (int i = 0; i < (size[0] + 1) * size[1]; i++)	velocity_u[i] = 0;
	for (int i = 0; i < size[0] * (size[1] + 1); i++)	velocity_v[i] = 0;
}

void MACGrid::GetNeighbour(int i, int j, int wl, int wh, int hl, int hh, vector<MPMParticle*> &neighbour)
{
	for (int si = i + wl; si <= i + wh; si++) for (int sj = j + hl; sj <= j + hh; sj++)
	{
		if (si < 0 || si > size[0] - 1 || sj < 0 || sj > size[1] - 1) continue;
		int idx = sj*size[0] + si;
		neighbour.insert(neighbour.end(), nodes[idx].particles.begin(), nodes[idx].particles.end());
	}
}

void MACGrid::ParticleToGrid()
{
	double r = cellsize[0] / sqrt(2);
	for (int j = 0; j < size[1]; ++j) for (int i = 0; i < size[0] + 1; ++i)
	{
		Vector2d pos = Vector2d((i - 0.5)*cellsize[0], j*cellsize[0]) + start;//点落在竖直边线上，计算出横向速度
		vector<MPMParticle *> neighbors;
		//GetNeighbour(i, j, -2, 1, -2, 2, neighbors);//以速度承载点为中心
		GetNeighbour(i, j, -1, 0, -1, 1, neighbors);//以速度承载点为中心
		double sumw = 0.0;
		double sumu = 0.0;
		for (MPMParticle* p : neighbors)
		{
			//double wx = bspline((p->position[0] - pos[0]) / cellsize[0]);
			//double wy = bspline((p->position[1] - pos[1]) / cellsize[0]);
			//double w = wx*wy;
			double w = 4.0 / 3.0 * M_PI * 1.0 * r * r * r * linearkernel(p->position - pos, cellsize[0]);
			//sumu += w * p->velocity[0];
			sumu += w * (p->velocity[0] + p->B.col(0).dot(pos - p->position));
			sumw += w;
			//cout <<"u "<< p->position[0] << "  " << p->position[1] << " x " << pos[0] << " y " << pos[1] << " w " << sumw << endl;
		}
		int idx = j*(size[0] + 1) + i;
		velocity_u[idx] = (sumw > 1e-12) ? sumu / sumw : 0.0;//这个处理不用BSPLINE_BOUNDRY，很容易就避免了我遇到的问题
		/*if (velocity_u[idx] != 0)
		{
			cout << idx << " w " << sumw << " u " << velocity_v[idx] << endl;
		}*/
		neighbors.swap(vector<MPMParticle*>());
	}

	for (int j = 0; j < size[1] + 1; ++j) for (int i = 0; i < size[0]; ++i)
	{
		Vector2d pos = Vector2d(i*cellsize[0], (j - 0.5)*cellsize[0]) + start;
		std::vector<MPMParticle *> neighbors;

		//GetNeighbour(i, j, -2, 2, -2, 1, neighbors);
		GetNeighbour(i, j, -1, 1, -1, 0, neighbors);
		double sumw = 0.0;
		double sumu = 0.0;
		for (MPMParticle* p : neighbors)
		{
			//double wx = bspline((p->position[0] - pos[0]) / cellsize[0]);
			//double wy = bspline((p->position[1] - pos[1]) / cellsize[0]);
			//double w = wx*wy;
			double w = 4.0 / 3.0 * M_PI * 1.0 * r * r * r  * linearkernel(p->position - pos, cellsize[0]);
			//sumu += w * p->velocity[1];
			sumu += w * (p->velocity[1] + p->B.col(1).dot(pos - p->position));
			sumw += w;
			//cout << "v " << p->position[0]<<"  "<< p->position[1] << " x " << pos[0] << " y " << pos[1] << " w " << sumw << endl;
		}
		int idx = j*size[0] + i;
		velocity_v[idx] = (sumw > 1e-12) ? sumu / sumw : 0.0;
		//if (velocity_v[idx] < 0)
		//{
		//	cout << idx<<" w "<< sumw<<" v " << velocity_v[idx] << endl;
		//}
		neighbors.swap(vector<MPMParticle*>());
	}
	//cout  << endl;
}

void MACGrid::ComputerPhi()
{
	for (int i = 0; i < nodes_length; i++)
	{
		nodes[i].liquid_phi = 3 * cellsize[0];
	}
	for (int i = 0; i < obj->particles.size(); i++)
	{
		int idx_x = (obj->particles[i]->position[0] - start[0]) / cellsize[0] + 0.5;
		int idx_y = (obj->particles[i]->position[1] - start[1]) / cellsize[0] + 0.5;
		for (int j_off = idx_y - 2; j_off <= idx_y + 3; ++j_off) for (int i_off = idx_x - 2; i_off <= idx_x + 3; ++i_off)
		{
			if (i_off < 0 || i_off >= size[0] || j_off < 0 || j_off >= size[1])
				continue;
			Vector2d pos = Vector2d(i_off*cellsize[0], j_off*cellsize[1]) + start;
			double phi_temp = (pos - obj->particles[i]->position).norm() - 1.0000002 * cellsize[0] / 2;
			int idx = j_off*size[0] + i_off;
			nodes[idx].liquid_phi = min(nodes[idx].liquid_phi, phi_temp);
		}
	}
}

void MACGrid::ComputerWeight()
{
	//u_weights = new double[(size[0] + 1) * size[1]];
	//v_weights = new double[size[0] * (size[1] + 1)];
	for (int j = 0; j < size[1]; ++j) for (int i = 0; i < size[0] + 1; ++i) //水平方向速度，如果两个点都在外，weight为0，如果两个点都在内部，为1
	{
		int idx = j*(size[0] + 1) + i;
		int node_left = (j + 1)*(size[0] + 1) + i;
		int node_right = j*(size[0] + 1) + i;
		u_weights[idx] = 1 - fraction_inside(nodal_solid_phi[node_left], nodal_solid_phi[node_right]);
		u_weights[idx] = clamp(u_weights[idx], 0.0, 1.0);
		//if (idx == 3100) cout << u_weights[idx] << endl;
	}
	for (int j = 0; j < (size[1] + 1); ++j) for (int i = 0; i < size[0]; ++i)
	{
		int idx = j*size[0] + i;
		int node_left = j*(size[0] + 1) + i + 1;
		int node_right = j*(size[0] + 1) + i;
		v_weights[idx] = 1 - fraction_inside(nodal_solid_phi[node_left], nodal_solid_phi[node_right]);
		v_weights[idx] = clamp(v_weights[idx], 0.0, 1.0);
		//if (idx == 3070) cout << v_weights[idx] << endl;
	}
}


void MACGrid::SolvePressure()
{
	//cout << active_length << endl;
	double dt = delta_t;
	typedef Eigen::Triplet<double> T;
	std::vector<T> tripletList;
	SparseMatrixType M(active_length, active_length);                                                   //矩阵
	VectorXd X(active_length);                                                           //右方系数
	VectorXd Y(active_length);
	double scale = dt / (cellsize[0] * cellsize[0]);
	//double scale;
	int mark, index_u, index_v;
	//cout << scale << endl;
	for (int i = 0; i < active_list.size(); i++)
	{
		double major = 0;
		double right = 0;
		//右
		mark = active_list[i]->index + 1;
		index_u = active_list[i]->index + active_list[i]->index_y + 1;
		right += -velocity_u[index_u] / cellsize[0];
		if (nodes[mark].liquid_phi < 0)
		{
			major += scale;
			tripletList.push_back(T(i, nodes[mark].s_index, -scale));
		}
		else if(nodes[mark].type == SOLID)
		{
			right += velocity_u[index_u] / cellsize[0];
		}
		else
		{
			major += scale;
		}
		//左
		mark = active_list[i]->index - 1;
		index_u = active_list[i]->index + active_list[i]->index_y;
		right += velocity_u[index_u] / cellsize[0];
		if (nodes[mark].liquid_phi < 0)
		{
			major += scale;
			tripletList.push_back(T(i, nodes[mark].s_index, -scale));
		}
		else if(nodes[mark].type == SOLID)
		{
			right += -velocity_u[index_u] / cellsize[0];
		}
		else
		{
			major += scale;
		}
		
		//上
		mark = active_list[i]->index + size[0];
		index_v = active_list[i]->index + size[0];
		right += -velocity_v[index_v] / cellsize[0];
		if (nodes[mark].liquid_phi < 0)
		{
			major += scale;
			tripletList.push_back(T(i, nodes[mark].s_index, -scale));
		}
		else if(nodes[mark].type == SOLID)
		{
			right += velocity_v[index_v] / cellsize[0];
		}
		else
		{
			major += scale;
		}
		
		//下
		mark = active_list[i]->index - size[0];
		index_v = active_list[i]->index;
		right += velocity_v[index_v] / cellsize[0];
		if (nodes[mark].liquid_phi < 0)
		{
			major += scale;
			tripletList.push_back(T(i, nodes[mark].s_index, -scale));
		}
		else if (nodes[mark].type == SOLID)
		{
			right += -velocity_v[index_v] / cellsize[0];
		}
		else
		{
			major += scale;
		}


		tripletList.push_back(T(i, i, major));
		Y(i) = right;
		//cout << "round" << endl;
	}

	M.setFromTriplets(tripletList.begin(), tripletList.end());
	//cout << M << endl << endl;
	SparseLU<SparseMatrixType>MatricesM;
	MatricesM.compute(M);                                                              //预分解
	X = MatricesM.solve(Y); 
	//cout << Y << endl << endl;
	for (int i = 0; i < active_list.size(); i++)
	{
		active_list[i]->pressure = X(i);
	}

	for (int i = 1; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			int idx = j*size[0] + i;
			int idx_u = idx + j;
			if (u_weights[idx_u] > 0)
			//if(nodes[idx-1].type!=SOLID)
			{
				if (nodes[idx].liquid_phi < 0 || nodes[idx - 1].liquid_phi < 0)
				{
					velocity_u[idx_u] -= dt*(nodes[idx].pressure - nodes[idx - 1].pressure) / cellsize[0];
				}
			}
			else
				velocity_u[idx_u] = 0;
		}
	}

	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 1; j < size[1]; j++)
		{
			int idx = j*size[0] + i;
			int idx_v = idx;
			if (v_weights[idx_v] > 0)
			//if (nodes[idx - (int)size[0]].type != SOLID)
			{
				if (nodes[idx].liquid_phi < 0 || nodes[idx - (int)size[0]].liquid_phi < 0)
				{
					velocity_v[idx_v] -= dt*(nodes[idx].pressure - nodes[idx - (int)size[0]].pressure) / cellsize[0];
				}
			}
			else
				velocity_v[idx_v] = 0;
		}
	}

	/*for (int i = 0; i < size[0]; i++)
	{
		for (int j = 1; j < size[1]; j++)
		{
			int idx = j*size[0] + i;
			if (nodes[idx].pressure > 0)
			{
				cout << nodes[idx].pressure << endl;
			}
		}
	}*/
}

void MACGrid::GridToParticle()
{
	for (int i = 0; i < obj->particles.size(); i++) 
	{
		Vector2d pos = (obj->particles[i]->position - start) / cellsize[0] + Vector2d(0.5, 0.5);
		Vector2d p0 = pos - Vector2d(0, 0.5);
		Vector2d p1 = pos - Vector2d(0.5, 0);
		double v_x = lerp(p0, velocity_u, size[0] + 1);
		double v_y = lerp(p1, velocity_v, size[0]);
		obj->particles[i]->velocity[0] = v_x;
		obj->particles[i]->velocity[1] = v_y;
		obj->particles[i]->B = ComputerAffineMatrix(obj->particles[i]->position);
	}
}

void MACGrid::AddObject()
{
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			int idx = j*size[0] + i;
			nodes[idx].type = MACNONE;
			if (i == 0 || i == (size[0] - 1) || j == 0 || j == (size[1] - 1))
			{

				nodes[idx].type = SOLID;
			}
		}
	}
	int idx = 2 * size[0] + 2;
	nodes[idx].type = FLUID;
	nodes[idx].density = 1;
	nodes[idx].mass = 1;

	//idx = 2 * size[0] + 3;
	//nodes[idx].type = FLUID;
	//nodes[idx].density = 1;
	//nodes[idx].mass = 1;

	//idx = 4 * size[0] + 1;
	//nodes[idx].type = FLUID;
	//nodes[idx].density = 1;
	//nodes[idx].mass = 1;

	//idx = 4 * size[0] + 4;
	//nodes[idx].type = FLUID;
	//nodes[idx].density = 1;
	//nodes[idx].mass = 1;

	//idx = 3 * size[0] + 2;
	//nodes[idx].type = FLUID;
	//nodes[idx].density = 1;
	//nodes[idx].mass = 1;

	//idx = 3 * size[0] + 3;
	//nodes[idx].type = FLUID;
	//nodes[idx].density = 1;
	//nodes[idx].mass = 1;

	//idx = 3 * size[0] + 1;
	//nodes[idx].type = FLUID;
	//nodes[idx].density = 1;
	//nodes[idx].mass = 1;

	//idx = 3 * size[0] + 4;
	//nodes[idx].type = FLUID;
	//nodes[idx].density = 1;
	//nodes[idx].mass = 1;
}

Matrix2d MACGrid::ComputerAffineMatrix(Vector2d p)
{
	Vector2d pos = (p - start) / cellsize[0] + Vector2d(0.5, 0.5);
	Vector2d p0 = pos - Vector2d(0, 0.5);
	Vector2d p1 = pos - Vector2d(0.5, 0);
	Matrix2d affine;
	affine.col(0) = affine_lerp(p0, velocity_u, size[0] + 1) / cellsize[0];
	affine.col(1) = affine_lerp(p1, velocity_v, size[0]) / cellsize[0];
	return affine;
}

void MACGrid::Correct()
{
	double re = cellsize[0] / sqrt(2.0);
	for (int i = 0; i < obj->particles.size(); i++)
	{
		//Vector2d pos = Vector2d((i - 0.5)*cellsize[0], j*cellsize[0]) + start;//点落在竖直边线上，计算出横向速度
		MPMParticle* p = obj->particles[i];
		int ix = max(0, min((int)((p->position[0] - start[0]) / cellsize[0]), (int)size[0]));
		int iy = max(0, min((int)((p->position[1] - start[1]) / cellsize[1]), (int)size[1]));
		vector<MPMParticle *> neighbors;
		//GetNeighbour(i, j, -2, 1, -2, 2, neighbors);//以速度承载点为中心
		GetNeighbour(ix, iy, -1, 1, -1, 1, neighbors);//以速度承载点为中心
		Vector2d spring = Vector2d::Zero();
		for (MPMParticle* np : neighbors)
		{
			if (p != np)
			{
				double dist = (p->position - np->position).norm();
				double w = 50.0 * smooth_kernel(dist * dist, re);
				if (dist > 0.01 * re)
				{
					spring += w * (p->position - np->position) / dist * re;
				}
				else {
					spring(0) += 0.01 * re / delta_t * (rand() & 0xFF) / 255.0;
					spring(1) += 0.01 * re / delta_t * (rand() & 0xFF) / 255.0;
					//std::cout << spring(0) << std::endl;
				}
			}
		}
		p->position = p->position + delta_t * spring;
	}
}

void MACGrid::CollsionHandle()
{

}