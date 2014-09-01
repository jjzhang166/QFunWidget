/**
 * @file .
 * 
 * 由 Administrator 创建于 2014年9月1日 下午2:58:43 .
 */
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

#include "QFunWidget.h"
#include "TestQFunWidget.h"

int main(int argc,char *argv[]){
	QApplication app(argc,argv);

	TestQFunWidget widget;
	widget.show();

	return app.exec();
}
