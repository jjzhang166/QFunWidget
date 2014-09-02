/**
 * @file .
 * 
 * 由 WangWei 创建于 2014年9月1日 下午3:09:07 .
 */
#include <QtCore/qmath.h>
#include <QtCore/QSize>
#include <QtCore/QList>
#include <QtCore/QTime>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QMessageBox>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkInterface>

#include "QtException.h"
#include "QFunWidget.h"
#include "QFunWidgetPacket.h"
#include "macros.h"

static QList<QNetworkInterface> localNetworkInterfaces =  QNetworkInterface::allInterfaces();/** 本机接口集合 */
static QList<QHostAddress> localHostAddresses = QNetworkInterface::allAddresses();
const QHostAddress QFunWidget::_MultiCast("224.93.07.17");

/** 在当前已知的所有网络接口上,发送UDP广播数据包.
 * @param udp 套接字
 * @param data UDP数据包的数据部分 */
void advanceBoardcast(QUdpSocket *udp,int _Port,const QByteArray &data){
	udp->writeDatagram(data,QFunWidget::_MultiCast,_Port);
}

/* 如果 from 属于本地地址之一,则返回真;否则返回假 */
bool isFromLocal(const QHostAddress &from){
	return localHostAddresses.contains(from);
}

/** 根据IP地址from来计算颜色 */
QRgb colorFrom(const QHostAddress &from){
	if(from.protocol() == QAbstractSocket::IPv4Protocol){
		quint32 ip = from.toIPv4Address();
		int r = ((ip & 0x00ff0000) >> 16);
		int g = ((ip & 0x0000ff00) >> 8);
		int b = ((ip & 0x000000ff));
		return qRgb(r,g,b);
	}else
		return qRgb(qrand()%256,qrand()%256,qrand()%256);/* 换成随机数,会不会好一点呢 */
}

QFunWidget::QFunWidget(int hnum ,int vnum,int zoomFactor,QWidget *parent):
	QWidget(parent),
	_Map(hnum,vnum),
	_ZoomFactor(zoomFactor),
	_Color(color()),
	_Udp(new QUdpSocket(this))
{
	connect(_Udp,SIGNAL(readyRead()),this,SLOT(onUdpClientReadyRead()));
	if( !_Udp->bind(QHostAddress::Any,_Port) ||  !_Udp->joinMulticastGroup(_MultiCast))
		QMessageBox::warning(this,"将无法收取信息",_Udp->errorString());

	QTime time =  QTime::currentTime();
	qsrand(time.msec()+time.second()*1000);
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
	setXY(e->pos(),(e->buttons() & Qt::LeftButton)!=0);
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
			if (!e->region().intersected(rect).isEmpty())
				painter.fillRect(rect,_Map.colorAt(ci,cj));
		}
	}
}

QRgb QFunWidget::color() {
	if(localHostAddresses.isEmpty())
		return qRgb(qrand()%256,qrand()%256,qrand()%256);
	else
		return colorFrom(localHostAddresses.at(0));
}

void QFunWidget::setXY(const QPoint &pos,bool set){
	int xnum = pos.x()/(zoomFactor()+gridLineSize());/* (xnum,ynum)就是鼠标指向网格的坐标 */
	int ynum = pos.y()/(zoomFactor()+gridLineSize());

	try{
		if(set)
			_Map.setColorAt(xnum,ynum,_Color);
		else
			_Map.clearXY(xnum,ynum);
	}catch(const QtException &){
		return ;
	}
	QFunWidgetPacket packet;
	packet.setVersion(1);
	packet.setOpCode(set);
	packet.setX(xnum);
	packet.setY(ynum);
	advanceBoardcast(_Udp,_Port,packet.serialize());
	update(drawRect(xnum,ynum));
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

/* 此时收到了网络上的数据包,并处理收来的数据包 */
void QFunWidget::onUdpClientReadyRead(){
	QByteArray buf;
	QHostAddress from;

	while(_Udp->hasPendingDatagrams()){
		buf.resize(_Udp->pendingDatagramSize());
		int realead = _Udp->readDatagram(buf.data(),buf.size(),&from,0);
		if(realead > 0){
			if(realead != buf.size())
				buf.resize(realead);
			if(!isFromLocal(from)){/* 由本地发送的数据包将会被过滤  */
				QFunWidgetPacket packet;
				try{
					QFunWidgetPacket::createFrom(buf,packet);
					if(packet.opCode() == 0 )
						_Map.clearXY(packet.x(),packet.y());
					else
						_Map.setColorAt(packet.x(),packet.y(),colorFrom(from));
					update();
				}catch(const QtException &){/* 抛出异常,则表明本次接受的数据包不合法,所以 */
					continue;
				}
			}
		}else{
			QString str = "读取 ";
			str += QString::number(_Port);
			str += " 端口出错: ";
			str += _Udp->errorString();
			str += ";将无法收到其他小伙伴的轨迹";
			QMessageBox::warning(this,"读取失败",str);
		}
	}
}

