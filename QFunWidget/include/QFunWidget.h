/**
 * @file .
 * 
 * 由 WangWei 创建于 2014年9月1日 下午3:09:07 .
 */
#ifndef ORG_WANGWEI_QFUNWIDGET_H_
#define ORG_WANGWEI_QFUNWIDGET_H_

#include <QtCore/QBitArray>
#include <QtCore/QVector>
#include <QtWidgets/QWidget>

/** 一个有趣的窗体.
 * 使用方法:
 * 	构建一个QFunWidget对象,可以指定水平方向,竖直方向上网格的个数,同时也可以指定网格的大小.
 * 	然后显示出来即可 */
class QFunWidget: public QWidget {
public:
	QFunWidget(int hnum = 93 ,int vnum = 77,int zoomFactor = 7,QWidget *parent = 0);
	void setHNum(int hnum);
	void setVNum(int vnum);
	void setZoomFactor(int zoomFactor);

	inline int hnum()const;
	inline int vnum()const;
	inline int zoomFactor()const;

	inline static int gridLineSize();

protected:
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void paintEvent(QPaintEvent *);
	virtual void resizeEvent(QResizeEvent *);

	virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

private:
/** 保存QFunWidget上每一个网格的上色情况,由于颜色是固定的,所以只需要保存网格是否上色与否即可.
 * 在GridMap中,使用1bit来保存一个网格的上色情况. */
class GridMap{
public:
	inline GridMap(int hnum,int vnum);
	void setHNum(int hnum);
	void setVNum(int vnum);
	void setXY(int x,int y,bool set=true);
	inline void clearXY(int x,int y);

	int hnum()const;
	int vnum()const;
	bool testXY(int x,int y)const;
private:
	QVector<QBitArray> _Map;
};

private:
	static QRgb color() ;

	void setXY(const QPoint &pos,bool set);
	QRect drawRect(int x,int y)const;
	QSize calcSize(int zoomFactor)const;

	GridMap _Map;
	int _ZoomFactor;/* 指定了网格的大小为_ZoomFactor*_ZoomFactor,单位为像素 */
	QRgb _Color;/* 对网格上色时使用的颜色,根据本机IP地址决定 */

	static const int _GridLineSize = 1;/** 网格线的宽度/高度,为1个像素 */
	static const int _MinZoomFactor = 3;
};

inline int QFunWidget::hnum()const{
	return _Map.hnum();
}

inline int QFunWidget::vnum()const{
	return _Map.vnum();
}

inline int QFunWidget::zoomFactor()const{
	return _ZoomFactor;
}

inline int QFunWidget::gridLineSize(){
	return _GridLineSize;
}

inline QFunWidget::GridMap::GridMap(int hnum,int vnum):
	_Map(vnum,QBitArray(hnum,false))
{
	;
}
inline void QFunWidget::GridMap::clearXY(int x,int y){
	this->setXY(x,y,false);
}


#endif /* ORG_WANGWEI_QFUNWIDGET_H_ */
