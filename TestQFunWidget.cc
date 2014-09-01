/**
 * @file .
 * 
 * 由 Administrator 创建于 2014年9月1日 下午7:46:42 .
 */
#include "QFunWidget.h"

#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

#include "TestQFunWidget.h"

const char *LineEditTooltip[]={"水平方向的网格数","竖直方向的网格数","放缩因子"};

TestQFunWidget::TestQFunWidget(QWidget *parent):
	QWidget(parent)
{
	QHBoxLayout *bottom = new QHBoxLayout;
	for(int ci=0;ci<LineEditTotalNum;++ci){
		_LineEdits[ci] = new QLineEdit;
		_LineEdits[ci]->setPlaceholderText(LineEditTooltip[ci]);
		bottom->addWidget(_LineEdits[ci]);
	}
	connect(_LineEdits[HNum],&QLineEdit::returnPressed,this,&TestQFunWidget::onHNumLineEditEnter);
	connect(_LineEdits[VNum],&QLineEdit::returnPressed,this,&TestQFunWidget::onVNumLineEditEnter);
	connect(_LineEdits[Zoom],&QLineEdit::returnPressed,this,&TestQFunWidget::onZoomLineEditEnter);
	QVBoxLayout *root = new QVBoxLayout;
	root->addWidget(_FunWidget = new QFunWidget);
	root->addLayout(bottom);
	setLayout(root);
}

void TestQFunWidget::onHNumLineEditEnter(){
	int hnum = _LineEdits[HNum]->text().toInt();
	if(hnum >= 0)
		_FunWidget->setHNum(hnum);
}
void TestQFunWidget::onVNumLineEditEnter(){
	int hnum = _LineEdits[VNum]->text().toInt();
	if(hnum >= 0)
		_FunWidget->setVNum(hnum);
}
void TestQFunWidget::onZoomLineEditEnter(){
	int hnum = _LineEdits[Zoom]->text().toInt();
	if(hnum >= 0)
		_FunWidget->setZoomFactor(hnum);
}

