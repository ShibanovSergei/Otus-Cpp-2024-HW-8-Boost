#pragma once

#include <string>
#include <vector>
#include <cstdint>

using string_s = std::vector<std::string>;

enum class ScanLevel
{
    InCurrentDirectoryOnly = 0,
    InSubDirectoriesAlso = 1
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
    void RemoveIncludeDirsThatContainsInExlude();

public:
    bool FillFromArgs(int argc, char** argv);
    void ShowOptions(std::ostream& os);

    const string_s& includedDirs() const { return _includedDirs; }
    const string_s& excludedDirs() const { return _excludedDirs; }
    bool  IsScanSubdirectories() const { return _scanLevel == ScanLevel::InSubDirectoriesAlso; }
    unsigned getMinFileSize() const { return _minFileSize; }
    const string_s& fileMasks() const { return _fileMasks; }
    unsigned getBlockSize() const { return _blockSize; }
    HashAlgorithm getHashAlgorithm() const { return _hashAlgorithm; }
};


