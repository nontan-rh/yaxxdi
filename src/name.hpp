#ifndef YAXXDI_NAME_HPP
#define YAXXDI_NAME_HPP

#include <string>

#include "spec.hpp"

namespace yaxxdi {

inline std::string file_index_to_variable_name(size_t file_index) {
    return "file_data_" + std::to_string(file_index);
}

inline std::string get_find_function_name(const Spec &spec) {
    return "yaxxdi_" + spec.api_prefix + "_find";
}

} // namespace yaxxdi

#endif // YAXXDI_NAME_HPP
