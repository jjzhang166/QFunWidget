/**
 * @file .
 * 
 * 由 Administrator 创建于 2014年9月1日 下午5:22:47 .
 */

#include "QFunWidget.h"
#include "QtException.h"
#include "macros.h"

const QRgb QFunWidget::GridMap::_ClrColor = qRgb(0xFF,0xFF,0xFF);

/** 将 con[from,...,end) 区间范围的值填充为 val
 * @warning 函数内假设所有的参数都是合法的 */
template<typename T>
void vec_fill(QVector<T> &con,int from,int end,const T &val){
	for(int ci=from;ci<end;++ci)
		con[ci] = val;
	return ;
}

void QFunWidget::GridMap::setHNum(int hnum){
	if(hnum<0)
		throw QtException(ErrMsg("hnum: "+QString::number(hnum)));
	for(int ci=0;ci<_Map.size();++ci){
		int old_size = _Map.at(ci).size();
		_Map[ci].resize(hnum);
		vec_fill<QRgb>(_Map[ci],old_size,_Map.at(ci).size(),_ClrColor);
	}
}
void QFunWidget::GridMap::setVNum(int vnum){
	if(vnum<0)
		throw QtException(ErrMsg("vnum: "+QString::number(vnum)));
	int oldSize = _Map.size();
	_Map.resize(vnum);
	for(int ci = oldSize;ci<vnum;++ci){
		int old_size = _Map.at(ci).size();
		_Map[ci].resize(hnum());
		vec_fill(_Map[ci],old_size,_Map.at(ci).size(),_ClrColor);
	}
}
void QFunWidget::GridMap::setColorAt(int x,int y,QRgb rgb){
	if(x >= hnum() || x<0 )
		throw QtException(ErrMsg("x: "+QString::number(x)));
	if(y >= vnum() || y<0 )
		throw QtException(ErrMsg("y: "+QString::number(y)));

	_Map[y][x] = rgb;
}

int QFunWidget::GridMap::hnum()const{
	if(vnum() <= 0 )
		throw QtException(ErrMsg("vnum(): "+vnum()));
	return _Map[0].size();
}
int QFunWidget::GridMap::vnum()const{
	return _Map.size();
}

QRgb QFunWidget::GridMap::colorAt(int x,int y)const{
	if(x >= hnum() || x<0 )
		throw QtException(ErrMsg("x: "+QString::number(x)));
	if(y >= vnum() || y<0 )
		throw QtException(ErrMsg("y: "+QString::number(y)));

	return _Map.at(y).at(x);
}



