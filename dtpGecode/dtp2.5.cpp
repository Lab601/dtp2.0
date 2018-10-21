#include "pch.h"
#include <iostream>
#include "string.h"
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "ProblemDef.h"
#include "TSATParser.h"
#include "SolutionByGecode.h"
#include "EventTracer.h"

using namespace std;


int main___(int argc, char** argv)
{
	int i, j, k;
	clock_t start, finish;

	TSATParser* parser;
	DTP* problem;
	try {

		if (argc < 3)
		{
			cout << "The format of the valid command is: dtp2.0 -f dtp_filename [-n max_node] [-k max_check] [-t timeout] [-oxuvpclirgsmba]" << endl;
			cout << "\t timeout: a positive integer (seconds) for time limit." << endl;
			cout << "\t max_node: a positive integer for the maximal number of visited nodes." << endl;
			cout << "\t max_check: a positive integer for the maximal number of consistency checking." << endl;
			cout << "\t options: " << endl;
			cout << "\t     -o: mbo=1" << endl;
			cout << "\t     -x: mbo_max=1" << endl;
			cout << "\t     -u: mbo_sum=1" << endl;
			cout << "\t     -v: mbo_avg=1" << endl;
			cout << "\t     -p: mbo_exp=1" << endl;
			cout << "\t     -c: mbo_fac=1" << endl;
			cout << "\t     -l: mbo_val=1" << endl;
			cout << "\t     -i: mbo_infty=1" << endl;
			cout << "\t     -r: rsv=1" << endl;
			cout << "\t     -g: rsv2=1" << endl;
			cout << "\t     -s: sc_rsv=1" << endl;
			cout << "\t     -m: mrv=1" << endl;
			cout << "\t     -b: sb=1" << endl;
			cout << "\t     -a: sa=1" << endl;
			cout << "\t     -z: sp_val=1" << endl;
			return(1);
		}

		k = -1;
		for (i = 1; i < argc; )
		{
			if (argv[i][0] - '-') exit(1);
			switch (argv[i][1])
			{
			case 'f': if (i + 1 > argc) exit(1); k = i + 1; i = i + 2; break;
			case 'n': if (i + 1 > argc) exit(1); //sol.nodeset = atoi(argv[i + 1]); i = i + 2; break;
			case 't': if (i + 1 > argc) exit(1); //sol.timeset = atoi(argv[i + 1]); i = i + 2; break;
			case 'k': if (i + 1 > argc) exit(1); //sol.checkset = atoi(argv[i + 1]); i = i + 2; break;
			default:
			{
				for (j = 1; j < strlen(argv[i]); j++)
				{
					/*				switch (argv[i][j])
									{
									case 'm': sol.SetMRV(); break;
									case 'o': sol.SetMBO(); break;
									case 'x': sol.SetMAX(); break;
									case 'u': sol.SetSUM(); break;
									case 'v': sol.SetAVG(); break;
									case 'p': sol.SetEXP(); break;
									case 'c': sol.SetFAC(); break;
									case 'l': sol.SetVAL(); break;
									case 'i': sol.SetINFTY(); break;
									case 'r': sol.SetRSV(); break;
									case 'g': sol.SetRSV2(); break;
									case 's': sol.SetSCRSV(); break;
									case 'b': sol.SetSB(); break;
									case 'a': sol.SetSA(); break;
									case 'A': sol.SetA(); break;
									case 'z': sol.SetSP_VAL(); break;
									case 'W': sol.SetSP_MIN(); break;
									case 'X': sol.SetSP_SUM(); break;
									case 'Y': sol.SetSP_AVG(); break;
									case 'Z': sol.SetSP_EXP(); break;
									default: cout << "unknown option: " << argv[i][j] << endl; exit(0);
									}
								} */
					i++;
				}
			}
			}
			if (k < 0)
			{
				cout << "Please input a DTP file!" << endl;
				return(0);
			}

			ifstream yyyin(argv[k]);
			if (parser = new TSATParser(argv[k])) {

				problem = parser->parse();
				SolutionByGecode* s = new SolutionByGecode(problem);

				Search::Options opt;
				EventTracer* et = new EventTracer();
				opt.tracer = et;
				DFS<SolutionByGecode> e(s, opt);
			//	DFS<SolutionByGecode> e(s);
				if (SolutionByGecode* temp = e.next()) {
					temp->print();
					delete temp;
				}

				delete parser;
				delete problem;
				delete s;
			}

			//	sol.init();
			///	sol.resolve();
				//	sol.testAPSP();
			return 0;
		}
	}
	catch (exception e) {
		cout<<e.what()<<endl;
	}

}