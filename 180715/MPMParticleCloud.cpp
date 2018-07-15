#include"MPMParticleCloud.h"

MPMParticleCloud::MPMParticleCloud()
{
	size = 0;
	p_num = 0;
}
MPMParticleCloud::MPMParticleCloud(PointList list, ParticleType type, Vector2d velocity, Vector3d PColor)
{
	size = list.size();
	p_num = 0;
	//物质的参数读入
	SandParameters sand;
	SnowParameters snow;
	WaterParameters water;
	ElasticParameters elastic;
	if (type == WETSAND) SandParametersIn(sand);
	if (type == DRYSAND) SandParametersIn(sand);
	if (type == SNOW) SnowParametersIn(snow);
	if (type == WATER) WaterParametersIn(water);
	if (type == ELASTIC) ElasticParametersIn(elastic);

	for (int i = 0; i < list.size(); i++)
	{
		MPMParticle* p;
		if (type == DRYSAND) p = new MPMParticleSand(list[i], velocity, p_num, sand, DRY, PColor);
		if (type == WETSAND) p = new MPMParticleSand(list[i], velocity, p_num, sand, WET, PColor);
		if (type == ELASTIC) p = new MPMParticleElastic(list[i], velocity, p_num, elastic, PColor);
		if (type == WATER) p = new MPMParticleWater(list[i], velocity, p_num, water, PColor);
		if (type == SNOW) p = new MPMParticleSnow(list[i], velocity, p_num, snow, PColor);
		particles.push_back(p);
		p_num++;
	}
}

MPMParticleCloud::~MPMParticleCloud()
{
}

void MPMParticleCloud::merge(MPMParticleCloud& other)
{
	size += other.size;
	for (int i = 0; i < other.particles.size(); i++) {
		other.particles[i]->index += p_num;
	}
	p_num += other.size;
	particles.reserve(size);
	particles.insert(particles.end(), other.particles.begin(), other.particles.end());
}
void MPMParticleCloud::update()
{
	for (int i = 0; i<size; i++) {
		MPMParticle* p;
		p = particles[i];
		p->Update();
	}
}



void MPMParticleCloud::SandParametersIn(SandParameters &p)
{
	ifstream infile;
	infile.open("SandConfig.txt");
	if (!infile.is_open())
	{
		cout << "Could Not Open File!!!";
		exit(EXIT_FAILURE);
	}
	double val;
	string st;
	infile >> st;
	while (infile.good())
	{
		if (st.compare("lambda") == 0) {
			infile >> val;
			p.Lambda = val;
		}
		if (st.compare("mu") == 0) {
			infile >> val;
			p.Mu = val;
		}
		if (st.compare("h0") == 0) {
			infile >> val;
			p.H0 = val;
		}
		if (st.compare("h1") == 0) {
			infile >> val;
			p.H1 = val;
		}
		if (st.compare("h2") == 0) {
			infile >> val;
			p.H2 = val;
		}
		if (st.compare("h3") == 0) {
			infile >> val;
			p.H3 = val;
		}
		if (st.compare("mass") == 0) {
			infile >> val;
			p.mass = val;
		}
		infile >> st;
	}
}

void MPMParticleCloud::SnowParametersIn(SnowParameters &p)
{
	ifstream infile;
	infile.open("SnowConfig.txt");
	if (!infile.is_open())
	{
		cout << "Could Not Open File!!!";
		exit(EXIT_FAILURE);
	}
	double val;
	string st;
	infile >> st;
	while (infile.good())
	{
		if (st.compare("lambda") == 0) {
			infile >> val;
			p.Lambda = val;
		}
		if (st.compare("mu") == 0) {
			infile >> val;
			p.Mu = val;
		}
		if (st.compare("CRIT_COMPRESS") == 0) {
			infile >> val;
			p.CRIT_COMPRESS = val;
		}
		if (st.compare("CRIT_STRETCH") == 0) {
			infile >> val;
			p.CRIT_STRETCH = val;
		}
		if (st.compare("HARDENING") == 0) {
			infile >> val;
			p.HARDENING = val;
		}
		if (st.compare("mass") == 0) {
			infile >> val;
			p.mass = val;
		}
		infile >> st;
	}
}

void MPMParticleCloud::WaterParametersIn(WaterParameters &p)
{

	ifstream infile;
	infile.open("WaterConfig.txt");
	if (!infile.is_open())
	{
		cout << "Could Not Open File!!!";
		exit(EXIT_FAILURE);
	}
	double val;
	string st;
	infile >> st;
	while (infile.good())
	{
		if (st.compare("Gama") == 0) {
			infile >> val;
			p.Gama = val;
		}
		if (st.compare("Hardness") == 0) {
			infile >> val;
			p.Hardness = val;
		}
		if (st.compare("mass") == 0) {
			infile >> val;
			p.mass = val;
		}
		infile >> st;
	}
}

void MPMParticleCloud::ElasticParametersIn(ElasticParameters &p)
{
	ifstream infile;
	infile.open("ElasticConfig.txt");
	if (!infile.is_open())
	{
		cout << "Could Not Open File!!!";
		exit(EXIT_FAILURE);
	}
	double val;
	string st;
	infile >> st;
	while (infile.good())
	{
		if (st.compare("lambda") == 0) {
			infile >> val;
			p.Lambda = val;
		}
		if (st.compare("mu") == 0) {
			infile >> val;
			p.Mu = val;
		}
		if (st.compare("mass") == 0) {
			infile >> val;
			p.mass = val;
		}
		infile >> st;
	}
}
