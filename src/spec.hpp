#ifndef YAXXDI_SPEC_HPP
#define YAXXDI_SPEC_HPP

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace yaxxdi {

struct InputFile {
    std::string id;
    std::string path;

    bool operator<(const InputFile &other) const { return this->id < other.id; }
};

struct Spec {
    int max_cols;
    std::string api_prefix;
    std::string include_guard;
    std::string variable_type;
    std::vector<InputFile> input_files;
};

void to_json(nlohmann::json &j, const InputFile &o);
void from_json(const nlohmann::json &j, InputFile &o);
void to_json(nlohmann::json &j, const Spec &o);
void from_json(const nlohmann::json &j, Spec &o);


} // namespace yaxxdi

#endif // YAXXDI_SPEC_HPP
