﻿#include "CmdLineOptions.h"
#include "FilesDuplicatesSeacher.h"
#include "BlockHashGetter.cpp"
#include "main.h"

using namespace std;

int main(int argc, char** argv)
{
	cout << "HW 08." << endl;

	if (!BlockHashGetter::Tests())
	{
		cerr << "BlockHashGetter::Tests() fails" << endl;
		return 1;
	}

	CmdLineOptions cmdLineOptions;

	if (cmdLineOptions.FillFromArgs(argc, argv) == false)
	{
		cout << "cmdLineOptions.FillFromArgs return false." << endl;
		return 1;
	}

	FilesDuplicatesSeacher seacher;	
	list<string_s> foundedDuplicates = seacher.Seach(cmdLineOptions);

	if (foundedDuplicates.size() == 0)
		cout << "Duplicates not founded" << endl;
	else
	{
		for (const string_s& duplicates : foundedDuplicates)
		{
			for (const string& fileName : duplicates)
				cout << fileName << endl;

			cout << endl;
		}
	}

	return 0;
}
