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
            ("depth,d", po::value<short>()->default_value(0)->notifier(checkDepthValue), "Scan depth, 1 - all directories, 0 - current folder only. Default value is 0")
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
    }
    catch (const boost::program_options::error& ex)
    {
        std::cout << "Error while setuping: " << ex.what() << '\n';
    }

    return true;
}
