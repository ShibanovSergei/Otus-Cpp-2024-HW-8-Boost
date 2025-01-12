#pragma once

#include <iostream>
#include <vector>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>

namespace BlockHashGetter
{
    using hashBytes = std::vector<uint8_t>;
    using HashCalculationFunctionPtr = hashBytes(*)(const std::vector<uint8_t>&);

    constexpr uint8_t crc32_hash_len = 4;
    constexpr uint8_t md5_hash_len = 16;    

    static hashBytes Calculate_crc32(const std::vector<uint8_t>& dataBlock)
    {
        boost::crc_32_type crc;
        crc.process_bytes(dataBlock.data(), dataBlock.size());
        uint32_t checkSum = crc.checksum();
        const uint8_t* hash = reinterpret_cast<const uint8_t*>(&checkSum);
        return hashBytes(hash, hash + 4);
    }

    // Since we use the result to compare data blocks for equality, this implementation option will suit us.
    static hashBytes Calculate_md5(const std::vector<uint8_t>& dataBlock)
    {
        boost::uuids::detail::md5 md5;
        boost::uuids::detail::md5::digest_type digest;
        md5.process_bytes(dataBlock.data(), dataBlock.size());
        md5.get_digest(digest);
        const uint8_t* hash = reinterpret_cast<const uint8_t*>(&digest);
        return hashBytes(hash, hash + sizeof(digest));
    }

    static bool Tests()
    {
        std::vector<uint8_t> data = { 0x01, 0x02, 0x03, 0x04, 0x05 };


        // For crc32
        // ------------------------------
        hashBytes crc32_hash = Calculate_crc32(data);
        if (crc32_hash.size() != 4)
            return false;

        std::vector<uint8_t> crc32_hash_expected = { 0xF4, 0x99, 0x0B, 0x47 };
        for (uint8_t i = 0; i < 4; i++)
            if (crc32_hash[i] != crc32_hash_expected[i])
            {
                std::cout << "For crc32 test: expected: " << static_cast<int>(crc32_hash_expected[i]) <<
                    "   but was: " << static_cast<int>(crc32_hash[i]) << std::endl;
                return false;
            }


        // For md5
        // ---------------------------------
        hashBytes md5_hash = Calculate_md5(data);
        if (md5_hash.size() != 16)
            return false;

        std::vector<uint8_t> md5_hash_expected = { 0x78, 0xD0, 0xFD, 0x7C, 0x5D, 0x29, 0xB3, 0x89, 0x14, 0x09, 0x55, 0x6A, 0x68, 0xE0, 0x35, 0xAB };
        for (uint8_t i = 0; i < 8; i++)
            if (md5_hash[i] != md5_hash_expected[i])
            {
                std::cout << "For md5 test: expected: " << static_cast<int>(md5_hash_expected[i]) <<
                    "   but was: " << static_cast<int>(md5_hash[i]) << std::endl;
                return false;
            }

        return true;
    }
}