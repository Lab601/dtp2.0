#pragma once

#include "ProblemDef.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace dtp_definition;

class TSATParser
{
private:
	string fileName="";

	void parseCNF(DTP*, string);


public:
	TSATParser();
	TSATParser(string);
	inline void setFileName(string str) { fileName = str; };
	~TSATParser();
	DTP* parse();
};

