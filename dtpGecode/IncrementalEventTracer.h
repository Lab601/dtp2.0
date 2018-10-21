#pragma once
#include "EventTracer.h"
#include "ProblemDef.h"
#include "utils.h"
#include "SolutionByGecode.h"

using namespace dtp_definition;

namespace event_tracer {
	class DGElement {
	public:
		unsigned from;
		unsigned to;
		double weight;
		DGElement(unsigned i, unsigned j, double w) :from(i), to(j), weight(w) {};
	};
	class IncrementalEventTracer : public EventTracer
	{
	protected:
		DTP* dtp;
		SolutionByGecode* sbg;

		vector<vector<DGElement*>*> DGbackup;

		static const char* t2s(EngineType et) {
			return("DFS Engine");
		}

		int Inc_Floyd(Disjunct* add);
		void NodeCreated(string);
	public:
		IncrementalEventTracer(void) {};
		IncrementalEventTracer(DTP* d, SolutionByGecode* s);
		~IncrementalEventTracer(void);
		//NODE
		virtual void node(const EdgeInfo& ei, const NodeInfo& ni);
	};
}