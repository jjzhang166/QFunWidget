/**
 * @file .
 * 
 * 由 Administrator 创建于 2014年8月15日 下午1:24:07 .
 */
#include "../include/QtException.h"
#include <QtCore/QTextCodec>

const char* QtException::what() const throw(){
	QTextCodec *local = QTextCodec::codecForLocale();/* 便于本地化输出 */
	return local->fromUnicode(this->errMsg()).constData();
}

QtException::~QtException()throw(){
	;
}
