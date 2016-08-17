#pragma once
#include "warping.h"


class WarpingIDW :
	public Warping
{
public:
	WarpingIDW(void);
	~WarpingIDW(void);

	void CalculateWarping(Mat &image_mat);
	void FillHoles(Mat &image_mat);
};

