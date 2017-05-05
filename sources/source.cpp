#include<iostream>
#include<stdio.h>
#include<vector>

using namespace std;

//頂点座標を扱うPoint型構造体
typedef struct{
  int x;
  int y;
}Point;

class Piece{
  private:
    vector<Point> point; //頂点を格納するPoint型動的配列
  public:
    Piece(int pointNum,vector<Point> &data);  //引数：一つのピースの頂点数と構造体配列
    Point getPoint(int n);    //引数：取得するピースの頂点数
};

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

int main() {
  vector<Point> data(5);
  
  //仮データ
  for(int i=0;i<5;i++){
    data[i].x=rand() %6+1;
    data[i].y=rand() %6+1;
  }

  Piece pieceObj(5,data);
  cout<<"Pieceオブジェクト作成"<<endl;
  
  for(int i=0;i<5;i++){
    printf("ピース[%d]:(%d,%d)\n",i,pieceObj.getPoint(i).x,pieceObj.getPoint(i).y);
  }
  return 0;
}
