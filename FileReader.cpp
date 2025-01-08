#include "FileReader.h"

FileReader::FileReader(const std::string& path, unsigned blockSize, uintmax_t fileSize, HashCalculationFunctionPtr hashCalcPtr) :
    _path(path)    
{
    _fileSize = fileSize;
    _blockSize = blockSize;
    _isFullyReaded = false;
    _hashCalculationFunc = hashCalcPtr;
}

std::string FileReader::ShowInfo()
{
    return std::string(_path + "   " + _fileSize);
}

void FileReader::ReadBlock()
{
    if (!_isFullyReaded)    // For additional check
    {        
        std::vector<uint8_t> buffer(_blockSize);
        _fileStream.read(reinterpret_cast<char*>(buffer.data()), _blockSize);

        std::size_t bytesReadedNum = fileStream.gcount();

        if (bytesReadedNum < _blockSize)
        {
            std::fill(buffer.begin() + bytesReadedNum, buffer.end(), '\0');
            return;
        }

        _blocksHashes.push_back(_hashCalculationFunc(buffer));

        if (_fileStream.eof())
        {
            _isFullyReaded = true;
            _fileStream.close();
        }
    }
}
