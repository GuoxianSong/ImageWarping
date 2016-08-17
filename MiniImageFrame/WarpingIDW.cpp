#include "WarpingIDW.h"
#include <QImage>
#include <QColor>
#include <iostream>

using namespace std;

WarpingIDW::WarpingIDW(void)
{
}


WarpingIDW::~WarpingIDW(void)
{
}

void WarpingIDW::CalculateWarping(Mat& image_mat)
{
	SetFixedPoints(image_mat);

	Mat image_backup = image_mat.clone();
	int num = points_start_x_.size();
	int width = image_mat.cols;
	int height = image_mat.rows;

	MatIterator_<Vec3b> iter, iterend;
	for (iter=image_mat.begin<Vec3b>(), iterend=image_mat.end<Vec3b>(); iter != iterend; ++iter)
	{
		(*iter)[0] = 193;
		(*iter)[1] = 193;
		(*iter)[2] = 193;
	}
	for (int i=0; i<width; i++)
	{
		for (int j=0; j<height; j++)
		{
			vector<double> distance(num);
			double dist_sum = 0;
			
			int indice = IsInControlPoints(i, j);
			
			if (-1 != indice)
			{
				image_mat.at<Vec3b>( points_end_y_[indice], points_end_x_[indice]) = image_backup.at<Vec3b>(j, i);
			}
			else
			{
				for (int k=0; k<num; k++)
				{
					QPoint p_origin(i, j);
					QPoint p_result(points_start_x_[k], points_start_y_[k]);
					double dtmp = 1.0/Distance(p_origin, p_result);
					double dist_single = pow(dtmp, 2);
	
					distance[k] = dist_single;
					dist_sum += dist_single;
				}

				double dx = 0;
				double dy = 0;
				for (int k=0; k<num; k++)
				{
					dx += distance[k]/dist_sum*(points_end_x_[k]-points_start_x_[k]);
					dy += distance[k]/dist_sum*(points_end_y_[k]-points_start_y_[k]);
				}

				int pos_x = int(dx)+i;
				int pos_y = int(dy)+j;

				if ( pos_x>width-1 || pos_y>height-1 || pos_x<0 || pos_y<0)
				{
					continue;
				}
				image_mat.at<Vec3b>(pos_y, pos_x) = image_backup.at<Vec3b>(j, i);
			}		
		}
	}
}

void WarpingIDW::FillHoles(Mat &image_mat)
{

}
