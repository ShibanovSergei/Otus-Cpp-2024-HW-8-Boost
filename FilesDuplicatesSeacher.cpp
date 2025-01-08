#include <boost/filesystem.hpp>

#include "FilesDuplicatesSeacher.h"
#include "FileReader.h"
#include "BlockHashGetter.cpp"


namespace fs = boost::filesystem;

list<string_s> FilesDuplicatesSeacher::Seach(CmdLineOptions& cmdLineOptions)
{
    vector<FileReader> files = getAllFiles(cmdLineOptions);

    for (FileReader fr : files)
    {
        std::cout << fr.SowInfo() << std::endl;
    }

    return list<string_s>();
}

vector<FileReader> FilesDuplicatesSeacher::getAllFiles(CmdLineOptions& cmdLineOptions)
{
    vector<FileReader> result;

    SetParametersForSearchDuplicates(cmdLineOptions);

    for (const string& dir : cmdLineOptions->includedDirs())
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
            for (const auto& excludedDir : excludedDirs) {
                if (fs::equivalent(entry.path(), fs::path(excludedDir))) 
                {
                    isExcluded = true;
                    break;
                }
            }

            if (!isExcluded) 
            {
                collectFiles(entry.path(), excludedDirs, isNested, result);
            }
        }
        else if (fs::is_regular_file(entry)) 
        {
            uintmax_t fileSize = fs::file_size(entry);
            if (fileSize >= _minFileSize)
            {
                result.push_back(new FileReader(entry.path().string(), _blockSize, fileSize, _hashFunc));
            }
        }
    }
}

void FilesDuplicatesSeacher::SetParametersForSearchDuplicates(CmdLineOptions& cmdLineOptions)
{
    _excludedDirs = cmdLineOptions.excludedDirs();
    _scanSubdirectories = cmdLineOptions.scanLevel == ScanLevel.InSubDirectoriesAlso;
    _minFileSize = cmdLineOptions.minFileSize;
    _fileMasks = cmdLineOptions.fileMasks();
    _blockSize = cmdLineOptions.blockSize;

    switch (cmdLineOptions.hashAlgorithm)
    {
        case CRC32:
        default:
            _hashFunc = BlockHashGetter::Calculate_crc32;
            break;

        case MD5:
            _hashFunc = BlockHashGetter::Calculate_md5;
            break;
    };
}
