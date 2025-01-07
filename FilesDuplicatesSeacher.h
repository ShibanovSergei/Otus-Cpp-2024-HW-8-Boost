#include "CmdLineOptions.h"

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
	 bool			_scanSubdirectories;
	 string_s       _excludedDirs;
	 bool			_scanSubdirectories;
	 unsigned       _minFileSize;
	 string_s       _fileMasks;
	 unsigned       _blockSize;

	 HashCalculationFunctionPtr  _hashFunc;
	 // -----------------------------------

	 void SetParametersForSearchingFiles(CmdLineOptions& cmdLineOptions);
};