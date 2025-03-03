#include <boost/filesystem.hpp>

#include "FilesDuplicatesSeacher.h"
#include "BlockHashGetter.cpp"


list<string_s> FilesDuplicatesSeacher::Seach(CmdLineOptions& cmdLineOptions)
{
    list<FileReader> files = getAllFiles(cmdLineOptions);

    cout << "Files from: list<FileReader> files" << endl;
    for (auto& f : files)
    {
        cout << f.GetPath() << endl;
    }

    list<string_s> result;

    unsigned n = 1;
    auto curIt = files.begin();
    
    while (curIt != files.end())
    {
        while (curIt->groupNumber != 0)
        {
            curIt++;
            if (curIt == files.end()) break;
        }

        curIt->groupNumber = n;
        auto fileIt = curIt;
        for (fileIt++; fileIt != files.end(); fileIt++)
        {
            if (curIt->Compare(*fileIt))
            {
                fileIt->groupNumber = n;
            }
        }

        curIt++;
        n++;
    }

    for (unsigned i = 0; i <= n; i++)
    {
        string_s group;

        for (auto fileIt = files.begin(); fileIt != files.end(); fileIt++)
        {
            if (fileIt->groupNumber == n)
            {
                group.push_back(fileIt->GetPath());
            }
        }

        if (group.size() > 1)
        {
            group.push_back("");
            result.push_back(group);
        }
    }

    return std::move(result);
}

list<FileReader> FilesDuplicatesSeacher::getAllFiles(CmdLineOptions& cmdLineOptions)
{
    std::list<FileReader> result;

    SetParametersForSearchDuplicates(cmdLineOptions);

    for (const string& dir : cmdLineOptions.includedDirs())
    {        
        const fs::path path = fs::path(dir);

        collectFiles(path, result);
    }

    return result;
}

void FilesDuplicatesSeacher::collectFiles(const fs::path& dir, list<FileReader>& result)
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
