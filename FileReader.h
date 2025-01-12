
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

    //FileReader(const FileReader&) = delete;
    //FileReader& operator=(const FileReader&) = delete;

public:
    FileReader(const std::string& path, unsigned blockSize, unsigned fileSize, BlockHashGetter::HashCalculationFunctionPtr hashCalcPtr);
    std::string ShowInfo();

    void ReadBlock();
};
