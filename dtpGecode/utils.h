#pragma once
#include <stdio.h>
#include <limits>


class DistanceGraph
{
private:
	double* dg = NULL;
	unsigned dim = 0;
public:
	DistanceGraph() {};
	DistanceGraph(int d);
	~DistanceGraph() {
		if (dg) delete[] dg;
	}
	double DG(int i, int j);
	void DGinit();
	inline void DGset(int i, int j, double w) {
		dg[i*dim + j] = w;
	}
	// initialize distance graph
	inline double DGAdd(double d1, double d2) {
		if (d1 == FLT_MAX || d2 == FLT_MAX)
			return FLT_MAX;
		else return d1 + d2;
	}
};