#include "arcWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QMouseEvent>
#include <qmath.h>

ArcWidget::ArcWidget(QWidget *parent)
    : QWidget{parent}
{
	ArcInfo* arcInfo1 = new ArcInfo(0, 60, qRgb(255, 255, 255), qRgb(181, 3, 3), QStringLiteral("高"));
	ArcInfo* arcInfo2 = new ArcInfo(60, 60, qRgb(255, 255, 255), qRgb(248, 168, 10), QStringLiteral("中"));
	ArcInfo* arcInfo3 = new ArcInfo(120, 60, qRgb(255, 255, 255), qRgb(14, 203, 5), QStringLiteral("低"));
	arcInfoList.append(arcInfo1);
	arcInfoList.append(arcInfo2);
	arcInfoList.append(arcInfo3);
	this->setFixedSize(800,800); 
	pointerStyle = PointerStyle_Indicator;
}

void ArcWidget::setArcInfoData(QVector<ArcInfo*> arcInfos)
{
    arcInfoList = arcInfos;
}

void ArcWidget::setPointerAngle(double pointerAngle)
{
	this->pointerAngle = pointerAngle;
}

void ArcWidget::setBlowUpIndex(int index)
{
	if (index < arcInfoList.size())
	{
		magnifyArcIndex = index - 1;
	}
}

void ArcWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	int side = qMin(this->width(), this->height());//圆跟随较窄屏幕缩放
	//绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.translate(side / 2, side / 2);
	painter.scale(side / 200.0, side / 200.0);//这个保证圆的大小随窗口放大缩小而移动

	if (pointerStyle == PointerStyle_Circle) {
		drawPointerCircle(&painter);
	}
	else if (pointerStyle == PointerStyle_Indicator) {
		drawPointerIndicator(&painter);
	}
	else if (pointerStyle == PointerStyle_IndicatorR) {
		drawPointerIndicatorR(&painter);
	}
	else if (pointerStyle == PointerStyle_Triangle) {
		drawPointerTriangle(&painter);
	}
	for (int i = 0; i < arcInfoList.size(); ++i)
	{
		if (magnifyArcIndex == i)
		{
			gradientArc(&painter, arcInfoList[i]->startAngle, arcInfoList[i]->angleLength, arcInfoList[i]->color, true);
			gradientText(&painter, arcInfoList[i]->startAngle, arcInfoList[i]->angleLength, arcInfoList[i]->textcolor, arcInfoList[i]->text);
		}
		else
		{
			gradientArc(&painter, arcInfoList[i]->startAngle, arcInfoList[i]->angleLength, arcInfoList[i]->color);
			gradientText(&painter, arcInfoList[i]->startAngle, arcInfoList[i]->angleLength, arcInfoList[i]->textcolor, arcInfoList[i]->text);
		}
		painter.translate(side / 2, side / 2);
		painter.scale(side / 200.0, side / 200.0);//这个保证圆的大小随窗口放大缩小而移动
	}
	gradientLine(&painter, _radius, 1, 50, qRgb(117, 117, 117));
	painter.translate(side / 2, side / 2);
	painter.scale(side / 200.0, side / 200.0);//这个保证圆的大小随窗口放大缩小而移动
	gradientLine(&painter, _radius - 20, 1, 50, qRgb(117, 117, 117));
	QWidget::paintEvent(event);
}


void ArcWidget::gradientArc(QPainter* painter, int startAngle, int angleLength, QRgb color, bool magnifyArc)
{
	int radius = _radius;
	int arcHeight = _arcHeight;
	if (magnifyArc)
	{
		radius += 2;
		arcHeight += 4;
	}
	painter->setBrush(QBrush(color));

	// << 1（左移1位）相当于radius*2 即：150*2=300
	//QRectF(-150, -150, 300, 300)
	QRectF rect(-radius, -radius, radius << 1, radius << 1);
	QPainterPath path;
	path.arcTo(rect, startAngle, angleLength);

	// QRectF(-120, -120, 240, 240)
	QPainterPath subPath;
	subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));

	// path为扇形 subPath为椭圆
	path -= subPath;

	painter->setPen(Qt::NoPen);
	painter->drawPath(path);
}

void ArcWidget::gradientText(QPainter* painter, int startAngle, int angleLength, QRgb textcolor, QString text)
{
	int radius = _radius - _arcHeight + 4;
	QFont font;
	font.setFamily("Microsoft YaHei");
	font.setPointSize(6);
	painter->setPen(QPen(textcolor));
	painter->setFont(font);
	double sina = qSin((startAngle + angleLength / 2 + 5) * M_PI / 180);
	double cosa = qCos((startAngle + angleLength / 2 + 5) * M_PI / 180);

	int x = radius * cosa;
	int y = -radius * sina;
	double textangle = 90 - (startAngle + angleLength / 2);
	painter->translate(x, y);
	painter->rotate(textangle);
	painter->drawText(0, 0, text);
	painter->resetMatrix();
}

void ArcWidget::gradientLine(QPainter* painter, int start, int length, int steps, QRgb color)
{
	int radius = start;
	painter->rotate(90);
	double angleStep = 180.0 / steps;
	QPen pen;
	pen.setColor(QColor(color));
	pen.setCapStyle(Qt::RoundCap);
	painter->setPen(pen);
	for (int i = 0; i <= steps; i++) {
		painter->drawLine(0, radius - length, 0, radius);
		painter->rotate(angleStep);
	}

	painter->restore();
	painter->resetMatrix();
}

void ArcWidget::drawPointerCircle(QPainter* painter)
{
	int radius = 6;
	int offset = 30;
	painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(145, 143, 143));

	painter->rotate(90);
	double degRotate = 180 / 180 * pointerAngle;
	painter->rotate(degRotate);
	painter->drawEllipse(-radius, radius + offset, radius * 2, radius * 2);

	painter->restore();
}

void ArcWidget::drawPointerIndicator(QPainter* painter)
{
	int radius = 60;
	painter->save();
	painter->setOpacity(0.8);
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(145,143,143));

	QPolygon pts;
	pts.setPoints(4, -5, 0, 0, -5, 5, 0, 0, radius);

	painter->rotate(90);
	double degRotate = 180 / 180 * pointerAngle;
	painter->rotate(degRotate);
	painter->drawConvexPolygon(pts);

	painter->restore();
}

void ArcWidget::drawPointerIndicatorR(QPainter* painter)
{
	int radius = _radius * 0.375;
	painter->save();
	painter->setOpacity(1.0);

	QPen pen;
	pen.setWidth(1);
	pen.setColor(QColor(145, 143, 143));
	painter->setPen(pen);
	painter->setBrush(QColor(145, 143, 143));

	QPolygon pts;
	pts.setPoints(3, -5, 0, 5, 0, 0, radius - 125);

	painter->rotate(90);
	double degRotate = 180 / 180 * pointerAngle;
	painter->rotate(degRotate);
	painter->drawConvexPolygon(pts);

	//增加绘制圆角直线,与之前三角形重叠,形成圆角指针
	pen.setCapStyle(Qt::RoundCap);
	pen.setWidthF(4);
	painter->setPen(pen);
	painter->drawLine(0, 0, 0, radius - 125);

	painter->restore();
}

void ArcWidget::drawPointerTriangle(QPainter* painter)
{
	int radius = 10;
	int offset = 38;
	painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(145, 143, 143));

	QPolygon pts;
	pts.setPoints(3, -5, 0 + offset, 5, 0 + offset, 0, radius + offset);

	painter->rotate(90);
	double degRotate = 180 / 180 * pointerAngle;
	painter->rotate(degRotate);
	painter->drawConvexPolygon(pts);

	painter->restore();
}

void ArcWidget::mousePressEvent(QMouseEvent* event)
{
	int side = qMin(this->width(), this->height());//圆跟随较窄屏幕缩放

	float radius = _radius * side / 200.0;
	float arcHeight = _arcHeight * side / 200.0;

	QPoint clickPos = event->pos() - QPoint(side / 2, side / 2);
	double distance = std::sqrt(std::pow(clickPos.x(), 2) + std::pow(clickPos.y(), 2));
	if (distance < radius && distance > (radius - arcHeight))
	{
		double angle = qAtan2(clickPos.y(), clickPos.x()) * 180.0 / M_PI;
		if (angle < 0)
		{
			angle = qAbs(angle);
		}
		else
		{
			angle = 360 - angle;
		}
		emit sig_sendCilckedAngle(angle);

		switch (magnifyArcIndex)
		{
		case 0:
			if (angle >= 0 && angle <= 60)
			{
				sig_clickedArcIndex();
				qDebug()<<"1";
			}
			break;
		case 1:
			if (angle >= 60 && angle <= 120)
			{
				sig_clickedArcIndex();
				qDebug() << "2";
			}
			break;
		case 2:
			if (angle >= 120 && angle <= 180)
			{
				sig_clickedArcIndex();
				qDebug() << "3";
			}
			break;
		default:
			break;
		}
	}
	QWidget::mousePressEvent(event);
}

void ArcWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QWidget::mouseReleaseEvent(event);
}
