#ifndef _GEOMETRY_HPP_
#define _GEOMETRY_HPP_
#define MAX 256

#include"utility.hpp"
#include"piece.hpp"
#include<vector>

struct D_Point {
	double x;
	double y;
};

class Geometry {
public:
	Point getPutPoint(std::vector<Piece> &data, std::vector<putData> &already_put);
	std::vector<std::vector<Point>> areaPoint;
	Geometry(std::vector<std::vector<Point>> framePoint);
private:
	int putNum;
	int getPutNum() { return putNum; }
};

//引数(ピース情報,設置情報,フレーム情報)で左上頂点座標を返す
Point getPutPoint(std::vector<Piece>&, std::vector<putData>&,std::vector<std::vector<Point>>);

//引数(図形情報即ちPointのvector)で返り値は時計回りなら1を，反時計回りなら-1を，ねじれているなら0
int CheckAngle(std::vector<Point> point);

#endif
