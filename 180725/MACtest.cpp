#include"MACGrid.h"
#include<opencv2/opencv.hpp>

cv::Mat record= cv::Mat::zeros(360, 360, CV_8UC3);

void Draw(MACGrid grid)
{
	for (int i = 0; i < grid.size[0]; i++)
	{
		for (int j = 0; j < grid.size[1]; j++)
		{
			record.at<cv::Vec3b>(j, i)(0) = 255;
			record.at<cv::Vec3b>(j, i)(1) = 255;
			record.at<cv::Vec3b>(j, i)(2) = 255;
		}
	}
	int ji = 0;
	for (int i = 0; i < grid.size[0]; i++)
	{
		for (int j = 0; j < grid.size[1]; j++)
		{
			int idx= j* grid.size[0] + i;
			int sx = 300 * (grid.nodes[idx].position[0] - 0.5*grid.cellsize[0]);
			int sy = 300 * (grid.nodes[idx].position[1] - 0.5*grid.cellsize[1]);
			if (grid.nodes[idx].type == SOLID)
			{
				cv::rectangle(record, cv::Point(sx, sy), cv::Point(sx + 60, sy + 60), cv::Scalar(255, 0, 0), -1);
			}
			else if(grid.nodes[idx].type == FLUID)
			{
				cv::rectangle(record, cv::Point(sx, sy), cv::Point(sx + 60, sy + 60), cv::Scalar(0, 255, 0), -1);
				ji++;
			}
			else
			{
				cv::rectangle(record, cv::Point(sx, sy), cv::Point(sx + 60, sy + 60), cv::Scalar(0, 255, 255), -1);
			}
		}
	}

	for (int i = 0; i < grid.size[0]; i++)
	{
		for (int j = 0; j < grid.size[1]; j++)
		{
			int idx = j* grid.size[0] + i;
			int lsx = 300 * (grid.nodes[idx].position[0] - 0.5*grid.cellsize[0]);
			int lsy = 300 * grid.nodes[idx].position[1];
			int lex = lsx + grid.velocity_u[idx + j] * 200;
			cv::line(record, cv::Point(lsx, lsy), cv::Point(lex, lsy), cv::Scalar(255, 0, 255), 1);
			int dsx = 300 * grid.nodes[idx].position[0];
			int dsy = 300 * (grid.nodes[idx].position[1] - 0.5*grid.cellsize[1]);
			int dey = dsy + grid.velocity_v[idx] * 200;
			cv::line(record, cv::Point(dsx, dsy), cv::Point(dsx, dey), cv::Scalar(255, 0, 255), 1);
		}
	}

	//cout << "zong" << ji << endl;
}


int main()
{
	MACGrid* grid;
	grid=new MACGrid(Vector2d(0, 0), Vector2d(1, 1), Vector2d(5, 5));
	grid->Initial();
	grid->AddObject();
	//while (1)
	//{
	//	grid->AddForce();
	//	grid->GridCheck();
	//	grid->SolvePressure();
	//	grid->Update();
	//	grid->CalculateCenter();
	//	grid->Transport();
	//	//grid->GridCheck();
	//	Draw(*grid);
	//	cv::imshow("1", record);
	//	cv::waitKey(40);
	//}
	

	grid->AddForce();
	grid->GridCheck();
	grid->SolvePressure();
	grid->Update();
	grid->CalculateCenter();
	grid->Transport();

	grid->AddForce();
	grid->GridCheck();
	grid->SolvePressure();
	grid->Update();
	grid->CalculateCenter();
	grid->Transport();
	Draw(*grid);
	cv::imshow("1", record);
	cv::waitKey(0);
}

