#include <boost/filesystem.hpp>

#include "FilesDuplicatesSeacher.h"
#include "BlockHashGetter.cpp"


list<string_s> FilesDuplicatesSeacher::Seach(CmdLineOptions& cmdLineOptions)
{
    vector<FileReader> files = getAllFiles(cmdLineOptions);

    for (FileReader &fr : files)
    {
        cout << fr.ShowInfo() << endl;
    }

    return list<string_s>();
}

vector<FileReader> FilesDuplicatesSeacher::getAllFiles(CmdLineOptions& cmdLineOptions)
{
    std::vector<FileReader> result;

    SetParametersForSearchDuplicates(cmdLineOptions);

    for (const string& dir : cmdLineOptions.includedDirs())
    {        
        const fs::path path = fs::path(dir);

        collectFiles(path, result);
    }

    return result;
}

void FilesDuplicatesSeacher::collectFiles(const fs::path& dir, vector<FileReader>& result)
{
    if (!fs::exists(dir) || !fs::is_directory(dir))
    {
        return;
    }

    for (const auto& entry : fs::directory_iterator(dir)) 
    {
        if (fs::is_directory(entry) && _scanSubdirectories)
        {
            bool isExcluded = false;
            for (const auto& excludedDir : _excludedDirs) {
                if (fs::equivalent(entry.path(), fs::path(excludedDir))) 
                {
                    isExcluded = true;
                    break;
                }
            }

            if (!isExcluded) 
            {
                collectFiles(entry.path(), result);
            }
        }
        else if (fs::is_regular_file(entry)) 
        {
            unsigned fileSize = fs::file_size(entry);
            if (fileSize >= _minFileSize)
            {
                result.push_back(FileReader(entry.path().string(), _blockSize, fileSize, _hashFunc));
                //FileReader(const std::string & path, unsigned blockSize, unsigned fileSize, BlockHashGetter::HashCalculationFunctionPtr hashCalcPtr) :
            }
        }
    }
}

void FilesDuplicatesSeacher::SetParametersForSearchDuplicates(CmdLineOptions& cmdLineOptions)
{
    _excludedDirs = cmdLineOptions.excludedDirs();
    _scanSubdirectories = cmdLineOptions.IsScanSubdirectories();
    _minFileSize = cmdLineOptions.getMinFileSize();
    _fileMasks = cmdLineOptions.fileMasks();
    _blockSize = cmdLineOptions.getBlockSize();

    switch (cmdLineOptions.getHashAlgorithm())
    {
        case HashAlgorithm::CRC32:
        default:
            _hashFunc = BlockHashGetter::Calculate_crc32;
            break;

        case HashAlgorithm::MD5:
            _hashFunc = BlockHashGetter::Calculate_md5;
            break;
    };
}
