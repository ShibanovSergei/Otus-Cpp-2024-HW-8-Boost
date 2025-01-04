
#include <vector>
#include <string>
#include <list>
#include <fstream>

#include "BlockHashGetter.cpp"

struct FileHandler
{
private:
    std::string        _path;
    std::ifstream      _fileStream;    
    unsigned           _blockSize;
    bool               _isFullyReaded;

    std::list<BlockHashGetter::hashBytes> _blocksHashes;

    using HashCalculationFunctionPtr = hashBytes(*)(const std::vector<uint8_t>&);
    HashCalculationFunctionPtr _hashCalculationFunc;

    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

public:
    FileHandler(const std::string& path, unsigned blockSize, HashCalculationFunctionPtr hashCalcPtr);

    void ReadBlock();
};
