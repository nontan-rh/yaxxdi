#include <filesystem>
#include <fstream>

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>

#include "spec.hpp"

namespace fs = std::filesystem;
using nlohmann::json;

int main(int argc, char *argv[]) {
    cxxopts::Options options("yaxxdi-gen-spec",
                             "Spec Generator of Yet Another `xxd -i'");
    options.add_options() //
        ("o,output", "Output file",
         cxxopts::value<std::string>()->default_value("-")) //
        ("b,base", "Base directory",
         cxxopts::value<std::string>()) //
        ("p,prefix", "API prefix",
         cxxopts::value<std::string>()->default_value("default")) //
        ("g,guard", "Include guard",
         cxxopts::value<std::string>()->default_value("YAXXDI_H_INCLUDED")) //
        ("i,indent", "Indent",
         cxxopts::value<int>()->default_value("2")) //
        ("h,help", "Print help")                    //
        ;

    const auto args = options.parse(argc, argv);
    if (args.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    yaxxdi::Spec spec;
    spec.max_cols = 16;
    spec.variable_type = "uint8_t";
    spec.api_prefix = args["prefix"].as<std::string>();
    spec.include_guard = args["guard"].as<std::string>();

    if (!args.count("base")) {
        std::cout << "--base options is required" << std::endl;
    }
    const auto base = args["base"].as<std::string>();

    for (int index = 1; index < argc; index++) {
        const auto directory_path = fs::path(argv[index]);
        if (!fs::exists(directory_path)) {
            std::cout << "directory: " << directory_path << " not found"
                      << std::endl;
            return 1;
        }

        for (const auto &entry :
             fs::recursive_directory_iterator(directory_path)) {
            const auto status = fs::status(entry);
            if (!fs::is_regular_file(status)) {
                continue;
            }

            yaxxdi::InputFile f;
            f.id = fs::relative(entry.path(), directory_path).generic_string();
            f.path = fs::relative(entry.path(), base).generic_string();
            spec.input_files.push_back(f);
        }
    }

    std::sort(std::begin(spec.input_files), std::end(spec.input_files));

    const auto indent = args["indent"].as<int>();
    const auto spec_str = static_cast<json>(spec).dump(indent);

    const auto output = args["output"].as<std::string>();
    if (output == "-") {
        std::cout << spec_str;
    } else {
        std::ofstream ofs(output);
        if (!ofs) {
            std::cout << "Failed to open output file: " << output << std::endl;
            return 1;
        }

        ofs << spec_str;
    }

    return 0;
}
