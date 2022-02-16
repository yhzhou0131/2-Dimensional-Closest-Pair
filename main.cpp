#include <iostream>
#include <limits>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>
/*
ref: https://www.geeksforgeeks.org/closest-pair-of-points-onlogn-implementation/
*/

using namespace std;

struct Point
{
	long long x, y;
	friend bool operator < (const struct Point& a, const struct Point& b);
};
inline bool operator < (const struct Point& a, const struct Point& b) {
	return (a.x != b.x) ? (a.x < b.x) : a.y < b.y;
}
map<Point, int> dict;
map<pair<int ,int>, int> res;
long long resDist = numeric_limits<long long>::max();

bool compareX(Point a, Point b)
{
	return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y);
}

bool compareY(Point a, Point b)
{
	return (a.y != b.y) ? (a.y < b.y) : (a.x < b.x);
}

long long dist(Point p1, Point p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + \
		(p1.y - p2.y) * (p1.y - p2.y);
}

void checkIfMin(Point a, Point b) {
	long long dis = dist(a, b);
	if (dis == resDist) {
		int a_ind = min(dict[a], dict[b]), b_ind = max(dict[a], dict[b]);
		pair<int, int> tmp;
		tmp = make_pair(a_ind, b_ind);
		res[tmp] = 1;
	}
	else if (dis < resDist) {
		res.clear();
		resDist = dis;

		int a_ind = min(dict[a], dict[b]), b_ind = max(dict[a], dict[b]);
		pair<int, int> tmp;
		tmp = make_pair(a_ind, b_ind);
		res[tmp] = 1;
	}
}

long long bruteForce(Point P[], int n)
{
	long long min = numeric_limits<long long>::max();
	for (int i = 0; i < n; ++i)
	{
		for (int j = i + 1; j < n; ++j)
		{
			if (dist(P[i], P[j]) < min)
			{
				min = dist(P[i], P[j]);
			}
			if (min <= resDist)
				checkIfMin(P[i], P[j]);
		}
	}
	return min;
}

long long min(long long x, long long y)
{
	return (x < y) ? x : y;
}

long long stripClosest(Point strip[], int size, long long d)
{
	long long min = d;

	for (int i = 0; i < size; ++i)
	{
		for (int j = i + 1; j < size && (strip[j].y - strip[i].y) <= sqrt(min); ++j)
		{
			long long dis = dist(strip[i], strip[j]);
			if (dis < min)
			{
				min = dis;
			}
			if (dis <= resDist) {
				checkIfMin(strip[i], strip[j]);
			}
		}
	}
	return min;
}

long long closestUtil(Point Px[], Point Py[], int n)
{
	if (n <= 3)
		return bruteForce(Px, n);

	int mid = n / 2;
	Point midPoint = Px[mid];

	Point *Pyl = new Point[mid];
	Point *Pyr = new Point[n - mid]; 
	int li = 0, ri = 0; 
	for (int i = 0; i < n; i++)
	{
		if ((Py[i].x < midPoint.x || (Py[i].x == midPoint.x && Py[i].y < midPoint.y)) && li < mid)
			Pyl[li++] = Py[i];
		else
			Pyr[ri++] = Py[i];
	}

	long long dl = closestUtil(Px, Pyl, mid);
	long long dr = closestUtil(Px + mid, Pyr, n - mid);

	long long d = min(dl, dr);

	Point *strip = new Point[n];
	int j = 0;
	for (int i = 0; i < n; i++)
		if (abs(Py[i].x - midPoint.x) <= sqrt(d))
			strip[j] = Py[i], j++;
	return stripClosest(strip, j, d);
}

long long closest(Point P[], int n)
{
	Point *Px = new Point[n];
	Point *Py = new Point[n];
	for (int i = 0; i < n; i++)
	{
		Px[i] = P[i];
		Py[i] = P[i];
	}

	sort(Px, Px + n, compareX);
	sort(Py, Py + n, compareY);

	return closestUtil(Px, Py, n);
}

int main()
{
	int n;
	bool xEqual = true;
	cin >> n;
	Point* P = new Point[n];
	for (int i = 0; i < n; i++) {
		long long x, y;
		cin >> x >> y;
		P[i].x = x;
		P[i].y = y;
		dict[P[i]] = i + 1;
	}
	cout << closest(P, n);
	// cout << " " << res.size() << endl;
	printf(" %d\n", res.size());
	for (auto it = res.begin(); it != res.end(); ++it) {
		// cout << (it->first).first << " " << (it->first).second << endl;
		printf("%d %d\n", (it->first).first, (it->first).second);
	}

	return 0;
}
