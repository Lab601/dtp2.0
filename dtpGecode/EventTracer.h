#pragma once
#include <gecode/search.hh>

using namespace Gecode;

namespace event_tracer {
//namespace gecode_slution {
class EventTracer : public SearchTracer
{
protected:

	static const char* t2s(EngineType et) {
		return("DFS Engine");
	}

public:
	EventTracer(void);
	~EventTracer(void);
	//INIT
	virtual void init(void);
	//NODE
	virtual void node(const EdgeInfo& ei, const NodeInfo& ni);
	//ROUND
	virtual void round(unsigned int eid);
	//SKIP
	virtual void skip(const EdgeInfo& ei);
	//DONE
	virtual void done(void);
};


}
