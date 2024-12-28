#pragma once

#include <string>
#include <vector>
#include <cstdint>

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
private:
    string_s       _includedDirs;
    string_s       _excludedDirs;
    ScanLevel      _scanLevel;
    unsigned       _minFileSize;
    string_s       _fileMasks;
    unsigned       _blockSize;
    HashAlgorithm  _hashAlgorithm;

    std::ostream& out_string_s(std::ostream& os, string_s strs);

public:
    bool FillFromArgs(int argc, char** argv);
    void ShowOptions(std::ostream& os);
};


