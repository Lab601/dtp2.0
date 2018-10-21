#include "pch.h"
#include <gecode/int.hh>
#include <gecode/search.hh>

using namespace Gecode;

/**
 * \brief %Example: SEND+MORE=MONEY puzzle
 *
 * Well-known cryptoarithmetic puzzle.
 * Henry Dudeney, Strand Magazine, July 1924.
 *
 * \ingroup Example
 *
 */
class SendMoreMoney : public Space {
protected:
	IntVarArray le;
public:

	SendMoreMoney(void) : le(*this, 8, 0, 9) {
		IntVar
			s(le[0]), e(le[1]), n(le[2]), d(le[3]),
			m(le[4]), o(le[5]), r(le[6]), y(le[7]);

		rel(*this, s, IRT_NQ, 0);
		rel(*this, m, IRT_NQ, 0);

		distinct(*this, le);

		//linear equation
		IntArgs c(4 + 4 + 5); IntVarArgs x(4 + 4 + 5);
		c[0] = 1000; c[1] = 100; c[2] = 10; c[3] = 1;
		x[0] = s; x[1] = e; x[2] = n; x[3] = d;
		c[4] = 1000; c[5] = 100; c[6] = 10; c[7] = 1;
		x[4] = m; x[5] = o; x[6] = r; x[7] = e;
		c[8] = -10000; c[9] = -1000; c[10] = -100; c[11] = -10; c[12] = -1;
		x[8] = m; x[9] = o; x[10] = n; x[11] = e; x[12] = y;
		linear(*this, c, x, IRT_EQ, 0);

		//post branching
		branch(*this, le, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
	}
	SendMoreMoney(SendMoreMoney& s) : Space(s) {
		le.update(*this, s.le);
	}
	virtual Space* copy(void) {
		return new SendMoreMoney(*this);
	}
	void print(void) const {
		std::cout << le << std::endl;
	}
};


/** \brief Main-function
 *  \relates Money
 */
int main_(int argc, char* argv[]) {
	//create model and search engine
	SendMoreMoney* m = new SendMoreMoney;
	DFS<SendMoreMoney> e(m);

	//search and print all solutions
	while (SendMoreMoney* s = e.next()) {
		s->print();
		delete s;
	}

	return 0;
}