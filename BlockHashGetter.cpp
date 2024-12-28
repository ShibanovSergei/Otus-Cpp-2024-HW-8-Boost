#include <boost/crc.hpp>
#include <iostream>
#include <vector>

namespace BlockHashGetter
{
    uint32_t Calculate_crc32(const std::vector<uint8_t>& dataBlock)
    {
        boost::crc_32_type crc;
        crc.process_bytes(dataBlock.data(), dataBlock.size());
        return crc.checksum();
    }

    static bool Tests()
    {
        std::vector<uint8_t> data = { 0x01, 0x02, 0x03, 0x04, 0x05 };

        uint32_t crc32_hash = Calculate_crc32(data);
        if (crc32_hash != 0x470b99f4)
            return false;

        return true;
    }
}