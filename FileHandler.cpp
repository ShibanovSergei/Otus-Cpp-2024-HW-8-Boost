#include "FileHandler.h"

FileHandler::FileHandler(const std::string& path, unsigned blockSize, HashCalculationFunctionPtr hashCalcPtr) :
    _path(path),
    _fileStream(_path)
{
    if (!_fileStream.is_open())
    {
        throw std::runtime_error("Failed to open file: " + _path);
    }

    _blockSize = blockSize;
    _isFullyReaded = false;
    _hashCalculationFunc = hashCalcPtr;
}

void FileHandler::ReadBlock()
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
