#include <cstddef>
#include <cstdint>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>

#include "name.hpp"
#include "spec.hpp"

namespace fs = std::filesystem;
using nlohmann::json;

using namespace yaxxdi;

struct GeneratedFile {
    size_t file_index;
    std::string id;
    std::string variable_name;
    size_t size;
};

void generate_source_intro(std::ostream &o, const Spec &spec) {
    o << "//" << std::endl;
    o << "// generated by yaxxdi" << std::endl;
    o << "//" << std::endl;
    o << std::endl;
    o << "#include <stddef.h>" << std::endl;
    o << "#include <stdint.h>" << std::endl;
    o << "#include <string.h>" << std::endl;
    o << std::endl;
    o << "typedef struct TAG_YAXXDI_TableEntry {" << std::endl;
    o << "    const char* id;" << std::endl;
    o << "    const uint8_t* data;" << std::endl;
    o << "    size_t size;" << std::endl;
    o << "} YAXXDI_TableEntry;" << std::endl;
}

GeneratedFile generate_source_single_file(std::ostream &o, const Spec &spec,
                                          const InputFile &file,
                                          size_t file_index) {
    std::ifstream ifs(file.path);
    if (!ifs) {
        throw std::runtime_error("Could not open file: " + file.path);
    }

    auto size = fs::file_size(file.path);

    const std::string variable_name = file_index_to_variable_name(file_index);

    o << std::endl;
    o << "// " << file.id << " - " << file.path << std::endl;

    o << "static " + spec.variable_type + " " + variable_name + "[] = {"
      << std::endl;

    uintmax_t count = 0;
    for (;;) {
        union {
            char ch;
            uint8_t val;
        } buf;
        ifs.read(&buf.ch, 1);
        if (ifs.eof()) {
            if (count != 0 && count % spec.max_cols == 0) {
                o << std::endl << "    ";
            }
            o << "0x00";
            break;
        }

        if (!ifs) {
            throw std::runtime_error("Failed to read file at " +
                                     std::to_string(count) +
                                     " in file: " + file.path);
        }

        if (count % spec.max_cols == 0) {
            if (count != 0) {
                o << std::endl;
            }
            o << "    ";
        }

        o << "0x" << std::setfill('0') << std::setw(2) << std::hex
          << (unsigned int)buf.val << std::setfill(' ') << std::setw(0)
          << std::dec << ",";
        ++count;
    }

    o << std::endl;
    o << "};" << std::endl;

    if (size != count) {
        throw std::logic_error("File size and count of read bytes mismatch: (" +
                               std::to_string(size) +
                               " != " + std::to_string(count) + ")" +
                               " for file: " + file.path);
    }

    GeneratedFile generated_file;
    generated_file.file_index = file_index;
    generated_file.id = file.id;
    generated_file.variable_name = variable_name;
    generated_file.size = size;

    return generated_file;
}

std::vector<GeneratedFile> generate_source_all_files(std::ostream &o,
                                                     const Spec &spec) {
    auto num_files = spec.input_files.size();
    std::vector<GeneratedFile> generated_files;
    for (size_t file_index = 0; file_index < num_files; file_index++) {
        const auto &file = spec.input_files[file_index];
        auto generated_file =
            generate_source_single_file(o, spec, file, file_index);
        generated_files.push_back(generated_file);
    }

    return generated_files;
}

void generate_source_table(std::ostream &o, const Spec &spec,
                           const std::vector<GeneratedFile> &generated_files) {
    o << std::endl;
    o << "static YAXXDI_TableEntry yaxxdi_table_entry[] = {" << std::endl;
    auto num_files = spec.input_files.size();
    for (const auto &generated_file : generated_files) {
        o << "    { \"" << generated_file.id << "\", "
          << generated_file.variable_name << ", " << generated_file.size
          << " }," << std::endl;
    }
    o << "    { NULL, NULL, 0 }," << std::endl;
    o << "};" << std::endl;
}

void generate_source_api(std::ostream &o, const Spec &spec,
                         const std::vector<GeneratedFile> &generated_files) {
    const auto find_function_name = get_find_function_name(spec);

    o << std::endl;
    o << "int " + find_function_name + "(const char* id, const "
      << spec.variable_type << "** data, size_t *size) {" << std::endl;
    o << "    for (int i = 0; i < " << generated_files.size() << "; i++) {"
      << std::endl;
    o << "        if (strcmp(id, yaxxdi_table_entry[i].id) == 0) {"
      << std::endl;
    o << "            *data = yaxxdi_table_entry[i].data;" << std::endl;
    o << "            *size = yaxxdi_table_entry[i].size;" << std::endl;
    o << "            return 0;" << std::endl;
    o << "        }" << std::endl;
    o << "    }" << std::endl;
    o << std::endl;
    o << "    return 1;" << std::endl;
    o << "}" << std::endl;
}

void generate_source(std::ostream &o, const Spec &spec) {
    generate_source_intro(o, spec);
    auto generated_files = generate_source_all_files(o, spec);
    generate_source_table(o, spec, generated_files);
    generate_source_api(o, spec, generated_files);
}

int main(int argc, char *argv[]) {
    cxxopts::Options options("yaxxdi-gen-source",
                             "C Source Generator of Yet Another `xxd -i'");
    options.add_options()                                           //
        ("b,base", "Base directory", cxxopts::value<std::string>()) //
        ("o,output", "Output source file path",
         cxxopts::value<std::string>()->default_value("-")) //
        ("help", "Print help")                              //
        ;
    const auto args = options.parse(argc, argv);
    if (args.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (!args.count("base")) {
        std::cout << "--base options is required" << std::endl;
        return 1;
    }
    std::string base = args["base"].as<std::string>();

    if (argc != 2) {
        std::cout << options.help() << std::endl;
        return 1;
    }
    std::string spec_path = argv[1];

    Spec spec;
    {
        json j;
        if (spec_path == "-") {
            std::cin >> j;
        } else {
            std::ifstream ifs(spec_path);
            if (!ifs) {
                std::cout << "failed to open spec file: " << spec_path
                          << std::endl;
            }
            ifs >> j;
        }
        spec = j;
    }

    {
        const auto source_path_str = args["output"].as<std::string>();
        if (source_path_str == "-") {
            generate_source(std::cout, spec);
        } else {
            std::ofstream ofs(source_path_str);
            if (!ofs) {
                std::cout << "failed to open output source file: "
                          << source_path_str << std::endl;
                return 1;
            }

            generate_source(ofs, spec);
        }
    }

    return 0;
}