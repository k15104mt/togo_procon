#include"piece.hpp"
#include"utility.hpp"
#include"color.hpp"

#include<cstdio>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<stack>
#include<array>


std::vector<std::vector<Point>> framePoint;


#include"utility.hpp"
#include"piece.hpp"
#include<vector>
#include<cstdlib>
#define PI 3.1415926535

//std::vector<std::vector<Point>> framePoint;		//フレーム情報は分断されることがある(設置過程においても)

int	ap = 0, ax[256], ay[256], bp = 0, bx[256], by[256];	// 入力図形の記憶用	ap,bp:頂点数
int	rc = 0, rp[256], rx[256][256], ry[256][256];		// 演算後図形の記憶用
int	vn = 0, vx1[256], vy1[256], vx2[256], vy2[256];	// ベクトルの記憶用
int	tcnt = 0, tp, tx[256], ty[256], used[256];		// 一時記憶用

void OnNot();
void OnMerge();
void OnClean();
double CheckAngle(int n, int *x, int *y);
void Reverse(int n, int *x, int *y);

// ptr: 削除位置  last: 配列末尾位置（最後の要素の次）
void erase(int *ptr, int *last) {
  while (++ptr < last)
	*(ptr - 1) = *ptr;       //  要素をひとつ前にずらす
}

//とりあえず頂点が一番左上の部分に設置するとする
Point getPutPoint(std::vector<Piece> &data, std::vector<putData> &already_put) {
  if (already_put.size() == 0) {
	return framePoint[0][0];
  }

  Point point;	//返り値

				////ピースと設置情報より，未設置部の図形頂点を求める
  for (int i = 0; i < already_put.size(); i++) {
	//printf("\n--NOT処理[%d]--\n", i);	//debug
	std::vector<std::vector<Point>> putPiece;	//フレームにNOT処理するピース情報

	bp = framePoint[0].size() + 1;										//NOT入力図形頂点数(フレーム)
	ap = data[already_put[i].piece_num].getPoint()[0].size() + 1;	//NOT入力図形頂点数(ピース)
																	//printf("bp:%d,ap:%d\n", bp, ap);		//debug

	for (int j = 0; j < bp - 1; j++) {	//NOT処理で使う変数格納
	  bx[j] = framePoint[0][j].x;	//※要修正（フレーム情報の一つしか使ってない）
	  by[j] = framePoint[0][j].y;
	  //printf("b[%d](%d,%d)\n", j, bx[j], by[j]);	//debug
	}
	bx[bp - 1] = bx[0];
	by[bp - 1] = by[0];


	for (int j = 0; j < ap - 1; j++) {	//NoT処理で使う変数格納
	  ax[j] = data[already_put[i].piece_num].getPoint()[already_put[i].point_num][j].x + already_put[i].base_point.x;	//きもいけど設置ピース取得してる
	  ay[j] = data[already_put[i].piece_num].getPoint()[already_put[i].point_num][j].y + already_put[i].base_point.y;
	  //printf("a[%d](%d,%d)\n", j, ax[j], ay[j]);	//debug
	}
	ax[ap - 1] = ax[0];
	ay[ap - 1] = ay[0];

	OnNot();	//not処理を行う
				/*
				////debug
				printf("debug>rc:%d\n", rc);
				for (int i = 0; i < rc; ++i) {
				printf("--i:%d--\n", i);
				for (int j = 0; j < rp[i]; ++j) {
				printf("(%d,%d)\n", rx[i][j], ry[i][j]);
				}
				}*/

				////NOTで出した設置ピース情報とフレームを結合する
	framePoint.clear();
	
	for (int i = 0; i < rc; i++) {
	  std::vector<Point> Tmp;
	  for (int j = 0; j < rp[i]; j++) {
		Point tmp;
		tmp.x = rx[i][j]; tmp.y = ry[i][j];
		Tmp.push_back(tmp);
	  }
	  framePoint.push_back(Tmp);
	}
  }

  printf("rc=%d\n", rc);

  for (int i = 0; i < rc; i++) {
	printf("area[i]=");
	for (int j = 0; j < rp[i]; j++) {
	  printf("{%d,%d},", rx[i][j], ry[i][j]);
	}
	printf("\n");
  }

  //出した未設置部頂点のうち，最も左上の座標を取得する
  for (int i = 0; i < rc; ++i) {
	int b;			//直線方程式 y=-x+b のb
	for (int j = 0; j < rp[i]; ++j) {
	  if (i == j && j == 0) {	//暫定のb最小値
		b = rx[i][j] + ry[i][j];
		point.x = rx[i][j]; point.y = ry[i][j];
	  }
	  else if (b > rx[i][j] + ry[i][j]) {	//最小
		b = rx[i][j] + ry[i][j];
		point.x = rx[i][j]; point.y = ry[i][j];
	  }
	  else if (b == rx[i][j] + ry[i][j] && point.x < rx[i][j]) {	//bの値が同じならばxが小さい方を優先
		b = rx[i][j] + ry[i][j];
		point.x = rx[i][j]; point.y = ry[i][j];
	  }
	}
  }

  return point;
}

// 頂点の曲がり角度の合計を求める
double CheckAngle(int n, int *x, int *y) {
  int	i;
  double	a1, a2, ang, total;

  total = 0;
  x[n] = x[1];
  y[n] = y[1];
  for (i = 1; i < n; ++i)
  {
	a1 = atan2(y[i] - y[i - 1], x[i] - x[i - 1]);
	a2 = atan2(y[i + 1] - y[i], x[i + 1] - x[i]);
	ang = a2 - a1;
	while (ang > PI) ang -= (2 * PI);
	while (ang < -PI) ang += (2 * PI);
	total += ang;
  }
  return total;
}

// 頂点の並び順を反転する
void Reverse(int n, int *x, int *y) {
  int	i, tmpx, tmpy;
  for (i = 0; i < n / 2; ++i) {
	tmpx = x[i];
	tmpy = y[i];
	x[i] = x[n - i - 1];
	y[i] = y[n - i - 1];
	x[n - i - 1] = tmpx;
	y[n - i - 1] = tmpy;
  }
}

// 図形の辺をベクトルに分解
void AddVector(int n, int *x, int *y) {
  for (int i = 1; i < n; ++i) {
	if (y[i - 1] != y[i]) {
	  // 水平ではないベクトルだけを登録
	  // 座標を10倍にして登録(((しなくする
	  vx1[vn] = x[i - 1];
	  vy1[vn] = y[i - 1];
	  vx2[vn] = x[i];
	  vy2[vn] = y[i];
	  ++vn;
	}
  }
  //printf("debug>addV vn=%d\n",vn);
}

// 向きが逆でぴったり重なっているベクトルペアを消す
void DeleteVectorPair() {
  for (int i = 0; i < vn - 1; ++i) {
	for (int j = i + 1; j < vn; ++j) {
	  if ((vx1[i] == vx2[j]) && (vy1[i] == vy2[j]) && (vx2[i] == vx1[j]) && (vy2[i] == vy1[j])) {
		//printf("debug> Vector deleted.\n");
		// ベクトルjを消す
		--vn;
		if (j != vn) {
		  // 最後のベクトルでjを上書き
		  vx1[j] = vx1[vn];
		  vy1[j] = vy1[vn];
		  vx2[j] = vx2[vn];
		  vy2[j] = vy2[vn];
		}
		// ベクトルiを消す
		--vn;
		if (i != vn) {
		  // 最後のベクトルでiを上書き
		  vx1[i] = vx1[vn];
		  vy1[i] = vy1[vn];
		  vx2[i] = vx2[vn];
		  vy2[i] = vy2[vn];
		}
		// 変更したので、ベクトルi から再調査。
		--i;
		break;
	  }
	}
  }
}

// ベクトルを交点および頂点のｙ座標で分割
void CutVector() {
  int	i, j, k, cutn, cuty[256], cx, cy;
  double	a, b, c, d, e, f, y;
  // 向きが逆でぴったり重なっているベクトルペアを消す
  DeleteVectorPair();
  //printf("debug>cut vn=%d\n",vn);
  // ベクトルの交点のy座標でベクトルを分断
  for (i = 0; i < vn - 1; ++i) {
	a = vy2[i] - vy1[i];
	b = vx1[i] - vx2[i];
	c = vx2[i] * vy1[i] - vy2[i] * vx1[i];
	for (j = i + 1; j < vn; ++j) {
	  d = vy2[j] - vy1[j];
	  e = vx1[j] - vx2[j];
	  f = vx2[j] * vy1[j] - vy2[j] * vx1[j];
	  if (b*d - a*e != 0) {
		y = (a*f - c*d) / (b*d - a*e);
		if ((y < 10 * 10) || (y > 310 * 10)) continue;
		cy = (int)(y + 0.5);
		if ((((cy > vy1[i]) && (cy < vy2[i])) || ((cy < vy1[i]) && (cy > vy2[i]))) &&
		  (((cy > vy1[j]) && (cy < vy2[j])) || ((cy < vy1[j]) && (cy > vy2[j])))) {
		  // ベクトルiとjは交差している。
		  // 両ベクトルを交点で分割する。
		  // ベクトルi,jを変更し、分割して増えたベクトルを末尾に追加。
		  cx = (int)((c*e - b*f) / (b*d - a*e) + 0.5);
		  vx2[vn] = vx2[i];
		  vy2[vn] = vy2[i];
		  vx2[i] = vx1[vn] = cx;
		  vy2[i] = vy1[vn] = cy;
		  ++vn;
		  vx2[vn] = vx2[j];
		  vy2[vn] = vy2[j];
		  vx2[j] = vx1[vn] = cx;
		  vy2[j] = vy1[vn] = cy;
		  ++vn;
		  if (vn > 253) {
			printf("ベクトル数の最大を超えてしまいました\n");
			vn = 0;
			return;
		  }
		  // 変更したので、ベクトルi から再調査。
		  --i;
		  break;
		}
	  }
	}
  }

  //// ベクトルの頂点のy座標でベクトルを分断する。
  // 始点のy座標を調べる。
  cutn = 0;
  for (i = 0; i < vn; ++i) {
	for (j = 0; j < cutn; ++j) {
	  if (vy1[i] == cuty[j]) break;
	}
	if (j == cutn) {
	  // 未登録なら登録。
	  cuty[cutn] = vy1[i];
	  ++cutn;
	}
  }
  // 調べたy座標でベクトルを分断
  for (i = 0; i < cutn; ++i) {
	k = vn;
	for (j = 0; j < k; ++j) {
	  if (((vy1[j] < cuty[i]) && (vy2[j] > cuty[i])) || ((vy1[j] > cuty[i]) && (vy2[j] < cuty[i]))) {
		vx2[vn] = vx2[j];
		vy2[vn] = vy2[j];
		vx2[j] = vx1[vn] = (int)(0.5 + vx1[j] + (vx2[j] - vx1[j])*(cuty[i] - vy1[j]) / (vy2[j] - vy1[j]));
		vy2[j] = vy1[vn] = cuty[i];
		++vn;
		//printf("debug>vn=%d\n",vn);
		if (vn > 254) {
		  printf("debug>vnが最大\n");
		  vn = 0;
		  return;
		}
	  }
	}
  }
  // 向きが逆でぴったり重なっているベクトルペアを消す
  DeleteVectorPair();
  //printf("debug>cutn=%d\n",cutn);

}

// ２つのベクトルを比較する。
int CompareVector(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
  // ｙ座標が大きいもの、ｙ座標が同じならｘ座標が小さいものが上位。
  // Ａ＜Ｂ　なら－１を返す。
  // Ａ＝Ｂ　なら０を返す。
  // Ａ＞Ｂ　なら１を返す。
  // 事前の処理により２つのベクトルは交差していないはず。
  // また、ｙ座標の範囲はぴったり同じか、全く重ならないかのどちらかになっているはず。

  if (ay1 + ay2 < by1 + by2) return -1;
  if (ay1 + ay2 > by1 + by2) return 1;
  if (ax1 + ax2 < bx1 + bx2) return 1;
  if (ax1 + ax2 > bx1 + bx2) return -1;
  return 0;
}

// ベクトルをソートする。yが大きいものを優先。yが同じならxの小さいものを優先。
void SortVector() {
  // ヒープソートを使用。
  int	i, j, k, n;
  int	x1, y1, x2, y2;

  n = vn;
  for (k = n / 2; k >= 1; k--)
  {
	i = k;
	x1 = vx1[i - 1];
	y1 = vy1[i - 1];
	x2 = vx2[i - 1];
	y2 = vy2[i - 1];
	while ((j = 2 * i) <= n)
	{
	  if ((j < n) && (CompareVector(vx1[j - 1], vy1[j - 1], vx2[j - 1], vy2[j - 1], vx1[j], vy1[j], vx2[j], vy2[j]) > 0)) j++;
	  if (CompareVector(x1, y1, x2, y2, vx1[j - 1], vy1[j - 1], vx2[j - 1], vy2[j - 1]) <= 0) break;
	  vx1[i - 1] = vx1[j - 1];
	  vy1[i - 1] = vy1[j - 1];
	  vx2[i - 1] = vx2[j - 1];
	  vy2[i - 1] = vy2[j - 1];
	  i = j;
	}
	vx1[i - 1] = x1;
	vy1[i - 1] = y1;
	vx2[i - 1] = x2;
	vy2[i - 1] = y2;
  }
  while (n > 1)
  {
	x1 = vx1[n - 1];
	y1 = vy1[n - 1];
	x2 = vx2[n - 1];
	y2 = vy2[n - 1];
	vx1[n - 1] = vx1[0];
	vy1[n - 1] = vy1[0];
	vx2[n - 1] = vx2[0];
	vy2[n - 1] = vy2[0];
	n--;
	i = 1;
	while ((j = 2 * i) <= n)
	{
	  if ((j < n) && (CompareVector(vx1[j - 1], vy1[j - 1], vx2[j - 1], vy2[j - 1], vx1[j], vy1[j], vx2[j], vy2[j]) > 0)) j++;
	  if (CompareVector(x1, y1, x2, y2, vx1[j - 1], vy1[j - 1], vx2[j - 1], vy2[j - 1]) <= 0) break;
	  vx1[i - 1] = vx1[j - 1];
	  vy1[i - 1] = vy1[j - 1];
	  vx2[i - 1] = vx2[j - 1];
	  vy2[i - 1] = vy2[j - 1];
	  i = j;
	}
	vx1[i - 1] = x1;
	vy1[i - 1] = y1;
	vx2[i - 1] = x2;
	vy2[i - 1] = y2;
  }
}

// ＮＯＴ処理
void OnNot() {
  //double	ang;
  int	i, j, yy;

  //// 図形Ａは右回り、図形Ｂは左回りにする。

  // 図形Ａについて
  //ang = CheckAngle(ap,ax,ay);
  //if ((ang > 2*PI-0.1) && (ang < 2*PI+0.1))
  //Reverse(ap,ax,ay);
  //else if (!((ang > -2*PI-0.1) && (ang < -2*PI+0.1)))
  //	("図形Ａはねじれています。正常に処理できません。");

  // 図形Ｂについて
  //ang = CheckAngle(bp,bx,by);
  //if ((ang > -2*PI-0.1) && (ang < -2*PI+0.1))
  Reverse(bp, bx, by);
  //else if (!((ang > 2*PI-0.1) && (ang < 2*PI+0.1)))
  //("図形Ｂはねじれています。正常に処理できません。");

  // ベクトルに分解
  vn = 0;
  AddVector(ap, ax, ay);
  AddVector(bp, bx, by);
  CutVector();

  // ベクトルをソート
  SortVector();

  // 演算結果を抽出
  rc = 0;
  rp[rc] = 0;
  yy = 0;
  for (i = 0; i < vn; ++i) {
	if (vy1[i] + vy2[i] != yy) {
	  yy = vy1[i] + vy2[i];
	  j = 0;
	}
	if (vy1[i] < vy2[i])
	  ++j;
	else
	  --j;

	if ((rp[rc] == 0) && (j == 1)) {
	  rp[rc] = 5;
	  rx[rc][0] = rx[rc][4] = vx1[i];
	  ry[rc][0] = ry[rc][4] = vy1[i];
	  rx[rc][1] = vx2[i];
	  ry[rc][1] = vy2[i];
	}
	else if ((rp[rc] > 0) && (j == 0)) {
	  rx[rc][2] = vx1[i];
	  ry[rc][2] = vy1[i];
	  rx[rc][3] = vx2[i];
	  ry[rc][3] = vy2[i];
	  ++rc;
	  rp[rc] = 0;
	}
  }
  tcnt = 0;
  OnMerge();
  printf("rp[rc]:%d\n", rp[rc]);

  for (int i = 0; i < rc; ++i) {
	for (int j = 0; j < rp[i]; ++j) {
	if (i != 0 && rx[i][j] == rx[i][j] && ry[i][j] == ry[i][j-1]) {
	printf("debug>(%d,%d)と(%d,%d)削除\n",rx[i][j], ry[i][j], rx[i][j - 1], ry[i][j - 1]);
	erase(std::begin(rx[i]) + i , std::end(rx[i]) );
	rp[i]--;
	}
	}
	Reverse(rp[i], rx[i], ry[i]);
  }
  //OnClean();
}

// 抽出図形ｎの外形線(x1,y)-(x2,y)に接する図形を調査する。
void MergeObject(int n, int x1, int x2, int y)
{
  int	i;

  if (x1 < x2)
  {
	for (i = 0; i < rc; ++i)
	{
	  // マージ済みなら次へ
	  if (used[i] > 0) continue;

	  // 下辺のｙ座標が違っているか、下に尖った三角形なら次へ
	  if ((ry[i][4] != y) || (rx[i][3] == rx[i][4])) continue;

	  // 下辺のｘ範囲が外れていたら次へ
	  if ((rx[i][3] <= x1) || (rx[i][4] >= x2)) continue;

	  // ここまで来れたら辺で接している。
	  ++used[i];

	  // 左側にはみ出している場合、はみ出した部分を調査。
	  if (rx[i][4] < x1) MergeObject(i, x1, rx[i][4], y);

	  // 左下頂点が第１座標ではない場合、マージ後図形に頂点を追加。
	  if (rx[i][4] != x1)
	  {
		tx[tp] = rx[i][4];
		ty[tp] = y;
		++tp;
	  }

	  // 左上頂点をマージ後図形に追加。
	  tx[tp] = rx[i][1];
	  ty[tp] = ry[i][1];
	  ++tp;

	  // 上に尖った三角形でない場合、図形の上辺を調査し、マージ後図形に右上頂点を追加。
	  if (rx[i][1] != rx[i][2])
	  {
		MergeObject(i, rx[i][1], rx[i][2], ry[i][1]);
		tx[tp] = rx[i][2];
		ty[tp] = ry[i][2];
		++tp;
	  }

	  // 右下頂点が第２座標ではない場合、マージ後図形に頂点を追加。
	  if (rx[i][3] != x2)
	  {
		tx[tp] = rx[i][3];
		ty[tp] = y;
		++tp;
	  }

	  // 図形が右側にはみ出している場合、はみ出した部分を調査。
	  if (rx[i][3] > x2) MergeObject(i, rx[i][3], x2, y);
	}
  }
  else
  {
	for (i = rc - 1; i >= 0; --i)
	{
	  // マージ済みなら次へ
	  if (used[i] > 0) continue;

	  // 上辺のｙ座標が違っているか、上に尖った三角形なら次へ
	  if ((ry[i][1] != y) || (rx[i][1] == rx[i][2])) continue;

	  // 上辺のｘ範囲が外れていたら次へ
	  if ((rx[i][2] <= x2) || (rx[i][1] >= x1)) continue;

	  // ここまで来れたら辺で接している。
	  ++used[i];

	  // 右側にはみ出している場合、はみ出した部分を調査。
	  if (rx[i][2] > x1) MergeObject(i, x1, rx[i][2], y);

	  // 右上頂点が第１座標ではない場合、マージ後図形に頂点を追加。
	  if (rx[i][2] != x1)
	  {
		tx[tp] = rx[i][2];
		ty[tp] = y;
		++tp;
	  }

	  // 右下頂点をマージ後図形に追加。
	  tx[tp] = rx[i][3];
	  ty[tp] = ry[i][3];
	  ++tp;

	  // 下に尖った三角形でない場合、図形の下辺を調査し、マージ後図形に左下頂点を追加。
	  if (rx[i][3] != rx[i][4])
	  {
		MergeObject(i, rx[i][3], rx[i][4], ry[i][3]);
		tx[tp] = rx[i][0];
		ty[tp] = ry[i][0];
		++tp;
	  }

	  // 左上頂点が第２座標ではない場合、マージ後図形に頂点を追加。
	  if (rx[i][1] != x2)
	  {
		tx[tp] = rx[i][1];
		ty[tp] = y;
		++tp;
	  }

	  // 図形が左側にはみ出している場合、はみ出した部分を調査。
	  if (rx[i][1] < x2) MergeObject(i, rx[i][1], x2, y);
	}
  }
}

// マージ処理
void OnMerge() {
  int	i, j;

  // 実行済みか、図形が無ければ何もしない。
  if ((tcnt > 0) || (rc == 0)) return;

  // マージ済みフラグを初期化
  for (i = 0; i < rc; ++i) used[i] = 0;

  for (i = 0; i < rc; ++i)
  {
	if (used[i] > 0)
	  continue; // マージ済みなら次へ
	else
	  ++used[i]; // そうでなければマージ済みフラグを立てる。

				 // 右上と右下の座標をマージ後図形に追加。
	tp = 0;
	tx[tp] = rx[i][2];
	ty[tp] = ry[i][2];
	++tp;
	tx[tp] = rx[i][3];
	ty[tp] = ry[i][3];
	++tp;

	// 図形が下に尖った三角形で無い場合、下辺を調査し、左下頂点をマージ後図形に追加。
	if (rx[i][3] != rx[i][4])
	{
	  MergeObject(i, rx[i][3], rx[i][4], ry[i][3]);
	  tx[tp] = rx[i][4];
	  ty[tp] = ry[i][4];
	  ++tp;
	}

	// 左上の座標をマージ後図形に追加。
	tx[tp] = rx[i][1];
	ty[tp] = ry[i][1];
	++tp;

	// 図形が上に尖った三角形で無い場合、上辺を調査し、右上頂点をマージ後図形に追加。
	if (rx[i][1] != rx[i][2])
	{
	  MergeObject(i, rx[i][1], rx[i][2], ry[i][1]);
	  tx[tp] = rx[i][2];
	  ty[tp] = ry[i][2];
	  ++tp;
	}

	// マージした結果を抽出図形に上書きコピーする。
	// 上書きされる図形はマージ済みなので問題ない。
	if (used[tcnt] == 0) printf("マージ済みでない図形が上書きされています！");
	for (j = 0; j < tp; ++j)
	{
	  rx[tcnt][j] = tx[j];
	  ry[tcnt][j] = ty[j];
	}
	rp[tcnt] = tp;
	++tcnt;
  }
  rc = tcnt;

}

// 冗長点削除の処理
void OnClean()
{
  int	i, j;
  double	a, b, c, len;

  if (tcnt == 0) return; // マージ前なら戻る。

  for (i = 0; i < rc; ++i)
  {
	tx[0] = rx[i][0];
	ty[0] = ry[i][0];
	tp = 1;
	for (j = 1; j < rp[i] - 1; ++j)
	{
	  // 直線(tx[tp-1],ty[tp-1])-(rx[i][j+1],ry[i][j+1])に対して
	  // 点(rx[i][j],ry[i][j])から垂線を下ろした時、
	  // その長さが単位長未満なら冗長点とみなす。

	  // 直線の方程式 ax+by+c=0，垂線の長さ len
	  a = ry[i][j + 1] - ty[tp - 1];
	  b = tx[tp - 1] - rx[i][j + 1];
	  c = rx[i][j + 1] * ty[tp - 1] - ry[i][j + 1] * tx[tp - 1];
	  len = (a * rx[i][j] + b * ry[i][j] + c) / sqrt(a * a + b * b);
	  if ((len >= 5) || (len <= -5))
	  {
		// 垂線が長いので冗長点ではない。
		tx[tp] = rx[i][j];
		ty[tp] = ry[i][j];
		++tp;
	  }
	}
	// 終始点一致させる
	tx[tp] = rx[i][0];
	ty[tp] = ry[i][0];
	++tp;

	// 冗長点削除後の座標でリストを上書き
	for (j = 0; j < tp; ++j)
	{
	  rx[i][j] = tx[j];
	  ry[i][j] = ty[j];
	}
	rp[i] = tp;
  }

}

//当たり判定
int checkHit(std::vector<Piece> &, std::vector<putData> &, putData &);

//Point getPutPoint(std::vector<Piece> &, std::vector<putData> &);

//再帰
int solve(std::vector<Piece> &, std::vector<putData> &);


int main() {
  //QRコードの数
  int qrNum;
  std::cin >> qrNum;
  std::cin.ignore();

  //QRコードの数だけ入力を繰り返す
  std::vector<std::string> input;
  for (int i = 0; i < qrNum; ++i) {
	std::string temp;
	std::getline(std::cin, temp);
	input.push_back(temp);
  }


  std::vector<std::vector<Point>> piecePoint;
  framePoint.resize(1);

  for (int i = 0; i < static_cast<int>(input.size()); ++i) {
	//QRコードi番目の文字列

	//:でピースごとに区切る
	auto pieceStr = split(input[i], ':');
	for (int j = 1; j < static_cast<int>(pieceStr.size()); ++j) {
	  // で頂点ごとに区切る
	  auto numStr = split(pieceStr[j], ' ');

	  if (i == input.size() - 1 && j == pieceStr.size() - 1) {
		//最後のQRの最後のデータの場合はframePointに格納する
		for (int k = 1; k < static_cast<int>(numStr.size()); k += 2) {
		  Point pointTemp(stoi(numStr[k]), stoi(numStr[k + 1]));
		  framePoint[0].push_back(pointTemp);
		}
	  }
	  else {
		//角頂点の座標をpieceTemoに数字に変換して格納して、piecePointに追加する
		std::vector<Point> pieceTemp;
		for (int k = 1; k < static_cast<int>(numStr.size()); k += 2) {
		  Point pointTemp(stoi(numStr[k]), stoi(numStr[k + 1]));
		  pieceTemp.push_back(pointTemp);
		}
		piecePoint.push_back(pieceTemp);
	  }

	}
  }

  std::vector<Piece> data;

  for (int i = 0; i < static_cast<int>(piecePoint.size()); i++) {
	data.push_back(piecePoint[i]);
  }

  //------------------------------------------------------
  setColor(F_BLACK, B_WHITE|B_INTENSITY);

  for (int i = 0; i < static_cast<int>(piecePoint.size()); ++i) {
	std::cout << "piece[" << i << "]" << "(頂点数:" << piecePoint[i].size() << "):";
	for (auto j : piecePoint[i]) {
	  j.print();
	}
	std::cout << std::endl;
  }


  std::cout << "frame（頂点数:" << framePoint[0].size() << "):";
  for (auto i : framePoint[0]) {
	i.print();
  }
  std::cout << std::endl;

  setColor();
//------------------------------------------------------
  int p = 0;
  for (auto i : data) {
	printf("%dピース\n",p);
	int n = 0;
	for (auto j : i.getPoint()) {
	  printf("	%d回転\n	  ", n);
	  for (auto k : j) {
		printf("(%d,%d) ", k.x, k.y);
	  }
	  printf("\n");
	  n++;
	}
	p++;
  }

  std::vector<putData> already_put;
  solve(data, already_put);

  //回答表示
  for (auto i : already_put) {
	printf("%d %d (%d,%d)\n", i.piece_num, i.point_num,i.base_point.x,i.base_point.y);
  }

  //ファイル出力
  FILE *fp;
  fopen_s(&fp,"out.txt","w");
  if (fp != NULL) {
	for (auto i : already_put) {
	  fprintf_s(fp, "%d %d (%d,%d)\n", i.piece_num, i.point_num, i.base_point.x, i.base_point.y);
	}
  }

  getchar();
  getchar();

  return 0;
}


int checkHit(std::vector<Piece> &data, std::vector<putData> &already_put, putData &put) {
  std::vector<Point> cp1(data[put.piece_num].getPoint()[put.point_num]);
  //移動
  move(cp1, put.base_point);

  for (int i = 0; i < static_cast<int>(already_put.size()); ++i) {
	std::vector<Point> cp2(data[already_put[i].piece_num].getPoint()[already_put[i].point_num]);
	//移動
	move(cp2, already_put[i].base_point);

	if (collisionPiece(cp1, cp2)) {
	  printf("piece");
	  return 1;
	}
  }

  int flag = 0;
  for (int j = 0; j < static_cast<int>(framePoint.size()); j++) {
	if (!collisionFrame(framePoint[j], cp1)) {
	  flag = 1;
	}
  }


  if (!flag) {
	printf("frame");
	return 1;
  }
  
  

  return 0;
}

/*
Point getPutPoint(std::vector<Piece> &data, std::vector<putData> &already_put) {
  Point tmp;
  scanf_s("%d %d", &tmp.x, &tmp.y);
  tmp.print();
  printf_s("に設置\n");
  return tmp;
}
*/

int solve(std::vector<Piece> &data, std::vector<putData> &already_put) {
  if (data.size() == already_put.size()) {
	//全部置いたってこと
	return 1;
  }

  //全ピース見ていこうな
  for (int i = 0; i < static_cast<int>(data.size()); ++i) {//ピースの数
														   //今のピースがすでに置かれているかどうか
	if ([=]() {for (int j = 0; j < static_cast<int>(already_put.size()); ++j) { if (already_put[j].piece_num == i) { return 0; } }return 1; }()) {
	  for (int j = 0; j < static_cast<int>(data[i].getPoint().size()); ++j) {//回転の組み合わせの数
		for (int k = 0; k < static_cast<int>(data[i].getPoint()[j].size()); ++k) {//設置頂点

		  printf("今置きたいやつ(%d,%d,%d)\n", i,j,k);
		  Point tmp = getPutPoint(data, already_put);
		  printf("置きたい点"); tmp.println();
		  printf("framePoint.size() %d\n", framePoint.size());
		  putData put(i, j, k, Point(tmp.x-data[i].getPoint()[j][k].x,tmp.y-data[i].getPoint()[j][k].y));
		  if (!checkHit(data, already_put, put)) {
			//もし当たり判定がokなら
			setColor(F_CYAN | F_INTENSITY);
			printf("put\n");
			setColor();
			already_put.push_back(put);
			if (solve(data, already_put)) {
			  //もしreturn 1なら解き終わったってこと
			  return 1;
			}
		  }
		  else {
			setColor(F_RED | F_INTENSITY);
			printf_s("Hit!!!!\n");
			setColor();
		  }
		}
	  }
	}
  }

  //ここまで来たってことはダメだったってことだからpopしてバック
  if(already_put.size())already_put.pop_back();
  return 0;
}
