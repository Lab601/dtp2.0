#pragma once
#include <gecode/int.hh>
#include <gecode/search.hh>
#include <limits>

#include "ProblemDef.h"
#include "utils.h"
#include "SolutionByGecode.h"

using namespace Gecode;
using namespace dtp_definition;

namespace model_def {
	class NaiveBrancher : Brancher {
	private:
		ViewArray<Int::IntView> x;

	public:
		//	static void post();
		NaiveBrancher(Space& home, ViewArray<Int::IntView>& x0);
		NaiveBrancher(Space& home, bool share, NaiveBrancher& m);
		NaiveBrancher(Space& home, NaiveBrancher& x0);
		NaiveBrancher(Space& home);

		/**
		 * Return if this brancher has alternatives left.
		 */
		class PosVal : public Choice {
		public:
			int pos; int val;
			PosVal(const NaiveBrancher& b, int p, int v) : Choice(b, 2), pos(p), val(v) {}
			virtual void archive(Archive& e) const {
				Choice::archive(e);
				e << pos << val;
			}
		};

		virtual Choice* choice(Space& home);
		virtual Choice* choice(const Space&, Archive& e);

		/**
		 * Commit to an alternative with branching description.
		 */
		virtual bool status(const Space& home) const {
//			const SolutionByGecode& q = static_cast<const SolutionByGecode&>(home);
			//
			// No non-assigned orders left
			return false;
		}
		virtual ExecStatus commit(Space& home, const Choice& d, unsigned int a) {

			const PosVal& pv = static_cast<const PosVal&>(d);
			int pos = pv.pos, val = pv.val;
			if (a == 0)
				return me_failed(x[pos].eq(home, val)) ? ES_FAILED : ES_OK;
			else
				return me_failed(x[pos].nq(home, val)) ? ES_FAILED : ES_OK;
		}
		virtual void print(const Space&, const Gecode::Choice& _c,
			unsigned int a,
			std::ostream& o) const {
			const PosVal& c = static_cast<const PosVal&>(_c);
			bool val = (a == 0) ? c.val : !c.val;
			o << "w[" << c.pos << "] = " << val;
		}
		virtual Actor* copy(Space& home) {
			return new (home) NaiveBrancher(home, *this);
		}
		static void post(SolutionByGecode& home) {
			(void) new (home) NaiveBrancher(home);
		}
		virtual size_t dispose(Space&) {
			return sizeof(*this);
		}
	};

	class QueenBranch : public Brancher {
	private:
		mutable int start;
		class Choice : public Gecode::Choice {
		public:
			int pos;
			bool val;
			Choice(const Brancher& b, int pos0, bool val0)
				: Gecode::Choice(b, 2), pos(pos0), val(val0) {}
			virtual void archive(Archive& e) const {
				Gecode::Choice::archive(e);
				e << pos << val;
			}
		};

		QueenBranch(Home home)
			: Brancher(home), start(0) {}
		QueenBranch(Space& home, QueenBranch& b)
			: Brancher(home, b), start(b.start) {}

	public:
		virtual bool status(const Space& home) const {
			const SolutionByGecode& q = static_cast<const SolutionByGecode&>(home);
			//
			// No non-assigned orders left
			return false;
		}
		virtual Gecode::Choice* choice(Space& home) {
			const SolutionByGecode& q = static_cast<const SolutionByGecode&>(home);
			int maxsize = -1;
			int pos = -1;
/*
			for (int i = start; i < q.n*q.n; ++i) {
				if (q.w[i].assigned()) continue;
				IntSetRanges ai(A[i]);
				SetVarUnknownRanges qU(q.U);
				Iter::Ranges::Inter<IntSetRanges, SetVarUnknownRanges> r(ai, qU);
				int size = Iter::Ranges::size(r);
				if (size > maxsize) {
					maxsize = size;
					pos = i;
				}
			}
			*/
			assert(pos != -1);
			return new Choice(*this, pos, true);
		}
		virtual Choice* choice(const Space&, Archive& e) {
			int pos, val;
			e >> pos >> val;
			return new Choice(*this, pos, val);
		}
		virtual ExecStatus commit(Space& home, const Gecode::Choice& _c,
			unsigned int a) {
			SolutionByGecode& q = static_cast<SolutionByGecode&>(home);
			const Choice& c = static_cast<const Choice&>(_c);
			bool val = (a == 0) ? c.val : !c.val;
			return me_failed//(Int::BoolView(q.w[c.pos]).eq(q, val))
				? ES_FAILED
				: ES_OK;
		}
		virtual void print(const Space&, const Gecode::Choice& _c,
			unsigned int a,
			std::ostream& o) const {
			const Choice& c = static_cast<const Choice&>(_c);
			bool val = (a == 0) ? c.val : !c.val;
			o << "w[" << c.pos << "] = " << val;
		}
		virtual Actor* copy(Space& home) {
			return new (home) QueenBranch(home, *this);
		}
		static void post(SolutionByGecode& home) {
			(void) new (home) QueenBranch(home);
		}
		virtual size_t dispose(Space&) {
			return sizeof(*this);
		}
	};
}