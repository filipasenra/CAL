/*
 * NearestPoints.cpp
 */

#include <limits>
#include <thread>
#include <algorithm>
#include <cmath>
#include "NearestPoints.h"
#include "Point.h"

const double MAX_DOUBLE = std::numeric_limits<double>::max();

Result::Result(double dmin, Point p1, Point p2) {
	this->dmin = dmin;
	this->p1 = p1;
	this->p2 = p2;
}

Result::Result() {
	this->dmin = MAX_DOUBLE;
	this->p1 = Point(0, 0);
	this->p2 = Point(0, 0);
}

/**
 * Auxiliary functions to sort vector of points by X or Y axis.
 */
static void sortByX(vector<Point> &v, int left, int right) {
	std::sort(v.begin() + left, v.begin() + right + 1,
			[](Point p, Point q) {return p.x < q.x || (p.x == q.x && p.y < q.y);});
}

static void sortByY(vector<Point> &v, int left, int right) {
	std::sort(v.begin() + left, v.begin() + right + 1,
			[](Point p, Point q) {return p.y < q.y || (p.y == q.y && p.x < q.x);});
}

/**
 * Brute force algorithm O(N^2).
 */
Result nearestPoints_BF(vector<Point> &vp) {
	Result res;

	for (int i = 0; i < vp.size(); i++) {
		for (int j = i + 1; j < vp.size(); j++) {
			if (vp.at(i).distance(vp.at(j)) < res.dmin) {
				res.dmin = vp.at(i).distance(vp.at(j));
				res.p1 = vp.at(i);
				res.p2 = vp.at(j);

				/*if (res.dmin == 0)
				 return res;*/
			}
		}
	}

	return res;
}

/**
 * Improved brute force algorithm, that first sorts points by X axis.
 */
Result nearestPoints_BF_SortByX(vector<Point> &vp) {
	Result res;
	sortByX(vp, 0, vp.size() - 1);
	// TODO
	return res;
}

/**
 * Auxiliary function to find nearest points in strip, as indicated
 * in the assignment, with points sorted by Y coordinate.
 * The strip is the part of vp between indices left and right (inclusive).
 * "res" contains initially the best solution found so far.
 */
static void npByY(vector<Point> &vp, int left, int right, Result &res) {
	int n = abs(right - left);

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (abs(vp.at(j).y - vp.at(i).y) > res.dmin)
				break;
			else if (vp.at(i).distance(vp.at(j)) < res.dmin) {
				res.p1 = vp.at(i);
				res.p2 = vp.at(j);
				res.dmin = vp.at(i).distance(vp.at(j));
			}
		}
	}

}

/**
 * Recursive divide and conquer algorithm.
 * Finds the nearest points in "vp" between indices left and right (inclusive),
 * using at most numThreads.
 */
static Result np_DC(vector<Point> &vp, int left, int right, int numThreads) {

	int range = abs(right-left);
	// Base case of two points
	if (range == 1)
		return Result(vp.at(left).distance(vp.at(right)), vp.at(left),
				vp.at(right));

	// Base case of a single point: no solution, so distance is MAX_DOUBLE
	if (range == 0)
		return Result(MAX_DOUBLE, vp.at(right), vp.at(left));

	// Divide in halves (left and right) and solve them recursively,
	// possibly in parallel (in case numThreads > 1)

	Result resleft = np_DC(vp, left, range / 2, numThreads);
	Result resright = np_DC(vp, range / 2 + 1, right, numThreads);

	// Select the best solution from left and right
	Result resBest;

	if (resleft.dmin < resright.dmin)
		resBest = resleft;
	else
		resBest = resright;

	// Determine the strip area around middle point
	double dist = resBest.dmin;
	//getting the coordenates of the middle
	double middle = (vp.at(range / 2).distance(
			vp.at(range / 2 + 1))) / 2.0;

	int stripLeft;
	int stripRight;
	//Pontos à esquerda
	for (int i = range / 2; i > 0; i--) {
		if (abs(vp.at(i).x - middle) <= dist)
			stripLeft = i;
		else
			break;
	}

	//Pontos à direita
	for (int i = range / 2 + 1; i < vp.size(); i++) {
		if (abs(vp.at(i).x - middle) <= dist)
			stripRight = i;
		else
			break;
	}

	// Order points in strip area by Y coordinate
	sortByY(vp, stripLeft, stripRight);

	// Calculate nearest points in strip area (using npByY function)
	//npByY(vp, stripLeft, stripRight, resBest);

	// Reorder points in strip area back by X coordinate
	sortByX(vp, stripLeft, stripRight);

	return resBest;
}

/**
 * Defines the number of threads to be used.
 */
static int numThreads = 1;
void setNumThreads(int num) {
	numThreads = num;
}

/*
 * Divide and conquer approach, single-threaded version.
 */
Result nearestPoints_DC(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() - 1);
	return np_DC(vp, 0, vp.size() - 1, 1);
}

/*
 * Multi-threaded version, using the number of threads specified
 * by setNumThreads().
 */
Result nearestPoints_DC_MT(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() - 1);
	return np_DC(vp, 0, vp.size() - 1, numThreads);
}
