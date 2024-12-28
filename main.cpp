#include "CmdLineOptions.h"
#include "BlockHashGetter.cpp"
#include "main.h"

using namespace std;

int main(int argc, char** argv)
{
	cout << "HW 08." << endl;
	CmdLineOptions cmdLineOptions;

	if (cmdLineOptions.FillFromArgs(argc, argv) == false)
	{
		cout << "cmdLineOptions.FillFromArgs return false." << endl;
		return 1;
	}

	cout << "The following options are set" << endl;
	cmdLineOptions.ShowOptions(std::cout);

	if (!BlockHashGetter::Tests())
	{
		cerr << "BlockHashGetter::Tests() fails" << endl;
		return 1;
	}

	return 0;
}
