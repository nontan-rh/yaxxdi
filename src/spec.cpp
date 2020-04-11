#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "spec.hpp"

using ::nlohmann::json;

namespace yaxxdi {

void to_json(json &j, const InputFile &o) {
    j = json{
        {
            "id",
            o.id,
        },
        {
            "path",
            o.path,
        },
    };
}

void from_json(const json &j, InputFile &o) {
    j.at("id").get_to(o.id);
    j.at("path").get_to(o.path);
}

void to_json(json &j, const Spec &o) {
    j = json{
        {
            "max_cols",
            o.max_cols,
        },
        {
            "api_prefix",
            o.api_prefix,
        },
        {
            "include_guard",
            o.include_guard,
        },
        {
            "variable_type",
            o.variable_type,
        },
        {
            "input_files",
            o.input_files,
        },
    };
}

void from_json(const json &j, Spec &o) {
    j.at("max_cols").get_to(o.max_cols);
    j.at("api_prefix").get_to(o.api_prefix);
    j.at("include_guard").get_to(o.include_guard);
    j.at("variable_type").get_to(o.variable_type);
    j.at("input_files").get_to(o.input_files);
}

} // namespace yaxxdi
