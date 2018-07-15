#pragma once

#include"Render2D.h"

class ParallelRender
{

	int* const my_a;
public:
	Render2D* m_render;
	void operator()(const tbb::blocked_range<size_t>&r)const
	{
		int *a = my_a;
		for (size_t i = r.begin(); i != r.end(); i++)
		{
			m_render->PixelCalculate(a[i]);
		}
	}
	ParallelRender(int a[], Render2D* render) :my_a(a) { m_render = render; }
};