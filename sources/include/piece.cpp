#include"piece.hpp"

using namespace std;

//Pieceコンストラクタ
Piece::Piece(int pointNum,vector<Point> &data){
  cout<<"コンストラクタ"<<endl;
  point.resize(pointNum); //pointのリサイズ
  point = data;           //頂点座標格納
  
}

//ピース座標のゲッター
Point Piece::getPoint(int n){
  return point[n];
}