#ifndef _GEOMETRY_HPP_
#define _GEOMETRY_HPP_
#define MAX 256	//NOT処理とかで使う固定長配列の要素数(分割ベクトルや頂点数)


#include"utility.hpp"
#include"piece.hpp"
#include<vector>



class Geometry {
public:
	Point getPutPoint(std::vector<Piece> &data, std::vector<putData> &already_put,int putMode);	//設置し左上座標取得,UP,RIGHT,DOWN,RIGHT,UP_LEFT
	void cancelPut();																//設置した後外す
	std::vector<std::vector<Point>> areaPoint;										//未設置部エリアの情報
	bool canPut(std::vector<Piece> &data, std::array<int, 100> &isPut);	//エリア面積<未設置ピース面積の場合0を，逆なら1を返す
	Geometry(std::vector<std::vector<Point>> &framePoint);							//コンストラクタ
private:
	std::vector<std::vector<std::vector<Point>>> tmpAreaPoint;		//cancel用の設置前エリア保持
	double minSurface;		//発生したエリアの最小面積
	double minAngle;		//発生したエリアの最小角度
	//int putNum;
};

#endif
