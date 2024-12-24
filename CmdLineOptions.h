#pragma once

#include <string>
#include <vector>

typedef std::vector<std::string> string_s;

enum class ScanLevel
{
    CurrentDirectory = 0,
    IncludedDirectory = 1
};

enum class HashAlgorithm
{
    CRC32,
    MD5
};

struct CmdLineOptions
{
    string_s       _includedDirs;
    string_s       _excludedDirs;
    ScanLevel      _scanLevel;
    unsigned       _minFileSize;
    string_s       _fileMasks;
    unsigned short _blockSize;
    HashAlgorithm  _hashAlgorithm;

    bool FillFromArgs(int argc, char** argv);
};
