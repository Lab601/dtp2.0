#include "pch.h"
#include "BrancherDef.h"

namespace model_def {
	NaiveBrancher::NaiveBrancher(Space& home, ViewArray<Int::IntView>& x0) : Brancher(home), x(x0) { }

	NaiveBrancher::NaiveBrancher(Space& home, bool share, NaiveBrancher& m) : Brancher(home), x(m.x) {
	}
	NaiveBrancher::NaiveBrancher(Space& home, NaiveBrancher& nb): Brancher(home), x(nb.x){
	}
	NaiveBrancher::NaiveBrancher(Space& home) : Brancher(home) {
	}


	/**
	 * Return if this branching has alternatives left.
	 */
//	bool NaiveBrancher::status(Space& home) {
//		return (!x.assigned());
//	}

	/**
	 * Return a branching choice.
	 */
	Choice* NaiveBrancher::choice(Space& home) {
		for (int i = 0; true; i++)
			if (!x[i].assigned())
				return new PosVal(*this, i, x[i].min());
		GECODE_NEVER;
		return NULL;
	}
	Choice* NaiveBrancher::choice(const Space&, Archive& e) {
		int pos, val;
		e >> pos >> val;
		return new PosVal(*this, pos, val);
	}
	/**
	* Commit to an alternative with branching choice.
	*/
	ExecStatus NaiveBrancher::commit(Space& home, Choice& d, unsigned int a) {

		const PosVal& pv = static_cast<const PosVal&>(d);
		int pos = pv.pos, val = pv.val;
		if (a == 0)
			return me_failed(x[pos].eq(home, val)) ? ES_FAILED : ES_OK;
		else
			return me_failed(x[pos].nq(home, val)) ? ES_FAILED : ES_OK;
	}
}