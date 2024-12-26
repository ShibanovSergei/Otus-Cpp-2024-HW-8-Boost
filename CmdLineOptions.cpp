#include <boost/program_options.hpp>
#include <iostream>

#include "CmdLineOptions.h"

namespace po = boost::program_options;

bool CmdLineOptions::FillFromArgs(int argc, char** argv)
{
    try
    {
        auto checkDepthValue = [](const int& value)
            {
                if (value != 0 && value != 1) throw po::validation_error(po::validation_error::invalid_option_value, "depth");
            };

        auto checkSizeValue = [](const int& value)
            {
                if (value < 1) throw po::validation_error(po::validation_error::invalid_option_value, "block-size");
            };

        auto checkAlgorithmValue = [](const std::string& value)
            {
                if (value == "CRC32" || value == "MD5")
                    return;

                throw po::validation_error(po::validation_error::invalid_option_value, "algorithm");
            };


        po::options_description desc("Allowed options: ");
        desc.add_options()
            ("help,h", "help message")
            ("include-dirs,i", po::value<string_s>()->default_value(string_s{ "." }, ".")->multitoken()->composing(), "Including to search directories, can be multiple. Default is current folder")
            ("exclude-dirs,e", po::value<string_s>()->multitoken()->composing(), "Excluded to search directories, can be multiple")
            ("depth,d", po::value<int>()->default_value(0)->notifier(checkDepthValue), "Scan depth, 1 - all directories, 0 - current folder only. Default value is 0")
            ("min-size,m", po::value<int>()->default_value(1)->notifier(checkSizeValue), "Minimum size of the processed file in bytes. Default value is 1")
            ("file-masks,f", po::value<string_s>()->multitoken()->composing(), "Masks for the files participating in comparison")
            ("block-size,b", po::value<int>()->default_value(256)->notifier(checkSizeValue), "The size in bytes of the block to read the files with. Default value is 256")
            ("algorithm,a", po::value<std::string>()->default_value("CRC32")->notifier(checkAlgorithmValue), "Hashing algorithm to hash file blocks. Default value is CRC32");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return false;
        }

        _includedDirs = vm["include-dirs"].as<string_s>();
        if (vm.count("exclude-dirs"))
            _excludedDirs = vm["exclude-dirs"].as<string_s>();
        if (vm.count("file-masks"))
            _fileMasks = vm["file-masks"].as<string_s>();

        _scanLevel = ScanLevel(vm["depth"].as<short>());
        _minFileSize = vm["min-size"].as<unsigned>();
        _blockSize = vm["block-size"].as<unsigned short>();
        
        if (vm["algorithm"].as<std::string>() == "CRC32")
            _hashAlgorithm = HashAlgorithm::CRC32;
        if (vm["algorithm"].as<std::string>() == "MD5")
            _hashAlgorithm = HashAlgorithm::MD5;
    }
    catch (const boost::program_options::error& ex)
    {
        std::cerr << "Error while setuping: " << ex.what() << std::endl;
        return false;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Error while setuping: " << ex.what() << std::endl;
        return false;
    }

    return true;
}

void CmdLineOptions::ShowOptions(std::ostream& os)
{
    os << "include-dirs: ";
    out_string_s(os, _includedDirs) << std::endl
        << "exclude-dirs: ";
    out_string_s(os, _excludedDirs) << std::endl
        << "depth: " << (short)_scanLevel << std::endl
        << "min-size: " << _minFileSize << std::endl
        << "file-masks: ";
    out_string_s(os, _fileMasks) << std::endl
        << "block-size: " << _blockSize << std::endl
        << "algorithm: " << (short)_hashAlgorithm << std::endl;
}

std::ostream& CmdLineOptions::out_string_s(std::ostream& os, string_s strs)
{
    if (strs.empty())
    {
        os << std::string("<null>");
        return os;
    }

    auto penultimate = std::prev(strs.end());
    for (auto it = strs.begin(); it != penultimate; ++it)
        os << *it << ", ";

    os << *penultimate;

    return os;
}
