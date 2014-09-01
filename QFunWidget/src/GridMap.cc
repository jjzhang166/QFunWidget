/**
 * @file .
 * 
 * 由 Administrator 创建于 2014年9月1日 下午5:22:47 .
 */

#include "QFunWidget.h"
#include "QtException.h"
#include "macros.h"

void QFunWidget::GridMap::setHNum(int hnum){
	if(hnum<0)
		throw QtException(ErrMsg("hnum: "+QString::number(hnum)));
	for(int ci=0;ci<_Map.size();++ci)
		_Map[ci].resize(hnum);
}
void QFunWidget::GridMap::setVNum(int vnum){
	if(vnum<0)
		throw QtException(ErrMsg("vnum: "+QString::number(vnum)));
	int oldSize = _Map.size();
	_Map.resize(vnum);
	for(int ci = oldSize;ci<vnum;++ci)
		_Map[ci].resize(hnum());
}
void QFunWidget::GridMap::setXY(int x,int y,bool set){
	if(x >= hnum() || x<0 )
		throw QtException(ErrMsg("x: "+QString::number(x)));
	if(y >= vnum() || y<0 )
		throw QtException(ErrMsg("y: "+QString::number(y)));

	_Map[y].setBit(x,set);
}

int QFunWidget::GridMap::hnum()const{
	if(vnum() <= 0 )
		throw QtException(ErrMsg("vnum(): "+vnum()));
	return _Map[0].size();
}
int QFunWidget::GridMap::vnum()const{
	return _Map.size();
}

bool QFunWidget::GridMap::testXY(int x,int y)const{
	if(x >= hnum() || x<0 )
		throw QtException(ErrMsg("x: "+QString::number(x)));
	if(y >= vnum() || y<0 )
		throw QtException(ErrMsg("y: "+QString::number(y)));

	return _Map[y].testBit(x);
}



