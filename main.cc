/**
 * @file .
 * 
 * 由 Administrator 创建于 2014年9月1日 下午2:58:43 .
 */
#include <QtGui/QtGui>
#include <QtGui/QtGui>

#include "QFunWidget.h"
#include "TestQFunWidget.h"

int main(int argc,char *argv[]){
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QApplication app(argc,argv);

	TestQFunWidget widget;
	widget.show();

	return app.exec();
}
