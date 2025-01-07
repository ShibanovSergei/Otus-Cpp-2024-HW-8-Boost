#include <boost/filesystem.hpp>

#include "FilesDuplicatesSeacher.h"
#include "BlockHashGetter.cpp"


namespace fs = boost::filesystem;

list<string_s> FilesDuplicatesSeacher::Seach(CmdLineOptions& cmdLineOptions)
{
    vector<FileReader> files = getAllFiles(cmdLineOptions);

    return list<string_s>();
}

vector<FileReader> FilesDuplicatesSeacher::getAllFiles(CmdLineOptions& cmdLineOptions)
{
    vector<FileReader> result;

    SetParametersForSearchingFiles(cmdLineOptions);

    for (const string& dir : cmdLineOptions->includedDirs())
    {        
        const fs::path path = fs::path(dir);

        collectFiles(path, result);
    }

    return result;
}


// for (const auto& entry : boost::filesystem::directory_iterator(boost::filesystem::path("directory")))



void FilesDuplicatesSeacher::collectFiles(const fs::path& dir, vector<FileReader>& result)
{
    if (!fs::exists(dir) || !fs::is_directory(dir))
    {
        return;
    }

    for (const auto& entry : fs::directory_iterator(dir)) 
    {
        if (fs::is_directory(entry)) 
        {
            bool isExcluded = false;
            for (const auto& excludedDir : excludedDirs) {
                if (fs::equivalent(entry.path(), fs::path(excludedDir))) 
                {
                    isExcluded = true;
                    break;
                }
            }

            if (!isExcluded && isNested) 
            {
                collectFiles(entry.path(), excludedDirs, isNested, result);
            }
        }
        else if (fs::is_regular_file(entry)) 
        {
            result.push_back(entry.path().string());
        }
    }
}

void FilesDuplicatesSeacher::SetParametersForSearchingFiles(CmdLineOptions& cmdLineOptions)
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



/*
using string_s = std::vector<std::string>;

������� ������ ���������� ��� ���������: string_s  includedDirs
���� ��������� ������ ����� � ���� ������ � ������ �����������, ��������� boost
�������� ����� ����������� � �����������: string_s exludedDirs
���� isNested ����� ������ �� ������ � ������� ����������� ��� � � ��������������

������ ��������������� ��� �� �++




2. ������ � �������������� Boost.Filesystem
���� �� ����������� Boost, �� ����� ��������������� ����������� Boost.Filesystem, ������� ������������� ����������� ����������:

cpp
Copy
#include <iostream>
#include <boost/filesystem.hpp> // ��� Boost.Filesystem

namespace fs = boost::filesystem; // ����� ��� ��������

int main() {
    std::string filePath = "path/to/your/file.txt";

    try {
        // �������� ������ �����
        uintmax_t fileSize = fs::file_size(filePath);
        std::cout << "File size: " << fileSize << " bytes\n";
    } catch (const fs::filesystem_error & ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }

    return 0;
}
��� ��� ��������:
fs::file_size(filePath) ���������� ������ ����� � ������.

���� ���� �� ���������� ��� ��������� ������, ������������� ���������� fs::filesystem_error.


-------------------------------------------------------------------------


#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

using string_s = std::vector<std::string>;

void collectFiles(const fs::path& dir, const string_s& excludedDirs, bool isNested, string_s& result) {
    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        return;
    }

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (fs::is_directory(entry)) {
            bool isExcluded = false;
            for (const auto& excludedDir : excludedDirs) {
                if (fs::equivalent(entry.path(), fs::path(excludedDir))) {
                    isExcluded = true;
                    break;
                }
            }

            if (!isExcluded && isNested) {
                collectFiles(entry.path(), excludedDirs, isNested, result);
            }
        } else if (fs::is_regular_file(entry)) {
            result.push_back(entry.path().string());
        }
    }
}

string_s getAllFiles(const string_s& includedDirs, const string_s& excludedDirs, bool isNested) 
{
    string_s result;

    for (const auto& dir : includedDirs) {
        collectFiles(fs::path(dir), excludedDirs, isNested, result);
    }

    return result;
}

int main() {
    string_s includedDirs = {"/path/to/include1", "/path/to/include2"};
    string_s excludedDirs = {"/path/to/exclude1", "/path/to/exclude2"};
    bool isNested = true;

    string_s files = getAllFiles(includedDirs, excludedDirs, isNested);

    for (const auto& file : files) {
        std::cout << file << std::endl;
    }

    return 0;
}
*/
