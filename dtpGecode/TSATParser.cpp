#include "pch.h"
#include "TSATParser.h"

using namespace dtp_definition;

TSATParser::TSATParser()
{
}

TSATParser::TSATParser(string str) : fileName(str) {

}


TSATParser::~TSATParser()
{
}

void TSATParser::parseCNF(DTP* dtp, string completeStr) {

	string clause = "";
	//dilimeter for 'V', '-', '<=' 
	std::string::size_type pos, posV, posS, posE;


	string str, disjunct, var1, var2, right;
	bool flag = false;

	if (dtp == NULL) return;

	while (1) {
		pos = completeStr.find('^');
		if (pos != string::npos) {
			str = completeStr.substr(0, pos);
			completeStr = completeStr.substr(pos + 1, string::npos);
		}
		else str = completeStr;
		dtp->AddDisjunction(str);
		if (pos == string::npos) break;
	}
}

DTP* TSATParser::parse() {
	string completeStr = "", str;
	bool end;

	try {
		fstream *ifs = new fstream(fileName);
		if (ifs == NULL) throw new exception(("File: " + fileName + " not found.").c_str());
		while (1) {
			//	while(*ifs>>str) {
			if (ifs->eof()) break;
			std::getline(*ifs, str);
			str.erase(0, str.find_first_not_of(" "));
			str.erase(str.find_last_not_of(" ") + 1);
			cout << str << endl;
			if (str.empty() || str.at(0) == '#') continue;
			completeStr = completeStr.append(str.c_str());
		}
		cout << completeStr << endl;
		DTP* dtp = new DTP();
		parseCNF(dtp, completeStr);
		return dtp;
	}
	catch (exception e) {
		cout << e.what() << endl;
		return NULL;
	}
}


