#include "pch.h"
#include "PropagaterDef.h"


namespace model_def {
	ExecStatus BasePropagator::post(Space& home, IntVarArray& v0, DTP* d) {
		IntVarArgs ia;
		ia << v0;
		ViewArray<Int::IntView> va(home, ia);
		(void) new (home) BasePropagator(home, va, d);
		return ES_OK;
	}

	BasePropagator::BasePropagator(Space& home, ViewArray<Int::IntView> iv0, DTP* d) : Propagator(home), dtp(d), x(iv0){
		varCount = d->AllVariables()->size();
		x.subscribe(home, *this, Int::PC_INT_DOM);
	}

	size_t BasePropagator::dispose(Space& home) {
		x.cancel(home, *this, Int::PC_INT_DOM);
		(void)Propagator::dispose(home);
		return sizeof(*this);
	}

	BasePropagator::BasePropagator(Space& home, BasePropagator& p)
		: Propagator(home, p) {
		x.update(home, p.x);
		dtp = p.dtp;
		varCount = p.varCount;
	}
	Propagator* BasePropagator::copy(Space& home) {
		return new (home) BasePropagator(home, *this);
	}
	PropCost BasePropagator::cost(const Space&, const ModEventDelta&) const {
		return PropCost::binary(PropCost::LO);
	}

	void BasePropagator::reschedule(Space& home) {
		x.reschedule(home, *this, Int::PC_INT_DOM);
	}

	ExecStatus BasePropagator::propagate(Space& home, const ModEventDelta&) {

		return ES_OK;
	}
	ExecStatus FCPropagator::post(Space& home, IntVarArray& v0, DTP* d) {
		IntVarArgs ia;
		ia << v0;
		ViewArray<Int::IntView> va(home, ia);
		(void) new (home) FCPropagator(home, va, d);
		return ES_OK;
	}

	DistanceGraph* FCPropagator::drg=NULL;

	FCPropagator::FCPropagator(Space& home, ViewArray<Int::IntView> iv0, DTP* d) : BasePropagator(home, iv0, d) {
		varCount = d->AllVariables()->size();
		if (drg == NULL) {
			drg = new DistanceGraph(varCount);
		}
		x.subscribe(home, *this, Int::PC_INT_DOM);
	}

	size_t FCPropagator::dispose(Space& home) {
		x.cancel(home, *this, Int::PC_INT_DOM);
		(void)BasePropagator::dispose(home);
		return sizeof(*this);
	}

	FCPropagator::FCPropagator(Space& home, FCPropagator& p)
		: BasePropagator(home, p) {
		x.update(home, p.x);
		varCount = p.varCount;
	}
	Propagator* FCPropagator::copy(Space& home) {
		return new (home) FCPropagator(home, *this);
	}

	ExecStatus FCPropagator::propagate(Space& home, const ModEventDelta&) {

		drg->DGinit();
		// get assigned edge
		for (int i = 0; i < varCount; i++) {
			if (x[i].assigned()) {
				vector<Disjunction*>* d = dtp->AllDisjunctions();
				Disjunction* dist = (*d)[i];
				Disjunct* dis = dist->Disjuncts(x[i].val());
				unsigned x = dis->first();
				unsigned y = dis->second();
				double w = dis->weight();
				if (drg->DG(x, y) >= FLT_MAX || drg->DG(x, y) > w)
					drg->DGset(x, y, w);
			}
		}
		// check consistency
		for (int k = 0; k < varCount; k++) {
			for (int i = 0; i < varCount; i++) {
				for (int j = 0; j < varCount; j++) {
					double t = drg->DGAdd(drg->DG(i, k), drg->DG(k, j));
					if (drg->DG(i, j) > t) {
						// whether exists a negative circle
						if (i == j && t < 0)
							return ES_FAILED;
						else
							drg->DGset(i, j, t);
					}
				}
			}
		}
		return ES_OK;
	}

	ExecStatus IncPropagator::post(Space& home, IntVarArray& v0, DTP* d, DistanceGraph* dg) {
		IntVarArgs ia;
		ia << v0;
		ViewArray<Int::IntView> va(home, ia);
		(void) new (home) IncPropagator(home, va, d, dg);
		return ES_OK;
	}

	IncPropagator::IncPropagator(Space& home, ViewArray<Int::IntView> iv0, DTP* d, DistanceGraph* dg) : BasePropagator(home, iv0, d), drg(dg) {
		varCount = d->AllVariables()->size();
		drg = dg;
		x.subscribe(home, *this, Int::PC_INT_DOM);
	}

	size_t IncPropagator::dispose(Space& home) {
		x.cancel(home, *this, Int::PC_INT_DOM);
		(void)BasePropagator::dispose(home);
		return sizeof(*this);
	}

	IncPropagator::IncPropagator(Space& home, IncPropagator& p)
		: BasePropagator(home, p) {
		x.update(home, p.x);
		dtp = p.dtp;
		varCount = p.varCount;
		drg = p.drg;
	}
	Propagator* IncPropagator::copy(Space& home) {
		return new (home) IncPropagator(home, *this);
	}
	
	ExecStatus IncPropagator::propagate(Space& home, const ModEventDelta&) {

		for (int i = 0; i < varCount; i++)
			if (drg->DG(i, i) < 0) return ES_FAILED;
		return ES_OK;
	}
}