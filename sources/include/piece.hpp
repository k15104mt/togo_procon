#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include<vector>
#include"utility.hpp"

class Piece{
private:
  std::vector<std::vector<Point>> point;
public:
  Piece(std::vector<Point> &data);
  std::vector<std::vector<Point>> getPoint();
};


#endif
