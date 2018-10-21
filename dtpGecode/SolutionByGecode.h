#pragma once
#include <gecode/int.hh>
#include <gecode/search.hh>
#include <limits>

#include "ProblemDef.h"
#include "utils.h"
#include "ProblemDef.h"
#include "BrancherDef.h"

using namespace Gecode;
using namespace dtp_definition;
using namespace model_def;
/**
 * Solution space definition to solve DTP via Gecode.
 *
 * @author YCLIU
 * @version 1.0.0
 */


class SolutionByGecode : public Space
{
private:
	DTP* dtp;
	IntVarArray metaVarArr;

	void post(Space& home, IntVarArray& var);

public:
	DistanceGraph* drg;
	inline DistanceGraph* dg() {
		return drg;
	}
	SolutionByGecode();
	~SolutionByGecode();
	SolutionByGecode(DTP*, DistanceGraph*);
	SolutionByGecode(SolutionByGecode& s);
	virtual Space* copy();
	void print() const;
};
