#include "pch.h"
#include "IncrementalEventTracer.h"

namespace event_tracer {

	int IncrementalEventTracer::Inc_Floyd(Disjunct* add)
	{
		unsigned var_num = 0;
		vector<unsigned> fNode; //forward node which is affected;
		vector<unsigned> bNode;  //backward node which is affected;

		if (add == NULL) return -1;
		var_num = dtp->VariableCount();
		unsigned _i = add->first();
		unsigned _j = add->second();
		double _w = add->weight();
		DistanceGraph* drg = sbg->dg();

		if (_w >= drg->DG(_i, _j))
		{
			return(0);
		}

		for (int j = 0; j < var_num; j++)
			if (drg->DG(_j, j) < FLT_MAX) fNode.push_back(j);
		for (int i = 0; i < var_num; i++)
			if (drg->DG(i, _i) < FLT_MAX) bNode.push_back(i);
		vector<DGElement*>* backup = new vector<DGElement*>();

		for (vector<unsigned>::iterator it1 = bNode.begin(); it1 != bNode.end(); it1++)
			for (vector<unsigned>::iterator it2 = fNode.begin();it2 != fNode.end();it2++) {
				double x0 = drg->DG(*it1, *it2);
				double x1 = drg->DGAdd(drg->DG(*it1, _i), _w);
				x1= x1+drg->DG(_j, *it2);
				if (x1 < x0) {
					backup->push_back(new DGElement(*it1, *it2, x0));
					drg->DGset(*it1, *it2, x1);
				}
			}
		DGbackup.push_back(backup);
		return 0;
	}
	EventTracer::EventTracer(void)
	{
	}

	IncrementalEventTracer::IncrementalEventTracer(DTP* d, SolutionByGecode* s)
	{
		dtp = d;
		sbg = s;
	}
	void IncrementalEventTracer::node(const EdgeInfo& ei, const NodeInfo& ni) {
		std::cout << " trace<Search>::node(";
		switch (ni.type()) {
		case NodeType::FAILED: {
			std::cout << "FAILED(" << ")";
			break;
		}
		case NodeType::BRANCH: {
			std::cout << "BRANCH(" << ni.choice().alternatives() << ")";
			break;
		}
		case NodeType::SOLVED: {
			std::cout << "SOLVED(" << ")";
			break;
		}
		default: std::cout << "OTHER_NODE(" << ")";
		}
		std::cout << ";" << "w£º" << ni.wid() << "," << "n: " << ni.nid() << ")";
		if (ei) {
			if (ei.wid() != ni.wid())
				std::cout << "stolen from w: " << ei.wid() << "]";
			std::cout << std::endl << "\t" << ei.string() << std::endl;
//			const Choice& c = static_cast<const Choice&>(ni.choice());
//			unsigned val = c.val;
//			unsigned pos = c.pos;

			NodeCreated(ei.string());
		}
		else {
			std::cout << std::endl;
		}
	}

	IncrementalEventTracer::~IncrementalEventTracer(void) {
		for (vector<vector<DGElement*>*>::iterator it = DGbackup.begin(); it != DGbackup.end(); it++) {
			if (*it != NULL)
				for (vector<DGElement*>::iterator it2 = (*it)->begin(); it2 != (*it)->end(); it2++)
					if (*it2 != NULL) delete *it2;
		}
	}

	void IncrementalEventTracer::NodeCreated(string str) {
		//the string is like "var[0]!=3"
		size_t pos, posVS, posVE, posVal;
		posVS = str.find("[");
		posVE = str.find("]");
		string strVar = str.substr(posVS+1, posVE-1);
		if ((pos = str.find("!=")) != string::npos) {
			vector<DGElement*>* e = DGbackup.back();
			if (e == NULL) {
				cout << "error happens!" << endl;
			}
			DGbackup.pop_back();
			for (vector<DGElement*>::iterator it = e->begin(); e!=NULL&&it != e->end(); it++) {
				DGElement* x = *it;
				DistanceGraph* drg = sbg->dg();
				drg->DGset(x->from, x->to, x->weight);
				delete x;
			}
			delete e;
		}
		else if ((posVal = str.find("=")) != string::npos) {   //the string is like "var[0]=3"
			string strVal = str.substr(posVal, string::npos);
			int varIdx = atoi(strVar.c_str());
			int valIdx = atoi(strVal.c_str());
			vector<Disjunction*>* vecDist = dtp->AllDisjunctions();
			Disjunction* dist=NULL;
			Disjunct* dis = NULL;
			if (vecDist != NULL) {
				dist = vecDist->at(varIdx);
				if (dist != NULL) {
					dis = dist->DisjunctAt(valIdx);
				}
				Inc_Floyd(dis);
			}
			return;
		}
		else {

		}
	}
}