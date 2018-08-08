#include"../../include/scene/Scene.h"

Scene::Scene()
{
	Flame_Frames = 0;
	camera.look_at = Vector2d(VIEW_WIDTH / 2.0, VIEW_HEIGHT / 2.0 - 0.1);
	camera.window_cell_x = VIEW_WIDTH / (double)WINDOW_WIDTH;
	camera.window_cell_y = VIEW_HEIGHT / (double)WINDOW_HEIGHT;
	camera.window_center[0] = (int)VIEW_WIDTH / (2.0*camera.window_cell_x);
	camera.window_center[1] = (int)VIEW_HEIGHT/ (2.0*camera.window_cell_y);
	camera.view_distance = 1.0;
}

Scene::~Scene()
{

}

void Scene::AddObject(SDF* polygon, SDFType Type, Vector2d velocity, ParticleType PType, Vector3d ColorFill)
{
	polygon->type = Type;
	SDFInfo info;
	info.sdf = polygon;
	info.stype = Type;
	info.ptype =PType;
	info.velocity = velocity;
	info.color = ColorFill;
	SDFlist.push_back(info);
}

void Scene::SceneUpdate()
{
	SDF temp;
	for (int i = 0; i < SDFlist.size(); i++)
	{
		if (SDFlist[i].sdf->move == true)
		{
			//cout << frames << endl;
			SDFlist[i].sdf->Control(Flame_Frames);
			SDFlist[i].sdf->Update(DT);
			SDFlist[i].velocity = temp.Rotate(SDFlist[i].velocity, SDFlist[i].sdf->omega*DT);
			if (SDFlist[i].sdf->sample_check == true)
			{
				SDFlist[i].sdf->sample_time = 0.1 / (SDFlist[i].velocity.norm()*DT);
				SDFlist[i].sdf->sample_check = false;
				//cout << "Sample Time:" << scene->SDFlist[i].sdf->sample_time << endl;
			}
		}
	}
}

SDF* Scene::FindSDF(string name)
{
	for (int i = 0; i < SDFlist.size(); i++)
		if (name == SDFlist[i].sdf->sdfName) return SDFlist[i].sdf;
}