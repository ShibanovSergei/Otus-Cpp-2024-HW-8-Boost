#include <vector>
#include <list>
#include <boost/filesystem.hpp>

#include "CmdLineOptions.h"
#include "FileReader.h"

namespace fs = boost::filesystem;

using namespace std;

class FilesDuplicatesSeacher
{
 public:
	list<string_s> Seach(CmdLineOptions& cmdLineOptions);

 private:
	 vector<FileReader> getAllFiles(CmdLineOptions& cmdLineOptions);
	 void collectFiles(const fs::path& dir, vector<FileReader>& result);
	 //void ShowResults();

	 // Parameters for searching files. 
	 // Not used as parameters of the collectFiles function, for better performance (for recursive calls for nested directories)
	 string_s       _excludedDirs;
	 bool			_scanSubdirectories;
	 unsigned       _minFileSize;
	 string_s       _fileMasks;
	 unsigned       _blockSize;

	 BlockHashGetter::HashCalculationFunctionPtr  _hashFunc;
	 // -----------------------------------

	 void SetParametersForSearchDuplicates(CmdLineOptions& cmdLineOptions);
};