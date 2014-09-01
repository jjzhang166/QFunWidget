/**
 * @file .
 *
 * 由 Administrator 创建于 2014年8月15日 下午1:22:19 .
 */
#ifndef ORG_WANGWEI_QTEXCEPTION_H_
#define ORG_WANGWEI_QTEXCEPTION_H_

#include <exception>
#include <QtCore/QString>

class QtException: public std::exception {
	QString _ErrMsg;
public:
	inline QtException(const QString &errmsg);
	virtual const char* what() const throw();
	virtual ~QtException()throw();

	inline QString errMsg()const;
	inline void setErrMsg(const QString &errMsg);
};

inline QtException::QtException(const QString &errmsg){
	this->setErrMsg(errmsg);
}

inline QString QtException::errMsg()const{
	return _ErrMsg;
}

inline void QtException::setErrMsg(const QString &errMsg){
	_ErrMsg = errMsg;
}


#endif /* QTEXCEPTION_H_ */
