#ifndef _GEOMETRY_HPP_
#define _GEOMETRY_HPP_

#include"utility.hpp"
#include"piece.hpp"
#include<vector>

//引数(ピース情報,設置情報,フレーム情報)で左上頂点座標を返す
Point getPutPoint(std::vector<Piece>&, std::vector<putData>&,std::vector<std::vector<Point>>);

//引数(図形情報即ちPointのvector)で返り値は時計回りなら1を，反時計回りなら-1を，ねじれているなら0
int CheckAngle(std::vector<Point> point);

#endif
