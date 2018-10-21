#include "pch.h"
#include "SolutionByGecode.h"
#include "PropagaterDef.h"
#include "BrancherDef.h"


SolutionByGecode::SolutionByGecode(void)
{
}


SolutionByGecode::~SolutionByGecode()
{
}

SolutionByGecode::SolutionByGecode(DTP* d, DistanceGraph* dg) : metaVarArr(*this, d->DisjunctionCount()) {
	dtp = d;
	if (dtp == NULL) {
		throw new exception("No input DTP for SolutionByGecode.");
		return;
	}
	drg = dg;

	// modeling
	int n = dtp->DisjunctionCount();
	vector<Disjunction*>::iterator it = dtp->AllDisjunctions()->begin();
	for (int i = 0; i < n; i++) {
		IntVar* ptr = new IntVar(*this, 0, (*it)->DisjunctCount()-1);
		metaVarArr[i] = *ptr;
		it++;
	}
	// Post core module
	auto Domain_Min_First = [](const Space& home, IntVar x, int i) {
		return x.size();
	};
	auto Position_Min_First = [](const Space& home, IntVar x, int i) {
		return i; // i is the position of x in var-array;
	};

	Rnd r(1U);
	branch(*this, metaVarArr, INT_VAR_RND(r), INT_VAL_MIN());//INT_VAL_RND(r));
//	branch(*this, metaVarArr, INT_VAR_NONE(), INT_VAL_MIN());
//	branch(*this, metaVarArr, INT_VAR_MERIT_MIN(Position_Min_First), INT_VAL_MIN());
//	branch(*this, metaVarArr, INT_VAR_MERIT_MIN(Domain_Min_First), INT_VAL_MIN());
	post(*this, metaVarArr);
}

SolutionByGecode::SolutionByGecode (SolutionByGecode& s) : Space(s) {
	dtp = s.dtp;
	drg = s.drg;
	metaVarArr.update(*this, s.metaVarArr);
}
Space* SolutionByGecode::copy(void) {
	return new SolutionByGecode(*this);
}
void SolutionByGecode::print(void) const {
	std::cout << metaVarArr << std::endl;
}


void SolutionByGecode::post(Space& home, IntVarArray& var) {
//	trace(home, TE_COMMIT|TE_PRUNE);
	model_def::NaiveBrancher::post(home, var);
	FCPropagator::post(home, var, dtp);
//	IncPropagator::post(home, var, dtp, drg);

}