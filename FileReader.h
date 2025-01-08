
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
    uintmax_t       _fileSize;

    std::list<BlockHashGetter::hashBytes> _blocksHashes;

    using HashCalculationFunctionPtr = hashBytes(*)(const std::vector<uint8_t>&);
    HashCalculationFunctionPtr _hashCalculationFunc;

    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;

public:
    FileReader(const std::string& path, unsigned blockSize, uintmax_t fileSize, HashCalculationFunctionPtr hashCalcPtr);
    std::string ShowInfo();

    void ReadBlock();
};
