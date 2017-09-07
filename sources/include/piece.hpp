#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include<vector>
#include"utility.hpp"

class Piece{
private:
  std::vector<std::vector<Point>> point;	//頂点座標
  double surface;							//ピース面積
public:
  Piece(std::vector<Point> &data);
  std::vector<std::vector<Point>> getPoint();
  int getSize();	
  double getSurface();
};


#endif
