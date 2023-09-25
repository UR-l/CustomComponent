#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

struct ArcInfo
{
    int startAngle;     //��ʼ�Ƕ�
    int angleLength;    //Բ���Ƕ�
	QRgb textcolor;     //������ɫ
	QRgb color;         //Բ����ɫ
    QString text;
    ArcInfo() {
        startAngle = 0;
        angleLength = 0;
        textcolor = qRgb(0, 0, 0);
        color = qRgb(0, 0, 0);
    }
    ArcInfo(int startAngle, int angleLength, QRgb textcolor, QRgb color, QString text) {      
        this->startAngle = startAngle;
        this->angleLength = angleLength;
		this->textcolor = textcolor;
        this->color = color;
        this->text = text;
    }
};

class ArcWidget : public QWidget
{
    Q_OBJECT
public:
	enum PointerStyle {
		PointerStyle_Circle = 0,        //Բ��ָʾ��
		PointerStyle_Indicator = 1,     //ָ��ָʾ��
		PointerStyle_IndicatorR = 2,    //Բ��ָ��ָʾ��
		PointerStyle_Triangle = 3       //������ָʾ��
	};

    explicit ArcWidget(QWidget *parent = nullptr);

    void setArcInfoData(QVector<ArcInfo*> arcInfos);
    void setPointerAngle(double pointerAngle);
    void setBlowUpIndex(int index);

Q_SIGNALS:
    void sig_sendCilckedAngle(float angle);
    void sig_clickedArcIndex();

private:
	int _radius = 80;  //Բ���뾶
    int _arcHeight = 15; //Բ�����
    int magnifyArcIndex = -1;
    double pointerAngle = 90;
    QVector<ArcInfo*> arcInfoList;
    PointerStyle pointerStyle;

protected:
    void paintEvent(QPaintEvent* event);

	void gradientArc(QPainter* painter, int startAngle, int angleLength, QRgb color, bool magnifyArc = false);
	void gradientText(QPainter* painter, int startAngle, int angleLength, QRgb textcolor, QString text);
	void gradientLine(QPainter* painter, int start, int length,int steps, QRgb color);

	void drawPointerCircle(QPainter* painter);
	void drawPointerIndicator(QPainter* painter);
	void drawPointerIndicatorR(QPainter* painter);
	void drawPointerTriangle(QPainter* painter);

	void mousePressEvent(QMouseEvent* event) final;
	void mouseReleaseEvent(QMouseEvent* event) final;

};

#endif // WIDGET_H
