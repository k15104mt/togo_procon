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
	Point getPutPoint(std::vector<Piece> &data, std::vector<putData> &already_put);	//設置し左上座標取得
	void cancelPut();																//設置した後外す
	std::vector<std::vector<Point>> areaPoint;										//未設置部エリアの情報
	Geometry(std::vector<std::vector<Point>> framePoint);							//コンストラクタ
private:
	std::vector<std::vector<Point>> tmpAreaPoint;		//cancel用の設置前エリア保持
	int putNum;
	int getPutNum() { return putNum; }
};

//引数(ピース情報,設置情報,フレーム情報)で左上頂点座標を返す
Point getPutPoint(std::vector<Piece>&, std::vector<putData>&,std::vector<std::vector<Point>>);

//引数(図形情報即ちPointのvector)で返り値は時計回りなら1を，反時計回りなら-1を，ねじれているなら0
int CheckAngle(std::vector<Point> point);

#endif
