#include"piece.hpp"


Piece::Piece(std::vector<Point> &data){
  point.push_back(data);
}

std::vector<std::vector<Point>> Piece::getPoint(){  
  return point;
}

int Piece::getSize() {
  return static_cast<int>(point[0].size());
}
