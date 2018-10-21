#include "pch.h"
#include "EventTracer.h"
#include <queue>

namespace event_tracer {

	EventTracer::~EventTracer(void)
	{
	}

	void EventTracer::init(void) {
		std::cout << "tracer<Search>::init()" << std::endl;
		for (unsigned int e = 0U; e < engines(); e++) {
			std::cout << "\t " << e << ": "
				<< t2s(engine(e).type());
			if (engine(e).meta()) {
				std::cout << ", engines: {";
				for (unsigned int i = engine(e).efst(); i < engine(e).elst(); i++) {
					std::cout << i;
					if (i + 1 < engine(e).elst()) std::cout << ", ";
				}
			}
			else {
				std::cout << ", workers :{ ";
				for (unsigned int i = engine(e).wfst(); i < engine(e).wlst(); i++) {
					std::cout << i;
					if (i + 1 < engine(e).wlst()) std::cout << ", ";
				}
			}
			std::cout << "}" << std::endl;
		}
	}

	void EventTracer::node(const EdgeInfo& ei, const NodeInfo& ni) {
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
		std::cout << ";" << "w：" << ni.wid() << "," << "n: " << ni.nid() << ")";
		if (ei) {
			if (ei.wid() != ni.wid())
				std::cout << "stolen from w: " << ei.wid() << "]";
			std::cout << std::endl << "\t" << ei.string() << std::endl;
		}
		else {
			std::cout << std::endl;
		}
	}

	void EventTracer::round(unsigned int eid) {
		std::cout << " trace<Search>::round(e: " << eid << ")" << std::endl;
	}

	void EventTracer::skip(const EdgeInfo& ei) {
		std::cout << "trace<Search>Search::skip(w:" << ei.wid()
			<< ", n: " << ei.nid()
			<< ", a :" << ei.alternative() << ")" << std::endl;
	}

	void EventTracer::done(void) {
		std::cout << "trace<Search>::done()" << std::endl;
	}
}