#include "ImageWidget.h"
#include <QImage>
#include <QPainter>
#include <QtWidgets> 
#include <iostream>
#include "WarpingIDW.h"
#include "WarpingRBF.h"

using std::cout;
using std::endl;
using namespace cv;

ImageWidget::ImageWidget(void)
{
	is_select_points_mode_ = false;
	is_draw_lines_ = false;
	warp_class_ = None;
}


ImageWidget::~ImageWidget(void)
{
}

void ImageWidget::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter;
	painter.begin(this);

	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image
	//QRect rect = QRect( (width()-image_->width())/2, (height()-image_->height())/2, image_->width(), image_->height());
	QImage image_show = QImage( (unsigned char *)(image_mat_.data), image_mat_.cols, image_mat_.rows, image_mat_.step, QImage::Format_RGB888 );
	QRect rect = QRect(0, 0, image_show.width(), image_show.height());
	painter.drawImage(rect, image_show); 

	// Draw control line
	QPen pen(Qt::red, 2);
	painter.setPen(pen);
	painter.drawLine(point_origin_, point_end_);
	if (points_start_.size() != 0)
	{
		for (size_t i=0; i<points_start_.size(); i++)
		{
			painter.drawLine(points_start_[i], points_end_[i]);
		}
	}
	
	painter.end();
}

void ImageWidget::mousePressEvent(QMouseEvent *mouseevent)
{
	if (is_select_points_mode_ && (mouseevent->button() == Qt::LeftButton))
	{
		is_draw_lines_ = true;
		point_origin_ = point_end_ = mouseevent->pos();
	}
}

void ImageWidget::mouseMoveEvent(QMouseEvent *mouseevent)
{
	if (is_draw_lines_)
	{
		point_end_ = mouseevent->pos();
	}
	update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *mouseevent)
{
	if (is_draw_lines_)
	{
		is_draw_lines_ = false;
		point_end_ = mouseevent->pos();
		points_start_.push_back(point_origin_);
		points_end_.push_back(point_end_);
	}
}

void ImageWidget::Open()
{
	// Open file
	QString fileName = QFileDialog::getOpenFileName(this, tr("Read Image"), ".", tr("Images(*.bmp *.png *.jpg)"));

	
	// Load file
	if (!fileName.isEmpty())
	{
		image_mat_ = cv::imread( fileName.toLatin1().data() );
		cvtColor( image_mat_, image_mat_, CV_BGR2RGB );	
		image_mat_backup_ = image_mat_.clone();
	}

	update();
}

void ImageWidget::Save()
{
	SaveAs();
}

void ImageWidget::SaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp)"));
	if (filename.isNull())
	{
		return;
	}	

	cv::Mat image_save;
	cvtColor(image_mat_, image_save, CV_RGB2BGR);
	imwrite(filename.toLatin1().data(), image_save);
}

void ImageWidget::Invert()
{
	MatIterator_<Vec3b> iter, iterend;
	for (iter=image_mat_.begin<Vec3b>(), iterend=image_mat_.end<Vec3b>(); iter != iterend; ++iter)
	{
		(*iter)[0] = 255-(*iter)[0];
		(*iter)[1] = 255-(*iter)[1];
		(*iter)[2] = 255-(*iter)[2];
	}

	// equivalent member function of class QImage
	// image_->invertPixels(QImage::InvertRgb);
	update();
}

void ImageWidget::Mirror(bool ishorizontal, bool isvertical)
{
	int width = image_mat_.cols;
	int height = image_mat_.rows;

	if (ishorizontal)
	{
		if (isvertical)
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					image_mat_.at<Vec3b>(j, i) = image_mat_backup_.at<Vec3b>(height-1-j, width-1-i);
				}
			}
		} 
		else
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					image_mat_.at<Vec3b>(j, i) = image_mat_backup_.at<Vec3b>(j, width-1-i);
				}
			}
		}
		
	}
	else
	{
		if (isvertical)
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					image_mat_.at<Vec3b>(j, i) = image_mat_backup_.at<Vec3b>(height-1-j, i);
				}
			}
		}
	}

	update();
}

void ImageWidget::TurnGray()
{
	MatIterator_<Vec3b> iter, iterend;
	for (iter=image_mat_.begin<Vec3b>(), iterend=image_mat_.end<Vec3b>(); iter != iterend; ++iter)
	{
		int itmp = ((*iter)[0]+(*iter)[1]+(*iter)[2])/3;
		(*iter)[0] = itmp;
		(*iter)[1] = itmp;
		(*iter)[2] = itmp;
	}

	update();
}

void ImageWidget::Restore()
{
	image_mat_ = image_mat_backup_.clone();
	points_start_.clear();
	points_end_.clear();
	point_origin_ = point_end_ = QPoint(0, 0);
	update();
}

void ImageWidget::SelectControlPointsMode()
{
	is_select_points_mode_ = true;
}

void ImageWidget::WarpIDW()
{
	warp_class_ = IDW;
	DoWarp();
}

void ImageWidget::WarpRBF()
{
	warp_class_ =  RBF;
	DoWarp();
}

void ImageWidget::DoWarp()
{
	Warping *pwarp;
	switch (warp_class_)
	{
	case IDW: 
		pwarp = new WarpingIDW();
		break;
		
	case RBF:
		pwarp = new WarpingRBF();
		break;

	default:
		break;
	}

	pwarp->InitControlPoints(points_start_, points_end_);
	pwarp->CalculateWarping(image_mat_);
	update();
}