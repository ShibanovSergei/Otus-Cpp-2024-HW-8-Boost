#include <boost/filesystem.hpp>

#include "FilesDuplicatesSeacher.h"
#include "BlockHashGetter.cpp"


list<string_s> FilesDuplicatesSeacher::Seach(CmdLineOptions& cmdLineOptions)
{
    vector<FileReader> files = getAllFiles(cmdLineOptions);

    //for (FileReader &fr : files)
    //{
    //    cout << fr.ShowInfo() << endl;
    //}

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
            std::string fileName = entry.path().string();
            if (fileSize >= _minFileSize && isMatchToMask(fileName))
            {
                result.push_back(FileReader(fileName, _blockSize, fileSize, _hashFunc));
            }
        }
    }
}

std::string FilesDuplicatesSeacher::maskToRegex(const std::string& mask)
{
    std::string regexPattern;
    for (char ch : mask) 
    {
        switch (ch)
        {
            case '*':
                regexPattern += ".*"; // Any number of any characters
                break;
            case '?':
                regexPattern += ".";  // One any character
                break;
            case '.':
                regexPattern += "\\."; // Point shielding
                break;
            default:
                regexPattern += ch;   // Regular symbol
                break;
        }
    }
    return regexPattern;
}

bool FilesDuplicatesSeacher::isMatchToMask(const std::string& fileName) const
{
    if (_regexsForFileMasks.empty())
        return true;

    for (const std::string& regexPattern : _regexsForFileMasks)
    {        
        std::regex regex(regexPattern, std::regex_constants::icase); // Ignore case
        if (std::regex_match(fileName, regex)) 
        {
            return true;
        }
    }
    return false;
}

void FilesDuplicatesSeacher::SetParametersForSearchDuplicates(CmdLineOptions& cmdLineOptions)
{
    _excludedDirs = cmdLineOptions.excludedDirs();
    _scanSubdirectories = cmdLineOptions.IsScanSubdirectories();
    _minFileSize = cmdLineOptions.getMinFileSize();
    _blockSize = cmdLineOptions.getBlockSize();

    for (string mask : cmdLineOptions.fileMasks())
        _regexsForFileMasks.push_back(maskToRegex(mask));
    
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
