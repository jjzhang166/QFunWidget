/**
 * @file .
 * 
 * 由 wangwei 创建于 2014年8月13日 下午9:27:17 .
 */
#ifndef ORG_WANGWEI_MACROS_H_
#define ORG_WANGWEI_MACROS_H_

#define ToStr1(x)	#x
#define ToStr2(x)	ToStr1(x)

#ifdef QTHREAD_H
#define ThreadID (QString::number((unsigned int)QThread::currentThreadId()))
#else
#define ThreadID ""
#endif

/** 生成一条错误信息,格式:msg@文件名: 行号: 函数(线程ID) */
#define ErrMsg(msg)	((msg)+QString("@" __FILE__ ": " ToStr2(__LINE__) ": ") +QString(Q_FUNC_INFO)+"("+ThreadID+QString(")"))

#define Println(format,...)	(printf(format "\n",##__VA_ARGS__),fflush(stdout))

#define PostError(msg)	throw QtException(msg)

#ifdef NDEBUG
#define Rep(format,...)
#else
#define Rep(format,...)	Println(__FILE__ ": " ToStr2(__LINE__) ": %s(%s): " format,Q_FUNC_INFO,ThreadID,##__VA_ARGS__)
#endif



#endif /* MACROS_H_ */
