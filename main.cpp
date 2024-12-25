﻿#include "CmdLineOptions.h"
#include "main.h"

using namespace std;

int main(int argc, char** argv)
{
	cout << "HW 08." << endl;
	CmdLineOptions cmdLineOptions;

	if (cmdLineOptions.FillFromArgs(argc, argv) == false)
		return 1;

	std::cout << "The following options are set" << endl;
	cmdLineOptions.ShowOptions(std::cout);

	return 0;
}
