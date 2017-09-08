#include"utility.hpp"

#include<sstream>



std::vector<std::string> split(std::string input, char spilit_character) {
  std::stringstream stream(input);

  std::vector<std::string> result;
  std::string temp;
  while (std::getline(stream, temp, spilit_character)) {
	result.push_back(temp);
  }

  return result;
}


putData::putData(int piece_num, int point_num, int vertex_num, Point &base_point) :
  piece_num(piece_num), point_num(point_num), vertex_num(vertex_num), base_point(base_point) {}


//内積ィ！！
int dot(Vector &a,Vector &b){
  return a.x * b.x + a.y * b.y;
}

//外積ィ！！
int cross(Vector &a, Vector &b) {
  return a.x * b.y - a.y * b.x;
}

//渡されたvectorの値を直接変えるから注意
void move(std::vector<Point> &data, Point movingDistance) {
  for (auto &i : data) {
	i += movingDistance;
  }
}


//二つの線分の始点、終点から、線分同士が交差しているかを返す
//線分が平行な場合は交差していないものとなる
int cl(Point &Line1Start, Point &Line1End,
  Point &Line2Start, Point &Line2End) {

  if ((((Line1Start.x - Line1End.x)*(Line2Start.y - Line1Start.y) +
	(Line1Start.y - Line1End.y)*(Line1Start.x - Line2Start.x))*
	((Line1Start.x - Line1End.x)*(Line2End.y - Line1Start.y) +
	(Line1Start.y - Line1End.y)*(Line1Start.x - Line2End.x)) < 0) &&
	  (((Line2Start.x - Line2End.x)*(Line1Start.y - Line2Start.y) +
	(Line2Start.y - Line2End.y)*(Line2Start.x - Line1Start.x))*
		((Line2Start.x - Line2End.x)*(Line1End.y - Line2Start.y) +
		(Line2Start.y - Line2End.y)*(Line2Start.x - Line1End.x)) < 0)) {
	return 1;
  }

  return 0;
}

int crossLine(std::vector<Point> &data1, std::vector<Point> &data2) {
  for (int i = 0; i < static_cast<int>(data1.size()); ++i) {
	for (int j = 0; j < static_cast<int>(data2.size()); ++j) {
	  if (cl(data1[i], data1[(i + 1) % static_cast<int>(data1.size())],
		data2[j], data2[(j + 1) % static_cast<int>(data2.size())])) {
		return 1;
	  }
	}
  }
  return 0;
}

//data1にdata２の頂点がすべて内包されてい
int inPolygon(std::vector<Point> &data1, std::vector<Point> &data2) {
  for (int i = 0; i < static_cast<int>(data2.size()); ++i) {
	for (int j = 0; j < static_cast<int>(data1.size()); ++j) {

	  if (data1[j] == data2[i]) {
		if (cross(data1[j] - data1[(j + data1.size() - 1) % data1.size()],
		  data1[(j + 1) % data1.size()] - data1[(j + data1.size() - 1) % data1.size()])>0) {

		  if (cross(data1[(j + data1.size() - 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) <= 0 &&
			cross(data1[(j + 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) >= 0) {
			
		  }
		  else {
			return 0;
		  }

		}
		else if (cross(data1[j] - data1[(j + data1.size() - 1) % data1.size()],
		  data1[(j + 1) % data1.size()] - data1[(j + data1.size() - 1) % data1.size()])<0) {

		  if (cross(data1[(j + data1.size() - 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) > 0 &&
			cross(data1[(j + 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) < 0) {
			return 0;
		  }

		}
	  }

	  /*
	  if (data1[j] == data2[i]) {
		if (cross(data1[j] -data1[(j + data1.size() - 1) % data1.size()],
		  data1[(j + 1) % data1.size()] - data1[(j + data1.size() - 1) % data1.size()])>0) {

		  if (cross(data1[(j + data1.size() - 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) >= 0 &&
			cross(data1[(j + 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) <= 0) {
			return 0;
		  }

		}
		else if (cross(data1[j] - data1[(j + data1.size() - 1) % data1.size()],
		  data1[(j + 1) % data1.size()] - data1[(j + data1.size() - 1) % data1.size()])<0) {

		  if (cross(data1[(j + data1.size() - 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) <= 0 &&
			cross(data1[(j + 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) >= 0) {
			printf("(%d %d)\n", cross(data1[(j + data1.size() - 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i])
			  , cross(data1[(j + 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]));
			
			return 0;
		  }

		}
	  }*/
	  /*
	  if (data1[j] == data2[i] &&
		cross(data1[(j + data1.size() - 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) >= 0 &&
		cross(data1[(j + 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] -data2[i]) <= 0) {
		printf("%d %d\n", j, i);
		return 0;
	  }
	  */
	}
  

  //data2のi番目の点がdata1のポリゴン内にあるかどうか
	double x = 0.0;
	int flag = 1;
	for (int j = 0; j < static_cast<int>(data1.size()); ++j) {
	  Vector a = data1[(j + 1) % data1.size()] - data2[i];
	  Vector b = data1[j] - data2[i];

	  if (data1[j] == data2[i]) {
		flag = 0;
		break;
	  }

	  if (dot(a, b) <= 0 && cross(a, b) == 0) {
		flag = 0;
		break;
	  }


	  //角度を積極的に足していこう
	  if (cross(a, b) < 0) x += (acos((double)(dot(a, b)) / (a.size()*b.size()))*180.0 / acos(-1.0));
	  if (cross(a, b) > 0) x += -1.0*(acos((double)(dot(a, b)) / (a.size()*b.size()))*180.0 / acos(-1.0));
	}

	//天上にもなく、360じゃないときはさっさと終わらせる
	//ここ多分誤差を考慮する感じにしたほうがいい
	if (flag && (x >= 360.01 || x <= 359.99)) {
	  return 0;
	}
  }
  return 1;
}

int inEvenOnePolygon(std::vector<Point> &data1, std::vector<Point> &data2) {

  //1つでも内包されている点があるならreturn 1;
  for (int i = 0; i < static_cast<int>(data2.size()); ++i) {
	double x = 0.0;
	int flag = 1;
	for (int j = 0; j < static_cast<int>(data1.size()); ++j) {
	  Vector a = data1[(j + 1) % data1.size()] - data2[i];
	  Vector b = data1[j] - data2[i];

	  //頂点上にある場合
	  if (data1[j] == data2[i]) {
		if (cross(data1[j] - data1[(j + data1.size() - 1) % data1.size()],
		  data1[(j + 1) % data1.size()] - data1[(j + data1.size() - 1) % data1.size()])>0) {

		  if (cross(data1[(j + data1.size() - 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) <= 0 &&
			cross(data1[(j + 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) >= 0) {
			//もし頂点が内包されているなら
			return 1;
		  }
		}
		else if (cross(data1[j] - data1[(j + data1.size() - 1) % data1.size()],
		  data1[(j + 1) % data1.size()] - data1[(j + data1.size() - 1) % data1.size()])<0) {

		  if (cross(data1[(j + data1.size() - 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) > 0 &&
			cross(data1[(j + 1) % data1.size()] - data1[j], data2[(i + 1) % data2.size()] - data2[i]) < 0) {

		  }
		  else {
			//もし頂点が内包されているなら
			return 1;
		  }
		}
		flag = 0;
		break;
	  }else if (dot(a, b) <= 0 && cross(a, b) == 0) {//直線上にある場合
		if (cross(a, data2[(i + 1) % data2.size()] - data2[i])>0) {
		  //return 1;
		  
		}

		flag = 0;
		break;
	  }

	  //角度を積極的に足していこう
	  if (cross(a, b) < 0) x += (acos((double)(dot(a, b)) / (a.size()*b.size()))*180.0 / acos(-1.0));
	  if (cross(a, b) > 0) x += -1.0*(acos((double)(dot(a, b)) / (a.size()*b.size()))*180.0 / acos(-1.0));
	}

	//頂点上にもなくて直線上にもないなら360のときは内包されているから終わる
	//ここ多分誤差を考慮する感じにしたほうがいい
	if (flag && (359.99 <= x && x <= 360.01)) {
	  return 1;
	}
  }
  return 0;
}

int collisionPiece(std::vector<Point> &data1, std::vector<Point> &data2) {
  //線が一つでも交わっていたら当たっている（それはそう）
  if (crossLine(data1, data2)) {
	printf("cl");
	return 1;
  }

  if (inEvenOnePolygon(data1, data2) || inEvenOnePolygon(data2, data1)) {
	return 1;
  }

  /*
  if (inPolygon(data1, data2) || inPolygon(data2, data1)) {
	printf("in");
	return 1;
  }
  */
  /*
  for (int i = 0; i < static_cast<int>(data1.size()); i++){
	for (int j = 0; j < static_cast<int>(data2.size()); ++j) {


	  //両方が頂点上にある場合のみのやつ作る

	  if (!(data1[i] == data2[j]) && !(data1[i] == data2[(j + 1) % data2.size()])) {
		Vector a = data2[j] - data1[i];
		Vector b = data2[(j + 1) % data2.size()] - data1[i];
		Vector c = data1[(i + 1) % data1.size()] - data1[i];
		if (cross(a,b)==0 && dot(a,b)<0 && cross(b,c)>0) {//線分上に頂点があるとき
		  printf("??");
		  return 1;
		}
	  }
	  /*これいるかわかんないや！！！！
	  if (data1[i] == data2[j]) {
		Vector a = data1[(i + 1) % data1.size()] - data1[i];
		Vector b = data2[(j + 1) % data2.size()] - data2[j];
		if (cross(a, b) > 0) {
		  printf("!?");
		  return 1;
		}
	  }//*
	  
	}
  }
  */

  /*
  for (int i = 0; i<static_cast<int>(data1.size()); ++i) {
	for (int j = 0; j<static_cast<int>(data2.size()); ++j) {
	  Vector a = data2[(j + 1) % data2.size()] - data1[i];
	  Vector b = data2[j] - data1[i];
	  if (cross(a, b) == 0 && dot(a, b) < 0) {
		Vector c = data1[(i + 1) % data1.size()] - data1[i];
		Vector d = data2[(j + 1) % data2.size()] - data2[j];

		if (cross(d, c) < 0) {
		  //return 1;
		}
	  }

	  /*
	  Vector a = data1[(i + 1) % data1.size()] - data1[i];
	  Vector b = data2[(j + 1) % data2.size()] - data2[j];
	  //外積が平行なとき→ベクトルが平行
	  if (cross(a, b) == 0) {
		
		//どちらかの頂点がのみが片方の辺の中に存在しているとき
		Vector c = data2[(j + 1) % data2.size()] - data1[i];
		Vector d = data2[j]                      - data1[i];
		Vector e = data2[(j + 1) % data2.size()] - data1[(i + 1) % data1.size()];
		Vector f = data2[j]                      - data1[(i + 1) % data1.size()];

		if (cross(c, d) == 0 && cross(e, f) == 0) {
		  if ((dot(c, d) < 0 && dot(e, f) > 0) || (dot(c, d) > 0 && dot(e, f) < 0)) {
			return 1;
		  }
		}

		if (cross(e, f) == 0) {
		  if (c.size()==0.0 || d.size() == 0.0) {
			if (dot(e, f) < 0) {
			  return 1;
			}
		  }
		}

		if (cross(c, d) == 0) {
		  if (e.size() == 0.0 || f.size() == 0.0) {
			if (dot(c, d) < 0) {
			  return 1;
			}
		  }
		}
	  }
	}
  }
  */
  return 0;  
}

//data1 == frame
int collisionFrame(std::vector<Point> &data1,std::vector<Point> &data2){
  if(crossLine(data1,data2)){
	printf("cl");
    return 1;
  }
  
  if(!inPolygon(data1,data2)){
	printf("in");
    return 1;
  }
  /*
  for (int i = 0; i < static_cast<int>(data2.size()); ++i) {
	for (int j = 0; j < static_cast<int>(data1.size()); ++j) {
	  Vector a = data1[(j + 1) % data1.size()] - data2[i];
	  Vector b = data1[j] - data2[i];

	  //頂点上に頂点があるときはそのあとのベクトルより反時計回り(外積=正)ならHIT
	  if (data2[i] == data1[j]) {
		Vector c = data2[(i + 1) % data2.size()] - data2[i];
		Vector d = data1[(j + 1) % data1.size()] - data1[j];
		//下向きにyが正だから外積は反時計回りのときに負になる
		if (cross(d, c) < 0) {
		  printf("!?");
		  return 1;
		}
	  }else if (data2[i] == data1[(j + 1) % data1.size()]) {
		Vector c = data2[(i + 1) % data2.size()] - data2[i];
		Vector d = data1[(j + 2) % data1.size()] - data1[(j + 1) % data1.size()];
		if (cross(d, c) < 0) {
		  printf("?!");
		  return 1;
		}
	  }else if (cross(a, b) == 0 && dot(a, b) < 0) {
		Vector c = data2[(i + 1) % data2.size()] - data2[i];
		Vector d = data1[(j + 1) % data1.size()] - data1[j];

		if (cross(d, c) < 0) {
		  printf("??");
		  return 1;
		}
	  }
	}
  }*/

  
  return 0;  
}

