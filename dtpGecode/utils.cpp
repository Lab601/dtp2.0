#include "pch.h"
#include "utils.h"


double DistanceGraph::DG(int i, int j) {
	return this->dg[i*dim + j];
}
DistanceGraph::DistanceGraph(int d) : dim(d) {
	if (dg == NULL) {
		dg = new double[d*d];
		DGinit();
	}
};
void DistanceGraph::DGinit() {
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			if (i == j) DGset(i, j, 0);
			else DGset(i, j, FLT_MAX);
}
