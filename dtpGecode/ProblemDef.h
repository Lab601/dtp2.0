#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include <vector>

using namespace std;

namespace dtp_definition {
class Disjunction;

	class Variable {
	private:
		string varName;
	public:
		inline string name() {
			return varName;
		}
		Variable();
		Variable(string);
		~Variable();
	};


	//A disjunct is in form of "first-second<=right";
	class Disjunct {
	private:
		unsigned firstLeft;
		unsigned secondLeft;
		double right;
		Disjunction* disjunction;
	public:
		inline unsigned first() {
			return firstLeft;
		}
		inline unsigned second() {
			return secondLeft;
		}
		inline double weight() {
			return right;
		}
		Disjunct();
		Disjunct(Disjunction*, unsigned, unsigned, double);
		~Disjunct();
	};

	class Disjunction {

	private:
		vector<Disjunct*> disjuncts;
	public:
		Disjunction();
		~Disjunction();
		inline Disjunct* Disjuncts(int i) {
			if(i<disjuncts.size()) return disjuncts[i];
			else return NULL;
		}
		inline Disjunct* DisjunctAt(int i) {
			if (i >= 0 && i < disjuncts.size()) return disjuncts[i];
			else return NULL;
		};
		void AddDisjunct(Disjunct*);
		inline int DisjunctCount() {
			return disjuncts.size();
		}
	};


	//A DTP is a conjunction of conjuncts (aka. disjunctions).
	class DTP
	{
	private:
		map<string, unsigned> varMap;
		vector<Variable*> varStore;
		vector<Disjunct*> disjunctStore;
		vector<Disjunction*> disjunctionStore;

		string trim(string);

	public:
		DTP();
		~DTP();
		inline int VariableCount() {
			return varStore.size();
		}
		inline int DisjunctsCount() {
			return disjunctStore.size();
		}
		inline int DisjunctionCount() {
			return disjunctionStore.size();
		}

		void AddDisjunction(string dist);
		Disjunct* AddDisjunct(Disjunction*, string dis);

		unsigned AddVariable(string var);

		inline vector<Variable*>* AllVariables() {
			return &varStore;
		}
		inline vector<Disjunction*>* AllDisjunctions() {
			return &disjunctionStore;
		}

	};

}