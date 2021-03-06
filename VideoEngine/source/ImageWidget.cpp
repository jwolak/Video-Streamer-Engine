#include "headers/ImageWidget.h"

#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{
}

void ImageWidget::setImage(QImage image)
{
	image_ = image;
	update();
}

QImage ImageWidget::image() const
{
	return image_;
}

void ImageWidget::paintEvent(QPaintEvent *)
{
	int iw = image_.width();
	int ih = image_.height();
	if (iw < 1 || ih < 1) return;

	int w = width();
	int h = height();
	if (w < 1 || h < 1) return;

	int x = (w - iw) / 2;
	int y = (h - ih) / 2;

	QPainter pr(this);
	pr.drawImage(x, y, image_);
}
