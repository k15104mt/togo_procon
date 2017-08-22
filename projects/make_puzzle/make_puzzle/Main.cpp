#define NO_S3D_USING
# include <Siv3D.hpp>
#include"utility.hpp"

struct  data{
  int x;
  int y;
};

void Main(){
  s3d::Window::Resize(1200,700);

  std::vector<Point> frame;
  std::vector<std::vector<Point>> piece;
  std::vector<Point> now;

  std::array<std::array<Point, 101>, 65> plot;
  std::array<std::array<s3d::Circle,101>,65> circle;
  for (int i = 0; i < 65; i++) {
	for (int j = 0; j < 101; j++) {
	  plot[i][j].x = j * 10;
	  plot[i][j].y = i * 10;
	  circle[i][j]= s3d::Circle(j * 10+10, i * 10+10, 2);
	}
  }

  auto checkHit = [=]() {
	data tmp;
	tmp.x = -1;
	tmp.y = -1;

	for (int i = 0; i < 65; i++) {
	  for (int j = 0; j < 101; j++) {
		if (circle[i][j].leftClicked) {
		  tmp.x = i;
		  tmp.y = j;
		  return tmp;
		}
	  }
	}
	return tmp;
  };

  while (s3d::System::Update()){
	for (auto i : circle) { for (auto j : i) { j.draw(s3d::Color(255, 0, 0)); } }

	if (now.size() && s3d::Input::MouseR.clicked) {
	  now.pop_back();
	}

	if (now.size()) {
	  for (int i = now.size()-1; i > 0;i--) {
		s3d::Line(now[i].x+10, now[i].y+10, now[i - 1].x+10, now[i - 1].y+10).draw(s3d::Color(100,100,100));
	  }
	  s3d::Line(now[now.size() - 1].x+10, now[now.size() - 1].y+10, s3d::Mouse::Pos().x, s3d::Mouse::Pos().y).draw(s3d::Color(125,125,125));
	}

	if (frame.size() == 0) {
	  data tmp;
	  if ((tmp = checkHit()).x != -1) {
		if (now.size()>2 && now[0] == plot[tmp.x][tmp.y]) {
		  frame = now;
		  now.clear();
		}else {
		  now.push_back(plot[tmp.x][tmp.y]);
		}
	  }
	}
	else {
	  for (int i = 0;i < static_cast<int>(frame.size());i++){
		s3d::Line( frame[i].x + 10, frame[i].y + 10, frame[(i+1)%frame.size()].x + 10, frame[(i + 1) % frame.size()].y + 10).draw(s3d::Color(0, 0, 255));
	  }
	  if (now.size()){
		for (int i = now.size() - 1; i > 0; i--) {
		  s3d::Line(now[i].x + 10, now[i].y + 10, now[i - 1].x + 10, now[i - 1].y + 10).draw(s3d::Color(100, 100, 100));
		}
		s3d::Line(now[now.size() - 1].x + 10, now[now.size() - 1].y + 10, s3d::Mouse::Pos().x, s3d::Mouse::Pos().y).draw(s3d::Color(125, 125, 125));
	  }

	  if (piece.size()) {
		for (int i = 0; i < static_cast<int>(piece.size());i++){
		  for (int j = 0; j < static_cast<int>(piece[i].size()); j++) {
			s3d::Line(piece[i][j].x + 10, piece[i][j].y + 10, piece[i][(j + 1) % piece[i].size()].x + 10, piece[i][(j + 1) % piece[i].size()].y + 10).draw(s3d::Color(0,255,0));
		  }
		}
	  }

	  data tmp;
	  if ((tmp = checkHit()).x != -1) {
		if (now.size() && now[0] == plot[tmp.x][tmp.y]) {
		  piece.push_back(now);
		  now.clear();
		}
		else {
		  now.push_back(plot[tmp.x][tmp.y]);
		}
	  }


	}
  }
}
