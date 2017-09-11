#include"geometry.hpp"
#include"color.hpp"
#include<cstdlib>
#include<math.h>
#define PI 3.1415926535

void OnNot(int &ap,int &bp, std::vector<Point> &a, std::vector<Point> &b,int &rc,int *rp, std::vector<std::vector<D_Point>> &r);
void OnMerge(int &tcnt, int &tp, double *tx, double *ty, double *used, std::vector<std::vector<D_Point>> &r, int &rc, int *rp);
void OnClean(int &tcnt, int &tp, double *tx, double *ty, double *used, std::vector<std::vector<D_Point>> &r, int &rc, int *rp);
int CheckAngle(std::vector<Point> point);
void Reverse(int n,std::vector<Point> &point);
Point getUpperLeft(std::vector<std::vector<Point>> &areaPoint);

Geometry::Geometry(std::vector<std::vector<Point>> framePoint) {
	areaPoint = framePoint;
	putNum = 0;
}

//とりあえず頂点が一番左上の部分に設置するとする
Point Geometry::getPutPoint(std::vector<Piece> &data, std::vector<putData> &already_put) {
	Point point;	//返り値
	Point tmp;
	int	ap=0, bp=0;	// 入力図形の記憶用	ap,bp:頂点数
	std::vector<Point> a;
	std::vector<Point> b;

	int	rc=0, rp[MAX];		// 演算後図形の記憶用
	std::vector<std::vector<D_Point>> r;

	r.resize(MAX);
	for (int i = 0; i < MAX; i++) {
		r[i].resize(MAX);
	}

	//setColor(F_CYAN|F_INTENSITY, B_BLACK); printf("設置ピース数:%d\n分割エリア数:%d\n", already_put.size(), areaPoint.size()); 
	//printf("<初期フレーム座標>\n"); for (int i = 0; i < (int)areaPoint.size(); i++) { printf("area[%d]:", i); for (int j = 0; j < (int)areaPoint[i].size(); j++) { printf("{%d,%d},", areaPoint[i][j].x, areaPoint[i][j].y); }printf("\n"); }setColor();

	if (already_put.size() == 0) {
		point = getUpperLeft(areaPoint);
		return point;
	}

	////ピースと設置情報より，未設置部の図形頂点を求める
	for (int i = 0; i < (int)already_put.size(); i++) {		//設置ピース毎
		//setColor(F_BLUE|F_INTENSITY, B_BLACK);	printf("\n----[%d]番目の設置ピースでNOT処理----\n", i);	setColor();
		for (int k = 0; k < (int)areaPoint.size(); k++) {	//分割エリア毎
			a.clear();	//更新
			b.clear();

			//printf("--確認分割エリア[%d]--\n", k);

			bp = areaPoint[k].size() + 1;									//NOT入力図形頂点数(フレーム)
			ap = data[already_put[i].piece_num].getPoint()[0].size() + 1;	//NOT入力図形頂点数(ピース)

			for (int j = 0; j < bp - 1; j++) {	//NOT処理で使う変数格納
				tmp = areaPoint[k][j];
				b.push_back(tmp);
				//setColor(F_ORANGE); printf("b[%d](%d,%d)\n", j, b[j].x, b[j].y); setColor();	//debug
			}
			tmp= b[0];
			b.push_back(tmp);
			//puts("--");	//debug

			for (int j = 0; j < ap - 1; j++) {	//NoT処理で使う変数格納
				tmp.x = data[already_put[i].piece_num].getPoint()[already_put[i].point_num][j].x + already_put[i].base_point.x;	//きもいけど設置ピース取得してる
				tmp.y = data[already_put[i].piece_num].getPoint()[already_put[i].point_num][j].y + already_put[i].base_point.y;
				a.push_back(tmp);
				//setColor(F_ORANGE); printf("a[%d](%d,%d)\n", j, a[j].x, a[j].y); setColor();	//debug
			}
			tmp = a[0];	//一周
			a.push_back(tmp);
			OnNot(ap,bp,a,b,rc,rp,r);

			areaPoint.erase(areaPoint.begin()+k);	//一旦見てるエリア削除※削減あり
			////NOTで出した設置ピース情報とフレームを結合する
			//未設置エリアの更新
			for (int i = 0; i < rc; i++) {
				std::vector<Point> Tmp;
				for (int j = 0; j < rp[i]; j++) {
					tmp.x =static_cast<int>(r[i][j].x+0.5);
					tmp.y = static_cast<int>(r[i][j].y + 0.5);
					Tmp.push_back(tmp);
				}
				areaPoint.insert(areaPoint.begin()+k,Tmp);
			}
			point = getUpperLeft(areaPoint);

			//ここまでいくと更新
		}
	}
	return point;
}

// ptr: 削除位置  last: 配列末尾位置（最後の要素の次）
void erase(int *ptr, int *last) {
	while (++ptr < last)
		*(ptr - 1) = *ptr;       //  要素をひとつ前にずらす
}
// 頂点の曲がり角度の合計を求め，時計回りなら1を，反時計回りなら-1を，ねじれているなら0を返す
int CheckAngle(std::vector<Point> point) {
	int	i,n=point.size();
	double	a1, a2, ang, total;

	total = 0;
	point[n].x = point[1].x;
	point[n].y = point[1].y;
	for (i = 1; i < n; ++i)
	{
		a1 = atan2(point[i].y - point[i - 1].y, point[i].x - point[i - 1].x);
		a2 = atan2(point[i + 1].y - point[i].y, point[i + 1].x - point[i].x);
		ang = a2 - a1;
		while (ang > PI) ang -= (2 * PI);
		while (ang < -PI) ang += (2 * PI);
		total += ang;
	}
	if ((total > 2 * PI - 0.1) && (total < 2 * PI + 0.1))return -1;			//反時計回り
	else if (!((ang > -2 * PI - 0.1) && (ang < -2 * PI + 0.1)))return 0;	//ねじれている図形
	
	return 1;	//正常な時計回り図形
}

// 頂点の並び順を反転する
void Reverse(int n,std::vector<Point> &point) {
	Point tmp;
	int	i;
	for (i = 0; i < n / 2; ++i) {
		tmp = point[i];
		point[i] = point[n - i - 1];
		point[n - i - 1] = tmp;
	}
}

// 頂点の並び順を反転する
void Reverse(int n, std::vector<D_Point> &point) {
	D_Point tmp;
	int	i;
	for (i = 0; i < n / 2; ++i) {
		tmp = point[i];
		point[i] = point[n - i - 1];
		point[n - i - 1] = tmp;
	}
}

// 図形の辺をベクトルに分解
void AddVector(int n, std::vector<Point> &point, double *vx1,double *vy1,double *vx2,double *vy2,int &vn) {
	for (int i = 1; i < n; ++i) {
		if (point[i - 1].y != point[i].y) {
			// 水平ではないベクトルだけを登録
			// 座標を10倍にして登録(((※しなくしました
			vx1[vn] = point[i - 1].x;
			vy1[vn] = point[i - 1].y;
			vx2[vn] = point[i].x;
			vy2[vn] = point[i].y;
			++vn;
		}
	}
	//printf("debug>addV vn=%d\n",vn);
}

// 向きが逆でぴったり重なっているベクトルペアを消す
void DeleteVectorPair(double *vx1, double *vy1, double *vx2, double *vy2, int &vn) {
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
void CutVector(double *vx1, double *vy1, double *vx2, double *vy2, int &vn) {
	int	i, j, k, cutn;
	double	a, b, c, d, e, f, y,cuty[MAX], cx, cy;
	// 向きが逆でぴったり重なっているベクトルペアを消す
	DeleteVectorPair(vx1, vy1, vx2, vy2, vn);
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
				//cy = (int)(y + 0.5);
				cy = y;
				//printf("cy:%lf,",y);
				if ((((cy > vy1[i]) && (cy < vy2[i])) || ((cy < vy1[i]) && (cy > vy2[i]))) &&
					(((cy > vy1[j]) && (cy < vy2[j])) || ((cy < vy1[j]) && (cy > vy2[j])))) {
					// ベクトルiとjは交差している。
					// 両ベクトルを交点で分割する。
					// ベクトルi,jを変更し、分割して増えたベクトルを末尾に追加。
					//cx = (int)((c*e - b*f) / (b*d - a*e));
					cx = (c*e - b*f) / (b*d - a*e);
					//printf("cx:%lf", (c*e - b*f) / (b*d - a*e));
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
					if (vn > MAX-3) {
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
				//vx2[j] = vx1[vn] = (int)(0.5 + vx1[j] + (vx2[j] - vx1[j])*(cuty[i] - vy1[j]) / (vy2[j] - vy1[j]));
				vx2[j] = vx1[vn] = vx1[j] + (vx2[j] - vx1[j])*(cuty[i] - vy1[j]) / (vy2[j] - vy1[j]);
				vy2[j] = vy1[vn] = cuty[i];
				++vn;
				//printf("debug>vn=%d\n",vn);
				if (vn > MAX-2) {
					printf("debug>vnが最大\n");
					vn = 0;
					return;
				}
			}
		}
	}
	// 向きが逆でぴったり重なっているベクトルペアを消す
	DeleteVectorPair(vx1, vy1, vx2, vy2, vn);
	//printf("debug>cutn=%d\n",cutn);

}

// ２つのベクトルを比較する。
int CompareVector(double ax1, double ay1, double ax2, double ay2, double bx1, double by1, double bx2, double by2) {
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
void SortVector(double *vx1, double *vy1, double *vx2, double *vy2, int &vn) {
	// ヒープソートを使用。
	int	i, j, k, n;
	double	x1, y1, x2, y2;

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
void OnNot(int &ap, int &bp, std::vector<Point> &a, std::vector<Point> &b, int &rc, int *rp, std::vector<std::vector<D_Point>> &r) {
	int	vn = 0;
	double vx1[MAX], vy1[MAX], vx2[MAX], vy2[MAX];	// ベクトルの記憶用
	int	tcnt = 0, tp;
	double tx[MAX], ty[MAX], used[MAX];		// 一時記憶用

	//double	ang;
	int	i, j;
	double yy;

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
	Reverse(b.size(),b);
	//else if (!((ang > 2*PI-0.1) && (ang < 2*PI+0.1)))
		//("図形Ｂはねじれています。正常に処理できません。");

	// ベクトルに分解
	vn = 0;
	AddVector(ap, a, vx1, vy1, vx2,vy2, vn);
	AddVector(bp, b, vx1, vy1, vx2, vy2, vn);
	CutVector(vx1, vy1, vx2, vy2, vn);

	// ベクトルをソート
	SortVector(vx1, vy1, vx2, vy2, vn);

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
			r[rc][0].x = r[rc][4].x = vx1[i];
			r[rc][0].y = r[rc][4].y = vy1[i];
			r[rc][1].x = vx2[i];
			r[rc][1].y = vy2[i];
		}
		else if ((rp[rc] > 0) && (j == 0)) {
			r[rc][2].x = vx1[i];
			r[rc][2].y = vy1[i];
			r[rc][3].x = vx2[i];
			r[rc][3].y = vy2[i];
			++rc;
			rp[rc] = 0;
		}
	}
	tcnt = 0;
	OnMerge(tcnt,tp,tx, ty,used, r, rc, rp);
	//printf("rp[rc]:%d\n", rp[rc]);

	for (int i = 0; i < rc; ++i) {
		Reverse(rp[i], r[i]);
	}
	OnClean(tcnt, tp, tx, ty, used, r, rc, rp);
}

// 抽出図形ｎの外形線(x1,y)-(x2,y)に接する図形を調査する。
void MergeObject(int n, double x1, double x2, double y,int	&tcnt,int &tp,double *tx,double *ty,double *used, std::vector<std::vector<D_Point>> &r,int &rc) {
	int	i;

	if (x1 < x2)
	{
		for (i = 0; i < rc; ++i)
		{
			// マージ済みなら次へ
			if (used[i] > 0) continue;

			// 下辺のｙ座標が違っているか、下に尖った三角形なら次へ
			if ((r[i][4].y != y) || (r[i][3].x == r[i][4].x)) continue;

			// 下辺のｘ範囲が外れていたら次へ
			if ((r[i][3].x <= x1) || (r[i][4].x >= x2)) continue;

			// ここまで来れたら辺で接している。
			++used[i];

			// 左側にはみ出している場合、はみ出した部分を調査。
			if (r[i][4].x < x1) MergeObject(i, x1, r[i][4].x, y, tcnt,tp,tx,ty,used,r,rc);

			// 左下頂点が第１座標ではない場合、マージ後図形に頂点を追加。
			if (r[i][4].x != x1)
			{
				tx[tp] = r[i][4].x;
				ty[tp] = y;
				++tp;
			}

			// 左上頂点をマージ後図形に追加。
			tx[tp] = r[i][1].x;
			ty[tp] = r[i][1].y;
			++tp;

			// 上に尖った三角形でない場合、図形の上辺を調査し、マージ後図形に右上頂点を追加。
			if (r[i][1].x != r[i][2].x)
			{
				MergeObject(i, r[i][1].x, r[i][2].x, r[i][1].y, tcnt, tp, tx, ty, used, r, rc);
				tx[tp] = r[i][2].x;
				ty[tp] = r[i][2].y;
				++tp;
			}

			// 右下頂点が第２座標ではない場合、マージ後図形に頂点を追加。
			if (r[i][3].x != x2)
			{
				tx[tp] = r[i][3].x;
				ty[tp] = y;
				++tp;
			}

			// 図形が右側にはみ出している場合、はみ出した部分を調査。
			if (r[i][3].x > x2) MergeObject(i, r[i][3].x, x2, y, tcnt, tp, tx, ty, used, r, rc);
		}
	}
	else
	{
		for (i = rc - 1; i >= 0; --i)
		{
			// マージ済みなら次へ
			if (used[i] > 0) continue;

			// 上辺のｙ座標が違っているか、上に尖った三角形なら次へ
			if ((r[i][1].y != y) || (r[i][1].x == r[i][2].x)) continue;

			// 上辺のｘ範囲が外れていたら次へ
			if ((r[i][2].x <= x2) || (r[i][1].x >= x1)) continue;

			// ここまで来れたら辺で接している。
			++used[i];

			// 右側にはみ出している場合、はみ出した部分を調査。
			if (r[i][2].x > x1) MergeObject(i, x1, r[i][2].x, y, tcnt, tp, tx, ty, used, r, rc);

			// 右上頂点が第１座標ではない場合、マージ後図形に頂点を追加。
			if (r[i][2].x != x1)
			{
				tx[tp] = r[i][2].x;
				ty[tp] = y;
				++tp;
			}

			// 右下頂点をマージ後図形に追加。
			tx[tp] = r[i][3].x;
			ty[tp] = r[i][3].y;
			++tp;

			// 下に尖った三角形でない場合、図形の下辺を調査し、マージ後図形に左下頂点を追加。
			if (r[i][3].x != r[i][4].x)
			{
				MergeObject(i, r[i][3].x, r[i][4].x, r[i][3].y, tcnt, tp, tx, ty, used, r, rc);
				tx[tp] = r[i][0].x;
				ty[tp] = r[i][0].y;
				++tp;
			}

			// 左上頂点が第２座標ではない場合、マージ後図形に頂点を追加。
			if (r[i][1].x != x2)
			{
				tx[tp] = r[i][1].x;
				ty[tp] = y;
				++tp;
			}

			// 図形が左側にはみ出している場合、はみ出した部分を調査。
			if (r[i][1].x < x2) MergeObject(i, r[i][1].x, x2, y, tcnt, tp, tx, ty, used, r, rc);
		}
	}
}

// マージ処理
void OnMerge(int &tcnt, int &tp, double *tx, double *ty, double *used, std::vector<std::vector<D_Point>> &r, int &rc,int *rp) {
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
		tx[tp] = r[i][2].x;
		ty[tp] = r[i][2].y;
		++tp;
		tx[tp] = r[i][3].x;
		ty[tp] = r[i][3].y;
		++tp;

		// 図形が下に尖った三角形で無い場合、下辺を調査し、左下頂点をマージ後図形に追加。
		if (r[i][3].x != r[i][4].x)
		{
			MergeObject(i, r[i][3].x, r[i][4].x, r[i][3].y,tcnt, tp, tx, ty, used, r, rc);
			tx[tp] = r[i][4].x;
			ty[tp] = r[i][4].y;
			++tp;
		}

		// 左上の座標をマージ後図形に追加。
		tx[tp] = r[i][1].x;
		ty[tp] = r[i][1].y;
		++tp;

		// 図形が上に尖った三角形で無い場合、上辺を調査し、右上頂点をマージ後図形に追加。
		if (r[i][1].x != r[i][2].x)
		{
			MergeObject(i, r[i][1].x, r[i][2].x, r[i][1].y, tcnt, tp, tx, ty, used, r, rc);
			tx[tp] = r[i][2].x;
			ty[tp] = r[i][2].y;
			++tp;
		}

		// マージした結果を抽出図形に上書きコピーする。
		// 上書きされる図形はマージ済みなので問題ない。
		if (used[tcnt] == 0) printf("マージ済みでない図形が上書きされています！");
		for (j = 0; j < tp; ++j)
		{
			r[tcnt][j].x = tx[j];
			r[tcnt][j].y = ty[j];
		}
		rp[tcnt] = tp;
		++tcnt;
	}
	rc = tcnt;

}

// 冗長点削除の処理
void OnClean(int &tcnt, int &tp, double *tx, double *ty, double *used, std::vector<std::vector<D_Point>> &r, int &rc, int *rp) {
	int	i, j;
	double	a, b, c, len;

	if (tcnt == 0) return; // マージ前なら戻る。

	for (i = 0; i < rc; ++i)
	{
		tx[0] = r[i][0].x;
		ty[0] = r[i][0].y;
		tp = 1;
		for (j = 1; j < rp[i] - 1; ++j)
		{
			// 直線(tx[tp-1],ty[tp-1])-(r[i][j+1].x,r[i][j+1].y)に対して
			// 点(r[i][j].x,r[i][j].y)から垂線を下ろした時、
			// その長さが単位長未満なら冗長点とみなす。※としていたが，座標が整数でないか，lenが0なら冗長点とみなすに変更

			// 直線の方程式 ax+by+c=0，垂線の長さ len
			a = r[i][j + 1].y - ty[tp - 1];
			b = tx[tp - 1] - r[i][j + 1].x;
			c = r[i][j + 1].x * ty[tp - 1] - r[i][j + 1].y * tx[tp - 1];
			len = (a * r[i][j].x + b * r[i][j].y + c) / sqrt(a * a + b * b);
			
			if (fabs(len)<DBL_EPSILON || len==-0) {	//浮動小数点型におけるlen==0と同じこと
				//setColor(F_CYAN | F_INTENSITY, B_BLACK); printf("(%2lf,%2lf) len:%lf > 冗長点\n", r[i][j].x, r[i][j].y, len); setColor();
			}
			else if (floor( r[i][j].x)!=ceil(r[i][j].x) || floor(r[i][j].x) != ceil(r[i][j].x)) {	//座標が整数でないとき
				//setColor(F_RED | F_INTENSITY, B_BLACK); printf("(%2lf,%2lf) len:%lf > 冗長点\n", r[i][j].x, r[i][j].y, len); setColor();
			}
			else {
				//printf("(%2lf,%2lf) len:%lf\n", r[i][j].x, r[i][j].y, len);
				// 垂線が長いので冗長点ではない。
				tx[tp] = r[i][j].x;
				ty[tp] = r[i][j].y;
				++tp;
			}

			/*
			if ((len >= 0.5) || (len <= -0.5)){	
				// 垂線が長いので冗長点ではない。
				tx[tp] = r[i][j].x;
				ty[tp] = r[i][j].y;
				++tp;
			}*/

		}
		// 終始点一致させる
		tx[tp] = r[i][0].x;
		ty[tp] = r[i][0].y;
		++tp;

		// 冗長点削除後の座標でリストを上書き
		for (j = 0; j < tp; ++j)
		{
			r[i][j].x = tx[j];
			r[i][j].y = ty[j];
		}
		rp[i] = tp;
	}

}

// エリア内での左上座標を調べる
Point getUpperLeft(std::vector<std::vector<Point>> &areaPoint){
	Point point;
	int tall;			//直線方程式 y=-x+b のb
	for (int i = 0; i < (int)areaPoint.size(); i++) {
		//printf("area[%d]:", i);
		for (int j = 0; j < (int)areaPoint[i].size(); j++) {
			//printf("{%d,%d},", areaPoint[i][j].x, areaPoint[i][j].y);

			if (i == j&&i == 0) {	//暫定の左上
				tall = areaPoint[i][j].x + areaPoint[i][j].y;
				point.x = areaPoint[i][j].x; point.y = areaPoint[i][j].y;
			}
			else if (tall>areaPoint[i][j].x + areaPoint[i][j].y) {	//最小
				tall = areaPoint[i][j].x + areaPoint[i][j].y;
				point.x = areaPoint[i][j].x; point.y = areaPoint[i][j].y;
			}
			else if (tall == areaPoint[i][j].x + areaPoint[i][j].y &&point.x>areaPoint[i][j].x) {	//tallの値が同じならばxが小さい方を優先
				tall = areaPoint[i][j].x + areaPoint[i][j].y;
				point.x = areaPoint[i][j].x; point.y = areaPoint[i][j].y;
			}

		}
		//printf("\n");
	}
	//printf("暫定左上(%d,%d),tall:%d\n", point.x, point.y, tall);
	return point;
}