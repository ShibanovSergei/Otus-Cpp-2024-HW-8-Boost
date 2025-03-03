#include "FileReader.h"

FileReader::FileReader(const std::string& path, unsigned blockSize, unsigned fileSize, BlockHashGetter::HashCalculationFunctionPtr hashCalcPtr) :
    _path(path)    
{
    _fileSize = fileSize;
    _blockSize = blockSize;
    _isFullyReaded = false;
    _hashCalculationFunc = hashCalcPtr;
    groupNumber = 0;
}

std::string FileReader::ShowInfo() const
{
    return std::string(_path + "   " + std::to_string(_fileSize));
}

bool FileReader::Compare(FileReader& other)
{
    if (_fileSize != other._fileSize)
        return false;
    else
        return true;

    auto it1 = _blocksHashes.begin();
    auto it2 = other._blocksHashes.begin();

    while (true) 
    {
        if (it1 == _blocksHashes.end()) {
            ReadBlock();
            it1 = std::prev(_blocksHashes.end());
        }
        if (it2 == other._blocksHashes.end()) {
            other.ReadBlock();
            it2 = std::prev(other._blocksHashes.end());
        }
        
        if (*it1 != *it2) {
            return false;
        }
        
        if (_isFullyReaded && other._isFullyReaded) {
            return true;
        }
        
        ++it1;
        ++it2;
    }
}

void FileReader::ReadBlock()
{
    if (!_isFullyReaded)    // For additional check
    {        
        std::vector<uint8_t> buffer(_blockSize);
        _fileStream.read(reinterpret_cast<char*>(buffer.data()), _blockSize);

        std::size_t bytesReadedNum = _fileStream.gcount();

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
