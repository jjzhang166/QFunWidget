/**
 * @file .
 * 
 * 由 Administrator 创建于 2014年9月1日 下午7:46:42 .
 */
#ifndef ORG_WANGWEI_QFUNWIDGET_TESTQFUNWIDGET_H_
#define ORG_WANGWEI_QFUNWIDGET_TESTQFUNWIDGET_H_

#include <QtGui/QWidget>

class QFunWidget;
class QLineEdit;

class TestQFunWidget: public QWidget {
	Q_OBJECT
public:
	TestQFunWidget(QWidget *parent = 0);

private slots:
	void onHNumLineEditEnter();
	void onVNumLineEditEnter();
	void onZoomLineEditEnter();
private:
	enum{ HNum=0,VNum,Zoom,LineEditTotalNum };

	QLineEdit *_LineEdits[LineEditTotalNum];
	QFunWidget *_FunWidget;
};

#endif /* TESTQFUNWIDGET_H_ */
