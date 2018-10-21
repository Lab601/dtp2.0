#pragma once
#include <gecode/int.hh>
#include <gecode/search.hh>
#include "ProblemDef.h"
#include "utils.h"

using namespace Gecode;
using namespace dtp_definition;

namespace model_def {

	//Full consistency check propagator
	class BasePropagator : public Propagator {

	protected:
		ViewArray<Int::IntView> x;
		DTP* dtp;
		unsigned varCount;

	public:

		static ExecStatus post(Space& home, IntVarArray& iv0, DTP* d);
		BasePropagator(Space& home, ViewArray<Int::IntView> iv0, DTP* d);
		size_t dispose(Space& home);
		BasePropagator(Space& home, BasePropagator& p);
		Propagator* copy(Space& home);
		PropCost cost(const Space&, const ModEventDelta&) const;
		void reschedule(Space& home);
		ExecStatus propagate(Space& home, const ModEventDelta&);
	};

	class FCPropagator : public BasePropagator {

	protected:
		int varCount;

	public:
		static DistanceGraph* drg;

		static ExecStatus post(Space& home, IntVarArray& iv0, DTP* d);
		FCPropagator(Space& home, ViewArray<Int::IntView> iv0, DTP* d);
		size_t dispose(Space& home);
		FCPropagator(Space& home, FCPropagator& p);
		Propagator* copy(Space& home);
		ExecStatus propagate(Space& home, const ModEventDelta&);
	};

	class IncPropagator : public BasePropagator {

	protected:
		DistanceGraph* drg;

	public:

		static ExecStatus post(Space& home, IntVarArray& iv0, DTP* d, DistanceGraph*);
		size_t dispose(Space& home);
		IncPropagator(Space& home, ViewArray<Int::IntView> iv0, DTP* d, DistanceGraph* dg);
		IncPropagator(Space& home, IncPropagator& p);
		Propagator* copy(Space& home);
		ExecStatus propagate(Space& home, const ModEventDelta&);
	};
}
