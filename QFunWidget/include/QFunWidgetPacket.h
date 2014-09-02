/**
 * @file .
 * 
 * 由 Administrator 创建于 2014年9月2日 下午12:11:15 .
 */
#ifndef ORG_WANGWEI_QFUNWIDGETPACKET_H_
#define ORG_WANGWEI_QFUNWIDGETPACKET_H_

#include <QtCore/QtGlobal>
#include <QtCore/QByteArray>


/** QFunWidget中使用的数据包
 * 内部以主机字节序来保存,仅当调用 serialize()时才转换为网络字节序 */
class QFunWidgetPacket {
public:
	inline QFunWidgetPacket();
	inline QFunWidgetPacket(const QFunWidgetPacket &);
	QFunWidgetPacket& operator=(const QFunWidgetPacket &);
	void copyFrom(const QFunWidgetPacket &);

	inline void setOpCode(int opcode);
	inline void setVersion(int version);
	inline void setX(int x);
	inline void setY(int y);

	inline int opCode()const;
	inline int version()const;
	inline int x()const;
	inline int y()const;
	inline int size()const;
	QByteArray serialize()const;

	static QFunWidgetPacket& createFrom(const char *buf,int bufSize,QFunWidgetPacket &packet);
	inline static QFunWidgetPacket& createFrom(const QByteArray &buf,QFunWidgetPacket &packet);

private:
#pragma pack(1)
	struct Packet{
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN /* 小端模式 */
		unsigned char _OpCode: 4;
		unsigned char _Version: 4;
#else								/* 大端模式 */
		unsigned char _Version: 4;
		unsigned char _OpCode: 4;
#endif
		unsigned short _X;
		unsigned short _Y;
	}/*__attribute__((__packed__)) 不起作用 */;
#pragma pack()

	Packet _Packet;
};

/*******************************************************************************************************************************************************************************************************************/
inline QFunWidgetPacket& QFunWidgetPacket::createFrom(const QByteArray &buf,QFunWidgetPacket &packet){
	return QFunWidgetPacket::createFrom(buf.constData(),buf.size(),packet);
}

inline void QFunWidgetPacket::setOpCode(int opcode){
	_Packet._OpCode = (unsigned int)opcode;
}

inline void QFunWidgetPacket::setVersion(int version){
	_Packet._Version = (unsigned int)version;
}
inline void QFunWidgetPacket::setX(int x){
	_Packet._X = x;
}
inline void QFunWidgetPacket::setY(int y){
	_Packet._Y = y;
}

inline int QFunWidgetPacket::opCode()const{
	return _Packet._OpCode;
}
inline int QFunWidgetPacket::version()const{
	return _Packet._Version;
}
inline int QFunWidgetPacket::x()const{
	return _Packet._X;
}
inline int QFunWidgetPacket::y()const{
	return _Packet._Y;
}
/** 返回数据包的长度,单位:字节 */
inline int QFunWidgetPacket::size()const{
	return sizeof(_Packet);
}

inline QFunWidgetPacket::QFunWidgetPacket(){
	;
}

inline QFunWidgetPacket::QFunWidgetPacket(const QFunWidgetPacket &o){
	this->copyFrom(o);
}




#endif /* ORG_WANGWEI_QFUNWIDGETPACKET_H_ */
