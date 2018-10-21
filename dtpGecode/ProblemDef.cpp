#include "pch.h"
#include "ProblemDef.h"
#include <stdio.h>
#include <iostream>
#include <objbase.h>

using namespace std;

namespace dtp_definition {

	Variable::Variable() {
	}

	Variable::~Variable() {
	}

	Variable::Variable(string s) :varName(s) {

	}

	Disjunct::Disjunct() {
	}
	Disjunct::Disjunct(Disjunction* dis, unsigned leftOne, unsigned leftTwo, double w) {
		if (leftOne>=0 && leftTwo>=0 && leftOne!=leftTwo) {
			firstLeft = leftOne;
			secondLeft = leftTwo;
			right = w;
			disjunction = dis;
		}
		else throw new exception("Null variable insertion when constructing disjuncts!");
	}
	Disjunct::~Disjunct() {
	}

	Disjunction::Disjunction() {

	}
	Disjunction::~Disjunction() {

		for (vector<Disjunct*>::iterator it = disjuncts.begin(); it != disjuncts.end(); it++) {
			delete *it;
		}
	}
	void Disjunction::AddDisjunct(Disjunct* dist) {
		if (dist == NULL) return;
		disjuncts.push_back(dist);
	}


	DTP::DTP()
	{

	}


	DTP::~DTP()
	{
		for (vector<Disjunction*>::iterator it = disjunctionStore.begin(); it != disjunctionStore.end(); it++) {
			delete *it; 
		}
	}

	string DTP::trim(string str) {
		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ") + 1);
		return str;
	}

	void DTP::AddDisjunction(string str) {

		std::string::size_type posV;

		Disjunction* dist = new Disjunction();
		while (1) {
			posV = str.find('v');
			string disjunct = trim(str.substr(0, posV));

			if (AddDisjunct(dist, disjunct) == NULL) break;
			if (posV == string::npos) break;
			str = str.substr(posV + 1, string::npos);
		}
		disjunctionStore.push_back(dist);
	}

	Disjunct* DTP::AddDisjunct(Disjunction* dist, string disjunct) {
		// disjunct ("x-y<=d") 
		string::size_type posS, posE;
		string var1, var2, right;
		Disjunct* dis = NULL;

		try {
			posS = disjunct.find('-');
			posE = disjunct.find('<');
			if (posS == string::npos || posE == string::npos) return NULL;
			right = disjunct.substr(posE + 2, string::npos);
			var1 = disjunct.substr(0, posS);
			var2 = disjunct.substr(posS + 1, posE - posS - 1);
			double b = std::stof(right.c_str());
			cout << "var1: " << var1 << ", var2: " << var2 << "  right: " << right << endl;

			dis = new Disjunct(dist, AddVariable(var1), AddVariable(var2), b);
			disjunctStore.push_back(dis);
			dist->AddDisjunct(dis);
			//a constraint with same var1 and var2 ("var1-var2<=right") does not exist yet
/*			if (disjunctStore.find(var1+":"+var2) == disjunctStore.end()) {
				dis = new Disjunct(dist, AddVariable(var1), AddVariable(var2), b);
				dist->AddDisjunct(dis);
				disjunctStore.insert(pair<string, Disjunct*>(var1 + ":" + var2, dis));
			}
			else { // more than one disjuncts with the same variable var1 and var2
				string var3 = "~" + var1;
				string var4 = "~" + var2;
				dis = new Disjunct(dist, AddVariable(var3), AddVariable(var4), atof(right.c_str()));
				disjunctStore.insert(pair<string, Disjunct*>(var3 + ":" + var4, dis));
				AddDisjunction(var1 + "-" + var3 + "<=0");
				AddDisjunction(var3 + "-" + var1 + "<=0");
				AddDisjunction(var2 + "-" + var4 + "<=0");
				AddDisjunction(var4 + "-" + var2 + "<=0");
			} */
			return dis;
		}
		catch (exception e) {
			return NULL;
		}
	}

	unsigned DTP::AddVariable(string varName) {
		map<string, unsigned>::iterator it = varMap.find(varName);
		if (it == varMap.end()) {
			Variable* var = new Variable(varName);
			unsigned idx = varStore.size();
			varMap.insert(pair<string, unsigned>(varName, idx));
			varStore.push_back(var);
			return idx;
		}
		else {
			return it->second;
		}
	}
}