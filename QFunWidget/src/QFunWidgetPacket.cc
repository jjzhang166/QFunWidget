/**
 * @file .
 * 
 * 由 Administrator 创建于 2014年9月2日 下午12:11:15 .
 */

#include <string.h>
#include <QtCore/QtEndian>

#include "QFunWidgetPacket.h"
#include "QtException.h"
#include "macros.h"

QFunWidgetPacket& QFunWidgetPacket::operator=(const QFunWidgetPacket &o){
	this->copyFrom(o);
	return *this;
}
void QFunWidgetPacket::copyFrom(const QFunWidgetPacket &o){
	/* memcpy()会不会好点.. */
	this->setOpCode(o.opCode());
	this->setVersion(o.version());
	this->setX(o.x());
	this->setY(o.y());
}
/** 将数据包使用网络字节序序列化  */
QByteArray QFunWidgetPacket::serialize()const{
	Packet p;
	memcpy(&p,&_Packet,sizeof(_Packet));
	p._X = qToBigEndian(p._X);/* 转换为网络字节序 */
	p._Y = qToBigEndian(p._Y);

	return QByteArray((const char*)&p,sizeof(p));
}

/** [buf,buf+bufSize)为从网络上接收过来的数据包,依次来构建一个packet. */
QFunWidgetPacket& QFunWidgetPacket::createFrom(const char *buf,int bufSize,QFunWidgetPacket &packet){
	if(bufSize < (int)sizeof(packet._Packet))
		throw QtException(ErrMsg("bufSize: "+QString::number(bufSize)));
	memcpy(&packet._Packet,buf,sizeof(packet._Packet));
	packet._Packet._X = qFromBigEndian(packet._Packet._X);
	packet._Packet._Y = qFromBigEndian(packet._Packet._Y);

	if(packet.version() != 1)/* 不支持的数据包格式 */
		throw QtException(ErrMsg("_Packet._Version: "+QString::number((unsigned int)packet.version())));
	if(packet.opCode() >= 2)
		throw QtException(ErrMsg("_Packet._OpCode: "+QString::number((unsigned int)packet.opCode())));

	return packet;
}
