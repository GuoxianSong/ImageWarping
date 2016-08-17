#pragma once
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using std::vector;
using namespace cv;

class QPoint;

class Warping
{
public:
	Warping(void);
	virtual ~Warping(void);
	
	double Distance(QPoint p, QPoint q);
	void InitControlPoints(vector<QPoint>& points_start, vector<QPoint>& points_end);		// Get control points
	int IsInControlPoints(int x, int y);
	virtual void CalculateWarping(Mat &image_mat);

protected:
	void SetFixedPoints(Mat &image_mat);

protected:
	vector<double> points_start_x_;
	vector<double> points_start_y_;
	vector<double> points_end_x_;
	vector<double> points_end_y_;
	vector<bool> is_points_change_;
};

