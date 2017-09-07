#define NO_S3D_USING
# include <Siv3D.hpp>
#define NOMINMAX
#include<Windows.h>
#include<algorithm>
#include"utility.hpp"

struct  data {
  int x;
  int y;
};

void Main() {
  s3d::Window::Resize(1250, 700);
  s3d::Window::SetTitle(L"パズル作成");

  s3d::GUI gui(s3d::GUIStyle::Default);
  gui.addln(L"copy",s3d::GUIButton::Create(L"コピー"));
  gui.addln(L"clear", s3d::GUIButton::Create(L"クリア"));
  gui.add(s3d::GUIHorizontalLine::Create());
  gui.addln(L"copyraw", s3d::GUIButton::Create(L"元データのコピー"));
  gui.addln(L"pasteraw", s3d::GUIButton::Create(L"元データから復元"));
  gui.setPos(1020, 0);

  s3d::Font pos(30);

  //■■■■■■■■■■■■■■■■■■■■
  //AllocConsole();
  FILE* out = 0; freopen_s(&out, "CON", "w", stdout);
  FILE* in = 0; freopen_s(&in, "CON", "r", stdin);
  //■■■■■■■■■■■■■■■■■■■■


  std::vector<Point> frame;
  std::vector<std::vector<Point>> piece;
  std::vector<Point> now;

  std::array<std::array<Point, 101>, 65> plot;
  std::array<std::array<s3d::Circle, 101>, 65> circle;
  for (int i = 0; i < 65; i++) {
	for (int j = 0; j < 101; j++) {
	  plot[i][j].x = j * 10;
	  plot[i][j].y = i * 10;
	  circle[i][j] = s3d::Circle(j * 10 + 10, i * 10 + 10, 4);
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

  while (s3d::System::Update()) {
	//点描画
	for (int i = 0; i < static_cast<int>(circle.size()); ++i) {
	  for (int j = 0; j < static_cast<int>(circle[i].size()); ++j) {
		if (i % 5 == 0 || j % 5 == 0) {
		  circle[i][j].draw(s3d::Color(200, 200, 200));
		}
		else {
		  circle[i][j].draw(s3d::Color(100, 100, 100));
		}
	  }
	}

	if (s3d::Input::MouseR.clicked) {
	  if (now.size()) {
		now.pop_back();
	  }
	  else if (piece.size()) {
		now = piece[piece.size()-1];
		piece.pop_back();
	  }
	  else if(frame.size()){
		now = frame;
		frame.clear();
	  }
	  
	}

	if (gui.button(L"clear").pushed) {
	  frame.clear();
	  piece.clear();
	  now.clear();
	}

	if (gui.button(L"pasteraw").pushed) {
	  frame.clear();
	  piece.clear();
	  now.clear();

	  std::vector<std::string>  input;
	  input.push_back(s3d::Clipboard::GetText().narrow());

	  for (int i = 0; i < static_cast<int>(input.size()); ++i) {
		//:でピースごとに区切る
		auto pieceStr = split(input[i], ':');
		for (int j = 1; j < static_cast<int>(pieceStr.size()); ++j) {
		  // で頂点ごとに区切る
		  auto numStr = split(pieceStr[j], ' ');

		  if (i == input.size() - 1 && j == pieceStr.size() - 1) {
			//最後のQRの最後のデータの場合はframePointに格納する
			for (int k = 1; k < static_cast<int>(numStr.size()); k += 2) {
			  Point pointTemp(stoi(numStr[k])*10, stoi(numStr[k + 1]) * 10);
			  frame.push_back(pointTemp);
			}
		  }
		  else {
			//角頂点の座標をpieceTemoに数字に変換して格納して、piecePointに追加する
			std::vector<Point> pieceTemp;
			for (int k = 1; k < static_cast<int>(numStr.size()); k += 2) {
			  Point pointTemp(stoi(numStr[k]) * 10, stoi(numStr[k + 1]) * 10);
			  pieceTemp.push_back(pointTemp);
			}
			piece.push_back(pieceTemp);
		  }

		}
	  }
	}

	if (frame.size() == 0) {
	  data tmp;
	  if ((tmp = checkHit()).x != -1) {
		if (now.size() > 2 && now[0] == plot[tmp.x][tmp.y]) {
		  frame = now;
		  now.clear();
		}
		else {
		  now.push_back(plot[tmp.x][tmp.y]);
		}
	  }
	}
	else {
	  //フレーム描画
	  for (int i = 0; i < static_cast<int>(frame.size()); i++) {
		s3d::Line(frame[i].x + 10, frame[i].y + 10, frame[(i + 1) % frame.size()].x + 10, frame[(i + 1) % frame.size()].y + 10).draw(3,s3d::Color(0, 0, 255));
	  }
	  //ピース描画
	  if (piece.size()) {
		for (int i = 0; i < static_cast<int>(piece.size()); i++) {
		  std::vector<s3d::Vec2> tmp;
		  for (int j = 0; j < static_cast<int>(piece[i].size()); j++) {
			s3d::Line(piece[i][j].x + 10, piece[i][j].y + 10, piece[i][(j + 1) % piece[i].size()].x + 10, piece[i][(j + 1) % piece[i].size()].y + 10).draw(3,s3d::Color(0, 255, 0));
			tmp.push_back(s3d::Vec2(piece[i][j].x + 10, piece[i][j].y + 10));
		  }
		  s3d::Polygon po(tmp);
		  po.draw(s3d::Color(0, 128, 0));
		  if (po.mouseOver) {
			pos(s3d::ToString(i)).draw(s3d::Mouse::Pos().x + 20, s3d::Mouse::Pos().y - 25, s3d::Color(255, 0, 0));
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


	  
	  //copy
	  if (now.size() == 0 && gui.button(L"copy").pushed) {
		std::string str;
		str += std::to_string(piece.size());

		for (auto i : piece) {

		  str += (":" + std::to_string(i.size()));
		  int minx = 999, miny = 999;
		  for (auto j : i) {
			minx = std::min(minx, j.x);
			miny = std::min(miny, j.y);
		  }

		  for (auto j : i) {
			//寄せる
			str += (" " + std::to_string((j.x - minx) / 10) + " " + std::to_string((j.y - miny) / 10));
		  }
		}

		str += (":"+std::to_string(frame.size()));
		int minx = 999, miny = 999;
		for (auto i : frame) {
		  minx = std::min(minx, i.x);
		  miny = std::min(miny, i.y);
		}
		for (auto i : frame) {
		  str += (" " + std::to_string((i.x - minx) / 10) + " " + std::to_string((i.y - miny) / 10));
		}
		
		s3d::Clipboard::SetText(s3d::CharacterSet::Widen(str));
	  }

	  //copyRaw
	  if (now.size() == 0 && gui.button(L"copyraw").pushed) {
		std::string str;
		str += std::to_string(piece.size());

		for (auto i : piece) {

		  str += (":" + std::to_string(i.size()));

		  for (auto j : i) {
			//寄せる
			str += (" " + std::to_string((j.x) / 10) + " " + std::to_string((j.y) / 10));
		  }
		}

		str += (":" + std::to_string(frame.size()));

		for (auto i : frame) {
		  str += (" " + std::to_string((i.x) / 10) + " " + std::to_string((i.y) / 10));
		}

		s3d::Clipboard::SetText(s3d::CharacterSet::Widen(str));
	  }

	}

	

	for (int i = 0; i < static_cast<int>(circle.size()); ++i) {
	  for (int j = 0; j < static_cast<int>(circle[i].size()); ++j) {
		if (circle[i][j].mouseOver) {
		  pos(L"(" + s3d::ToString((circle[i][j].x - 10) / 10) + L"," + s3d::ToString((circle[i][j].y - 10) / 10) + L")").draw(s3d::Mouse::Pos().x + 10, s3d::Mouse::Pos().y + 15, s3d::Color(0, 255, 255));
		}
	  }
	}

	//現在の描画
	if (now.size()) {
	  for (int i = now.size() - 1; i > 0; i--) {
		s3d::Line(now[i].x + 10, now[i].y + 10, now[i - 1].x + 10, now[i - 1].y + 10).draw(3,s3d::Color(255, 255, 0));
	  }
	  s3d::Line(now[now.size() - 1].x + 10, now[now.size() - 1].y + 10, s3d::Mouse::Pos().x, s3d::Mouse::Pos().y).draw(3,s3d::Color(255, 0, 0));
	}
  }
}
