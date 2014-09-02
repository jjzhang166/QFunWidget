/**
 * @file .
 * 
 * 由 WangWei 创建于 2014年9月1日 下午3:09:07 .
 */
#include <QtCore/qmath.h>
#include <QtCore/QSize>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#include "QtException.h"
#include "QFunWidget.h"
#include "macros.h"

QFunWidget::QFunWidget(int hnum ,int vnum,int zoomFactor,QWidget *parent):
	QWidget(parent),
	_Map(hnum,vnum),
	_ZoomFactor(zoomFactor),
	_Color(color())
{
	;
}

void QFunWidget::setHNum(int hnum){
	if(hnum < 0)
		throw QtException(ErrMsg("hnum: "+hnum));
	if(hnum != this->hnum()){
		_Map.setHNum(hnum);
		updateGeometry();/* 注意,不要忘了 */
		update();
	}
}

void QFunWidget::setVNum(int vnum){
	if(vnum < 0)
		throw QtException(ErrMsg("vnum: "+vnum));
	if(vnum != this->vnum()){
		_Map.setVNum(vnum);
		updateGeometry();
		update();
	}
}

void QFunWidget::setZoomFactor(int zoomFactor){
	if(zoomFactor < 0)
		throw QtException(ErrMsg("zoomFactor: "+zoomFactor));
	if(zoomFactor != this->zoomFactor()){
		_ZoomFactor = zoomFactor;
		updateGeometry();
		update();
	}
}

/** 鼠标监听,若点击左键,则对鼠标所指向的网格使用指定的颜色上色,若点击右键,则清除指定网格的颜色*/
void QFunWidget::mousePressEvent(QMouseEvent *e){
	if(e->button() != Qt::LeftButton && e->button() != Qt::RightButton)
		return ;/* 如果点击了其他按钮,则直接返回 */
	setXY(e->pos(),e->button()==Qt::LeftButton);
}

/** 鼠标移动,若鼠标移动时一直按钮左键,则对鼠标经过的区域上色,若按住右键,则清除指定网格的颜色 */
void QFunWidget::mouseMoveEvent(QMouseEvent *e){
	if(( e->buttons() & Qt::LeftButton ) && ( e->buttons() & Qt::RightButton ))
		return ;/* 如果同时按钮左键/右键,则返回 */
	if( (e->buttons() & Qt::LeftButton ) ==0 && (e->buttons() & Qt::RightButton)==0 )
		return ;/* 如果即没有按住左键,也没有按住右键,则返回 */
	setXY(e->pos(),e->buttons() & Qt::LeftButton);
}

void QFunWidget::resizeEvent(QResizeEvent *e){
	double tmp1 = this->width() - 1;
	tmp1 /= this->hnum();
	setZoomFactor(qFloor(tmp1 - 1));
}

void QFunWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    /* 绘制网格 */
    int zoom = zoomFactor()+1;
	painter.setPen(palette().foreground().color());
	for (int i = 0; i <= this->hnum(); ++i)
		painter.drawLine(zoom * i, 0,zoom * i, zoom * vnum());
	for (int j = 0; j <= this->vnum(); ++j)
		painter.drawLine(0, zoom * j,zoom * this->hnum(), zoom * j);

	for(int ci = 0;ci<_Map.hnum();++ci){
		for(int cj=0;cj<_Map.vnum();++cj){
			QRect rect = drawRect(ci,cj);
			painter.fillRect(rect,_Map.colorAt(ci,cj));
		}
	}
}

QRgb QFunWidget::color() {
	return qRgb(93,07,17);/* TODO 后期改为由IP地址来决定颜色值 */
}

void QFunWidget::setXY(const QPoint &pos,bool set){
	int xnum = pos.x()/(zoomFactor()+gridLineSize());/* (xnum,ynum)就是鼠标指向网格的坐标 */
	int ynum = pos.y()/(zoomFactor()+gridLineSize());

//	if(xnum >= this->hnum() || ynum >= this->vnum())/* 越界,将该判断放入_Map.setXY()内部进行 */
//		return ;
	try{
		if(set)
			_Map.setColorAt(xnum,ynum,_Color);
		else
			_Map.clearXY(xnum,ynum);
	}catch(const QtException &){
		return ;
	}
	/* TODO 发送UDP广播数据包 */
	update();/* TODO 这里应该产生一个局部重绘 */
}

/** 返回x,y处的网格所对应的矩形区域,不包括网格四边的网格线 */
QRect QFunWidget::drawRect(int i,int j)const{
	int zoom = zoomFactor() + 1;
	return QRect(zoom * i + 1, zoom * j + 1, zoomFactor(),zoomFactor());
}

QSize QFunWidget::sizeHint() const{
	return calcSize(zoomFactor());
}
QSize QFunWidget::minimumSizeHint() const{
	return calcSize(_MinZoomFactor);
}

QSize QFunWidget::calcSize(int zoomFactor)const{
	return QSize(hnum()*(zoomFactor+1)+1,vnum()*(zoomFactor+1)+1);
}

