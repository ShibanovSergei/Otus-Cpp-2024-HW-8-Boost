
#include <vector>
#include <string>
#include <list>
#include <fstream>

#include "BlockHashGetter.cpp"

struct FileReader
{
private:
    std::string     _path;
    std::ifstream   _fileStream;    
    unsigned        _blockSize;
    bool            _isFullyReaded;
    unsigned        _fileSize;

    std::list<BlockHashGetter::hashBytes> _blocksHashes;
    BlockHashGetter::HashCalculationFunctionPtr _hashCalculationFunc;

    void ReadBlock();

    //FileReader(const FileReader&) = delete;
    //FileReader& operator=(const FileReader&) = delete;

public:
    unsigned groupNumber;
    FileReader(const std::string& path, unsigned blockSize, unsigned fileSize, BlockHashGetter::HashCalculationFunctionPtr hashCalcPtr);
    std::string ShowInfo() const;
    std::string GetPath() const { return _path; }
    unsigned GetFileSize() const { return _fileSize; }

    bool Compare(const FileReader& file);
};
